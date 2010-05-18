/*
 * tests/ap/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>
#include <ez430_watch/watch.h>

#include <iostream>
#include <stdexcept>

using namespace ez430;

int main(int argc, char const* argv[])
{
	if (argc < 2)
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

	std::cout << std::hex << "product id: " << ap.getProductId() << std::dec << std::endl;
	std::cout << "current radio state: " << ap.getRadioState() << std::endl;

	Watch watch(ap.getService());

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
		else if (arg == "acc")
		{
			std::cout << "Retrieving acc..." << std::endl;
			for (;;)
			{
				try
				{
					Motion motion = watch.getMotion();
					std::cout << motion << std::endl;
				}
				catch (const std::runtime_error&)
				{
					std::cout << "timeout" << std::endl;
				}
			}
		}
		else if (arg == "ppt")
		{
			std::cout << "Retrieving ppt..." << std::endl;
			for (;;)
			{
				try
				{
					Motion::Button button = watch.getButton();
					std::cout << "Button: " << button << std::endl;
				}
				catch (const std::runtime_error&)
				{
					std::cout << "timeout" << std::endl;
				}
			}
		}
	}
	std::cout << "bye..." << std::endl;
	return 0;
}
