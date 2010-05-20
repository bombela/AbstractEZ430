/*
 * tests/ap/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>

#include <iostream>

using namespace ez430;

int main(int argc, char const* argv[])
{
	AccessPoint ap;

	try
	{
		if (argc > 1)
			ap.open(argv[1]);
		else
			ap.open("/dev/ttyACM0");
	}
	catch (...)
	{
	}

	if (!ap.isOpen())
	{
		std::cout << "AP not open !" << std::endl;
		std::cout << "Usage: " << "/dev/ttyACM0 cmd" << std::endl
		<< "cmd\t" << "description" << std::endl
		<< "---\t" << "-----------" << std::endl
		<< "start\t" << "Starting RF dongle" << std::endl
		<< "stop\t" << "Stoping RF dongle" << std::endl
		return -1;
	}

	std::cout << std::hex << "product id: " << ap.getProductId() << std::dec << std::endl;
	std::cout << "current radio state: " << ap.getRadioState() << std::endl;

	if (argc > 2)
	{
		std::string arg = argv[2];
		if (arg == "start")
		{
			std::cout << "Starting radio..." << std::endl;
			ap.startRadio();
		}
		else if (arg == "stop")
		{
			std::cout << "Stopping radio..." << std::endl;
			ap.stopRadio();
		}
	}
	else
	{
		std::cout << "Usage: " << "/dev/ttyACM0 cmd" << std::endl
		<< "cmd\t" << "description" << std::endl
		<< "---\t" << "-----------" << std::endl
		<< "start\t" << "Starting RF dongle" << std::endl
		<< "stop\t" << "Stoping RF dongle" << std::endl
	}
	std::cout << "bye..." << std::endl;
	return 0;
}
