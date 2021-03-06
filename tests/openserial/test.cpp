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
#include <sstream>

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

struct __attribute__((packed)) SimplePacket
{
	unsigned char addr;
	union
	{
		unsigned char cmd[2];
		uint16_t      cmd16;
	};
	
	friend std::ostream& operator<<(std::ostream& os, const SimplePacket& af)
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

struct __attribute__((packed)) SyncPacket
{
	unsigned char addr;
	union
	{
		unsigned char cmd[2];
		uint16_t      cmd16;
	};
	char status;
	char useMetric:1;
	char hour:7;
	char minute;
	char second;
	uint16_t year;
	char month;
	char day;
	char alarmHour;
	char alarmMinute;
	uint16_t temperature; // *10
	uint16_t altitude;
	char padding[5];
	
	friend std::ostream& operator<<(std::ostream& os, const SyncPacket& af)
	{
		std::ostream::sentry init(os);
		if (init)
		{
			os << "addr=" << (int)af.addr
				<< " cmd=0x" << std::setfill('0') << std::setw(4)
				<< std::hex << (int)af.cmd16 << std::dec
				<< " status=" << (int)af.status
				<< " usemetric=" << std::boolalpha << (bool)af.useMetric
				<< " time=" << (int)af.hour << ":" << (int)af.minute << ":" << (int)af.second
				<< " date=" << (int)af.month << "/" << (int)af.day << "/" << (int)af.year
				<< " alarm=" << (int)af.alarmHour << ":" << (int)af.alarmMinute
				<< " temp=" << af.temperature / 10.0f
				<< " alt=" << (int)af.altitude;
			os << " padding=";
			unsigned i = 0;
			goto start;
			for (; i < sizeof(af.padding); ++i)
			{
				os << ",";
start:
				os << (int)af.padding[i];
			}
		}
		return os;
	}
};

struct __attribute__((packed)) StatusPacket
{
	unsigned char addr;
	union
	{
		unsigned char cmd[2];
		uint16_t      cmd16;
	};
	char status;
	
	friend std::ostream& operator<<(std::ostream& os, const StatusPacket& af)
	{
		std::ostream::sentry init(os);
		if (init)
		{
			os << "addr=" << (int)af.addr
				<< " cmd=0x" << std::setfill('0') << std::setw(4)
				<< std::hex << (int)af.cmd16 << std::dec
				<< " status=" << (int)af.status;
		}
		return os;
	}
};

struct __attribute__((packed)) AccelPacket
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
			unsigned char curMode:4;
			unsigned char btnCode:4;
		};
		unsigned char status;
	};
	char x;
	char y;
	char z;

	bool isEmpty() { return status == 0xFF; }

	friend std::ostream& operator<<(std::ostream& os, const AccelPacket& af)
	{
		std::ostream::sentry init(os);
		if (init)
		{
			os << "addr=" << (int)af.addr
				<< " cmd=0x" << std::setfill('0') << std::setw(4)
				<< std::hex << (int)af.cmd16 << std::dec
				<< " btnCode=" << (int)af.btnCode
				<< " curMode=" << (int)af.curMode
				<< " x=" << (int)af.x << " y=" << (int)af.y << " z=" << (int)af.z
				/*<< " x%=" << af.x * 100 / 255
				<< " y%=" << af.y * 100 / 255
				<< " z%=" << af.z * 100 / 255*/
				;
		}
		return os;
	}
};

SyncPacket readSyncPacket(int fd)
{
	SyncPacket af;
	int ret = read(fd, &af, sizeof(af));
	if (ret != sizeof(af))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return af;
}

