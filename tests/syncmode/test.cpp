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
		std::cout << "Usage: " << "/dev/ttyACM0 cmd" << std::endl
		<< "cmd\t" << "description" << std::endl
		<< "---\t" << "-----------" << std::endl
		<< "td\t" << "Get Date and Time from the watch" << std::endl
		<< "st\t" << "Set Time at 4:42:42 on the watch" << std::endl
		<< "a\t" << "Get Alarm from the watch" << std::endl
		<< "ta\t" << "Get Temperature and Altitude from the watch" << std::endl
		<< "us\t" << "Get UnitSystem from the watch" << std::endl
		<< "sus\t" << "Set UnitSystem to American on the watch" << std::endl
		<< "sm\t" << "Set UnitSystem to Metric on the watch" << std::endl
		<< "ssdt\t" << "Set System Date And Time on the watch" << std::endl;
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
			t.hour = 4;
			t.minute = 42;
			t.second = 42;
			std::cout << "Setting time: " << t << std::endl;
			watch.setTime(t);
			std::cout << "Time: " << watch.getTime() << std::endl;
		}
		else if (arg == "a")
		{
			std::cout << "Alarm: " << watch.getAlarm() << std::endl;
		}
		else if (arg == "ta")
		{
			std::cout << "Temp: " << watch.getTemperature() << std::endl;
			std::cout << "Alt: " << watch.getAltitude() << std::endl;
		}
		else if (arg == "sa")
		{
			Time t;
			t.hour = 4;
			t.minute = 42;
			t.second = 0;
			std::cout << "Setting AlarmTime: " << t << std::endl;
			watch.setAlarm(t);
			std::cout << "Alarm: " << watch.getAlarm() << std::endl;
		}
		else if (arg == "ta")
		{
			std::cout << "Temp: " << watch.getTemperature() << std::endl;
			std::cout << "Alt: " << watch.getAltitude() << std::endl;
		}
		else if (arg == "us")
		{
			std::cout << "UnitSystem: " << watch.getUnitSystem() << std::endl;
		}
		else if (arg == "sus")
		{
			std::cout << "Set American unit" << std::endl;
			watch.setUnitSystem(Watch::AMERICAN);
			std::cout << "UnitSystem: " << std::boolalpha
				<< watch.getUnitSystem() << std::endl;
		}
		else if (arg == "sm")
		{
			std::cout << "Set Metric unit" << std::endl;
			watch.setUnitSystem(Watch::METRIC);
			std::cout << "UnitSystem: " << std::boolalpha
				<< watch.getUnitSystem() << std::endl;
		}
		else if (arg == "ssdt")
		{
			watch.setSystemDateAndTime();
			std::cout << "Date: " << watch.getDate() << std::endl;
			std::cout << "Time: " << watch.getTime() << std::endl;
		}
	}
	std::cout << "bye..." << std::endl;
	return 0;
}
