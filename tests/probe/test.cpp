/*
 * tests/probe/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>
#include <ez430_watch/watch.h>
#include <iostream>

using namespace ez430;

int main()
{
	const std::vector<std::string>& ports = Watch::probePorts();

	if (ports.size() == 0)
	{
		std::cout << "No ports found :'('" << std::endl;
		return -1;
	}

	const std::vector<std::string>::const_iterator& end = ports.end();
	for (std::vector<std::string>::const_iterator i = ports.begin();
			i != end; ++i)
	{
		std::cout << *i << std::endl;
	}

	/*
	AccessPoint ap;

	ap.open("/dev/ttyACM0");

	if (!ap.isOpen())
	{
		std::cout << "not open !" << std::endl;
		return -1;
	}

	std::cout << std::hex << "product id: " << ap.getProductId() << std::dec << std::endl;
	std::cout << "current radio state: " << ap.getRadioState() << std::endl;

	std::cout << "bye..." << std::endl;*/
	return 0;
}