AccelPacket readAccPacket(int fd)
{
	AccelPacket af;
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

SimplePacket readSimplePacket(int fd)
{
	SimplePacket sf;
	assert(sizeof(sf) == 3);
	int ret = read(fd, &sf, sizeof(sf));
	if (ret != sizeof(sf))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return sf;
}

StatusPacket readStatusPacket(int fd)
{
	StatusPacket sf;
	int ret = read(fd, &sf, sizeof(sf));
	if (ret != sizeof(sf))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return sf;
}

#define ADDR 0xFF

bool resetHW(int fd, char addr = ADDR)
{
	std::cout << "Reseting access point..." << std::endl;

	static const char msg[] = { addr, 0x01, 0x03 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret write: " << ret << std::endl;
	if (ret < 0)
		perror("status will be not read");
	else
	{
		SimplePacket sp = readSimplePacket(fd);
		std::cout << "response: " << sp << std::endl;
	}
	return (ret == sizeof(msg));
}

bool sendData(int fd, const std::string& data, char addr = ADDR)
{
	std::cout << "Sending data: " << data << std::endl;

	int len = data.size() / 2 + 1;
	char msg[len];
	msg[0] = addr;
	unsigned j = 1;
	for (unsigned i = 0; i < data.size(); i += 2)
	{
		std::string str;
		str.push_back(data[i]);
		str.push_back(data[i+1]);
		std::istringstream is(str);
		int v;
		is >> std::hex >> v;
		msg[j++] = v;
	}

	int ret = write(fd, msg, len);
	if (ret != len)
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	else
		std::cout << "sent!" << std::endl;
	return true;
}

bool startAccessPointSyncMode(int fd, char addr = ADDR)
{
	std::cout << "Starting access point... SYNC mode" << std::endl;
	static const char msg[] = { addr, 0x30, 0x03 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return true;
}

bool startAccessPoint(int fd, char addr = ADDR)
{
	std::cout << "Starting access point..." << std::endl;
	static const char msg[] = { addr, 0x07, 0x03 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror(0);
	return true;
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
	return true;
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
	return true;
}

void readAndPrintStatus(int fd, char addr = ADDR, bool r = true)
{
	std::cout << "Reading hw status..." << std::endl;
	static const char msg[] = { addr, 0x00, 0x04, 0x00 };
	int ret = write(fd, msg, sizeof(msg));
	if (ret != sizeof(msg))
		std::cout << __func__ << " ret: " << ret << std::endl;
	if (ret < 0)
		perror("status will be not read");
	else
	{
		if (!r)
		{
			std::cout << "ask me to not read response" << std::endl;
			return;
		}
		StatusPacket sp = readStatusPacket(fd);
		std::cout << "status: " << sp << std::endl;
	}
}

void doTest(const char* line, int action, const std::string& data)
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

	if (action == 7)
	{
		ios.c_cc[VMIN] = 0;
		ios.c_cc[VTIME] = 10;
	}
	else
	{
		ios.c_cc[VMIN] = 1;
		ios.c_cc[VTIME] = 0;
	}
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

	if (action == 7)
	{
		char b[20];
		int r;
		int i = 0;
		std::cout << "Cleaning..." << std::endl;
		while (i == 0)
		{
			do
			{
				r = read(fd, &b, 20);
				i += r;
				if (r)
					std::cout << "Clean byte(s) " << r << std::endl;
			} while (r > 0);
			std::cout << "Send garbarge bytes..." << std::endl;
			readAndPrintStatus(fd, ADDR, false);
		}
		std::cout << "Total cleaned byte(s) " << i << std::endl;
		close(fd);
		return;
	}

	readAndPrintStatus(fd);
	switch (action)
	{
		case 0:
			{
				startAccessPoint(fd);
				SimplePacket sf = readSimplePacket(fd);
				std::cout << sf << std::endl;
				break;
			}
		case 1:
			{
				stopAccessPoint(fd);
				SimplePacket sf = readSimplePacket(fd);
				std::cout << sf << std::endl;
				break;
			}
		case 2:
			{
				resetHW(fd);
				break;
			}
		case 3:
			{
				readAndPrintStatus(fd);
				break;
			}
		case 4:
			{
				startAccessPointSyncMode(fd);
				SimplePacket sf = readSimplePacket(fd);
				std::cout << sf << std::endl;
				break;
			}
		case 5:
			{
				for (;;)
				{
					accDataRequest(fd);
					AccelPacket af = readAccPacket(fd);
					if (!af.isEmpty())
						std::cout << af << std::endl;
				}
				break;
			}
		case 6:
			{
				sendData(fd, data);
				if (data == "3203")
				{
					StatusPacket sf = readStatusPacket(fd);
					std::cout << "sent response for data (status) " << data << " : " << sf << std::endl;
				}
				else if (data == "3303")
				{
					SimplePacket sf = readSimplePacket(fd);
					std::cout << "sent response for data " << data << " : " << sf << std::endl;
					int i = 0;
					int r;
					do
					{
						char c;
						r = read(fd, &c, 1);
						if (r > 0)
							std::cout << i++ << " value: " << (int)c << std::endl;
					} while (r > 0 && i <= 18);
				}
				else
				{
					SimplePacket sf = readSimplePacket(fd);
					std::cout << "sent response for data " << data << " : " << sf << std::endl;
				}
				break;
			}
		case 7:
			break;
		case 8:
			{
				std::cout << "Read SYNC data request" << std::endl;
				sendData(fd, "3303");
				std::cout << "Read SYNC data packet" << std::endl;
				SyncPacket sf = readSyncPacket(fd);
				std::cout << "response: " << sf << std::endl;
				break;
			}
		case 9:
			{
				std::cout << "Send SYNC fill data request" << std::endl;
				sendData(fd, "310402");
				
				StatusPacket sp = readStatusPacket(fd);
				std::cout << "ack: " << sp << std::endl;
				break;
			}
	}
	readAndPrintStatus(fd);
	close(fd);
}

int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: " << argv[0] << " /dev/ttyS0 [stop|reset|status|sync|acc|clean|rsync] [send DATA]" << std::endl;
		return -1;
	}
	int action = 0;
	if (argc >= 3)
	{
		static std::string cmds[] = { "start", "stop", "reset", "status", "sync", "acc", "send", "clean", "rsync", "fsync" };
		std::string an = argv[2];

		action = -1;
		for (unsigned i = 0; i < sizeof(cmds) / sizeof(*cmds); ++i)
		{
			if (cmds[i] == an)
			{
				action = i;
				break;
			}
		}
		if (action == -1)
		{
			std::cerr << "invalid cmd" << std::endl;
			return -1;
		}
	}
	std::string data;
	if (argc == 4)
		data = argv[3];
	doTest(argv[1], action, data);
	return 0;
}
