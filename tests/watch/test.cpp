/*
 * tests/ap/test.cpp
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

static const uint16_t mgrav_per_bit[7] = { 18, 36, 71, 143, 286, 571, 1142 };

float convert_acceleration_value_to_mgrav(int value)
{
	if (value < 0)
		value = -value;
	
	float result = 0;
	for (int i = 0; i < 7; ++i)
		if (value & (1 << i))
			result += mgrav_per_bit[i];
	
	return result / 1000;
}

int main(int argc, char const* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << "/dev/ttyACM0 cmd" << std::endl
		<< "cmd\t" << "description" << std::endl
		<< "---\t" << "-----------" << std::endl
		<< "start\t" << "Starting RF dongle" << std::endl
		<< "stop\t" << "Stoping RF dongle" << std::endl
		<< "acc\t" << "Receive ACC" << std::endl
		<< "ppt\t" << "Receive PPT" << std::endl;
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

	watch.setSmooth(0.80);

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

					float gx = convert_acceleration_value_to_mgrav(motion.x);
					float gy = convert_acceleration_value_to_mgrav(motion.y);
					float gz = convert_acceleration_value_to_mgrav(motion.z);

					static float max_gx = gx;
					static float max_gy = gy;
					static float max_gz = gz;

					if (gx > max_gx)
						max_gx = gx;
					if (gy > max_gy)
						max_gy = gy;
					if (gz > max_gz)
						max_gz = gz;

					std::cout << std::setprecision(2) << motion
						<< " gx=" << gx
						<< " gy=" << gy
						<< " gz=" << gz
						<< " mgx=" << max_gx
						<< " mgy=" << max_gy
						<< " mgz=" << max_gz
						<< std::endl;
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
