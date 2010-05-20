/*
 * tests/probe/test.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>
#include <iostream>

using namespace ez430;

int main()
{
	const std::vector<std::string>& ports = AccessPoint::probePorts();

	if (ports.size() == 0)
	{
		std::cout << "No ports found :'('" << std::endl;
		return -1;
	}

	AccessPoint ap;

	const std::vector<std::string>::const_iterator& end = ports.end();
	for (std::vector<std::string>::const_iterator i = ports.begin();
			i != end; ++i)
	{
		std::cout << "opening: " << *i << " ..." << std::endl;
		try { ap.open(*i); } catch (...) {}
		if (!ap.isOpen())
		{
			std::cout << "unable to open " << *i << std::endl;
			continue;
		}

		std::cout << std::hex << "product id: " << ap.getProductId() << std::dec << std::endl;
		std::cout << "current radio state: " << ap.getRadioState() << std::endl;
	}

	std::cout << "bye..." << std::endl;
	return 0;
}
