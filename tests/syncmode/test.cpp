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
		else if (arg == "time")
		{
			std::cout << "Time: " << watch.getTime() << std::endl;
		}
		else if (arg == "date")
		{
			std::cout << "Date: " << watch.getDate() << std::endl;
		}
	}
	std::cout << "bye..." << std::endl;
	return 0;
}
