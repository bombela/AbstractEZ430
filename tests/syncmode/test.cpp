/*
 * tests/syncmode/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>
#include <ez430_watch/watch.h>

#include <iostream>
#include <stdexcept>
#include <iomanip>

#include <stdint.h>

using namespace ez430;

int main(int argc, char const* argv[])
{
	if (argc < 3)
	{
		std::cout << "usage: " << "/dev/ttyACM0 cmd" << std::endl;
		return -1;
	}

	AccessPoint ap;

	try
	{
		ap.open(argv[1]);
	}
	catch (...)
	{
	}

	if (!ap.isOpen())
	{
		std::cout << "not open !" << std::endl;
		return -1;
	}

	if (ap.getRadioState() == AccessPoint::STOPPED)
	{
		std::cout << "Starting radio..." << std::endl;
		ap.startRadio();
	}

	Watch watch(ap.getService());

	if (argc > 2)
	{
		std::string arg = argv[2];
		if (arg == "exit")
		{
			std::cout << "Exiting sync mode on the watch..." << std::endl;
			watch.exitWatchSyncMode();
		}
		else if (arg == "td")
		{
			std::cout << "Date: " << watch.getDate() << std::endl;
			std::cout << "Time: " << watch.getTime() << std::endl;
		}
		else if (arg == "st")
		{
			Time t;
			t.hour = 8;
			t.minute = 8;
			t.second = 8;
			std::cout << "Setting time: " << t << std::endl;
			watch.setTime(t);
			std::cout << "Date: " << watch.getDate() << std::endl;
			std::cout << "Time: " << watch.getTime() << std::endl;
		}
		else if (arg == "ga")
		{
			std::cout << "Alarm: " << watch.getAlarm() << std::endl;
		}
		else if (arg == "ta")
		{
			std::cout << "Temp: " << watch.getTemperature() << std::endl;
			std::cout << "Alt: " << watch.getAltitude() << std::endl;
		}
	}
	std::cout << "bye..." << std::endl;
	return 0;
}
