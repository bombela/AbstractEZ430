/*
 * lib/linux/probeports.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include "../probeports.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

bool checkId(path file, unsigned int id)
{
	std::ifstream is(file.string().c_str());
	unsigned int value;
	is >> std::hex >> value;
	return value == id;
}

std::vector<std::string> probePorts()
{
	std::vector<std::string> r;

	path devices = "/sys/bus/usb/devices";
	directory_iterator dEnd;
	for (directory_iterator device(devices); device != dEnd; ++device)
	{
		path vend = path(*device) / "idVendor";
		path id = path(*device) / "idProduct";

		if (checkId(vend, RFEZ430_VENDOR) && checkId(id, RFEZ430_PRODUCT))
		{
			for (directory_iterator interface(*device); interface != dEnd; ++interface)
			{
				if (!is_directory(*interface))
					continue;
				path tty = path(*interface) / "tty";
				if (is_directory(tty))
					for (directory_iterator dev(tty); dev != dEnd; ++dev)
						r.push_back(std::string("/dev/") + path(*dev).leaf());
			}
		}
	}
	return r;
}
