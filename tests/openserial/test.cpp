/*
 * tests/openserial/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <iomanip>

static struct
{
	const char* item;
	unsigned    value;
} speedMap[] =
{
	{ "B0", 0000000 },
	{ "B50", 0000001 },
	{ "B75", 0000002 },
	{ "B110", 0000003 },
	{ "B134", 0000004 },
	{ "B150", 0000005 },
	{ "B200", 0000006 },
	{ "B300", 0000007 },
	{ "B600", 0000010 },
	{ "B1200", 0000011 },
	{ "B1800", 0000012 },
	{ "B2400", 0000013 },
	{ "B4800", 0000014 },
	{ "B9600", 0000015 },
	{ "B19200", 0000016 },
	{ "B38400", 0000017 },
	{ "B57600", 0010001 },
	{ "B115200", 0010002 },
	{ "B230400", 0010003 },
	{ "B460800", 0010004 },
	{ "B500000", 0010005 },
	{ "B576000", 0010006 },
	{ "B921600", 0010007 },
	{ "B1000000", 0010010 },
	{ "B1152000", 0010011 },
	{ "B1500000", 0010012 },
	{ "B2000000", 0010013 },
	{ "B2500000", 0010014 },
	{ "B3000000", 0010015 },
	{ "B3500000", 0010016 },
	{ "B4000000", 0010017 }
};

const char* speedName(unsigned value)
{
	for (unsigned i = 0; i < sizeof(speedMap) / sizeof(*speedMap); ++i)
	{
		if (speedMap[i].value == value)
			return speedMap[i].item;
	}
	return "Not found";
}

struct __attribute__((packed)) StartedFrame
{
	unsigned char addr;
	union
	{
		unsigned char cmd[2];
		uint16_t      cmd16;
	};
	
	friend std::ostream& operator<<(std::ostream& os, const StartedFrame& af)
	{
		std::ostream::sentry init(os);
		if (init)
		{
			os << "addr=" << (int)af.addr
				<< " cmd=0x" << std::setfill('0') << std::setw(4)
				<< std::hex << (int)af.cmd16 << std::dec;
		}
		return os;
	}
};
struct __attribute__((packed)) AccelFrame
{
	unsigned char addr;
	union
	{
		unsigned char cmd[2];
		uint16_t      cmd16;
	};
	union
	{
		struct
		{
			unsigned char withPos:4;
			unsigned char btnCode:4;
		};
		unsigned char status;
	};
	unsigned char x;
	unsigned char y;
	unsigned char z;

	bool isEmpty() { return status == 0xFF; }

	friend std::ostream& operator<<(std::ostream& os, const AccelFrame& af)
	{
		std::ostream::sentry init(os);
		if (init)
		{
			os << "addr=" << (int)af.addr
				<< " cmd=0x" << std::setfill('0') << std::setw(4)
				<< std::hex << (int)af.cmd16 << std::dec
				<< " btnCode=" << (int)af.btnCode
				<< " withPos=" << (int)af.withPos
				//<< " x=" << (int)af.x << " y=" << (int)af.y << " z=" << (int)af.z
				<< " x%=" << af.x * 100 / 255
				<< " y%=" << af.y * 100 / 255
				<< " z%=" << af.z * 100 / 255
				;
		}
		return os;
	}
};

AccelFrame readAccFrame(int fd)
{
	AccelFrame af;
	assert(sizeof(af) == 7);
	int ret = read(fd, &af, sizeof(af));
	if (ret != sizeof(af))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret != sizeof(af))
		af.status = 0xFF;
	if (ret < 0)
		perror(0);
	return af;
}

StartedFrame readStartedFrame(int fd)
{
	StartedFrame sf;
	assert(sizeof(sf) == 3);
	int ret = read(fd, &sf, sizeof(sf));
	if (ret != sizeof(sf))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return sf;
}

#define ADDR 0xFF

bool startAccessPoint(int fd, char addr = ADDR)
{
	std::cout << "Starting access point..." << std::endl;
	static const char msg[] = { addr, 0x07, 0x03 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
}

bool stopAccessPoint(int fd, char addr = ADDR)
{
	std::cout << "Stopping access point..." << std::endl;
	static const char msg[] = { addr, 0x09, 0x03 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
}

bool accDataRequest(int fd, char addr = ADDR)
{
	static const char msg[] = { addr, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00 };
	assert(sizeof(msg) == 7);
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
}

void doTest(const char* line, bool stop = false)
{
	int fd = open(line, O_RDWR);
	if (fd < 0)
	{
		std::cout << "Unabled to open " << line << std::endl;
		perror(0);
		return;
	}

	goto ok;
err:
	perror(0);
	close(fd);
	return;
ok:

	struct termios ios;
	if (tcgetattr(fd,  &ios) < 0)
		goto err;
	speed_t speed = cfgetispeed(&ios);
	std::cout << "current in bps -> " << speedName(speed) << std::endl;
	speed = cfgetospeed(&ios);
	std::cout << "current out bps -> " << speedName(speed) << std::endl;

	ios.c_cc[VMIN] = 1;
	ios.c_cc[VTIME] = 0;
	ios.c_lflag = 0;
	ios.c_iflag = 0;
	ios.c_oflag = 0;

	if (cfsetospeed(&ios, B115200) < 0)
		goto err;
	if (cfsetispeed(&ios, 0) < 0)
		goto err;
	if (tcflush(fd, TCIFLUSH) < 0)
		goto err;
	if (tcsetattr(fd, TCSANOW, &ios) < 0)
		goto err;

	if (tcgetattr(fd,  &ios) < 0)
		goto err;
	speed = cfgetispeed(&ios);
	std::cout << "current in bps -> " << speedName(speed) << std::endl;
	speed = cfgetospeed(&ios);
	std::cout << "current out bps -> " << speedName(speed) << std::endl;

	if (!stop)
	{
		startAccessPoint(fd);
		StartedFrame sf = readStartedFrame(fd);
		std::cout << sf << std::endl;

		for (;;)
		{
			accDataRequest(fd);
			AccelFrame af = readAccFrame(fd);
			if (!af.isEmpty())
				std::cout << af << std::endl;
		}
	}
	if (stop)
	{
		stopAccessPoint(fd);
		StartedFrame sf = readStartedFrame(fd);
		std::cout << sf << std::endl;
	}
	close(fd);
}

int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: " << argv[0] << " /dev/ttyS0 [STOP]" << std::endl;
		return -1;
	}
	doTest(argv[1], argc == 3);
	return 0;
}
