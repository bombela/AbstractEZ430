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
		std::cout << "not open !" << std::endl;
		return -1;
	}

	std::cout << std::hex << "product id: " << ap.getProductId() << std::dec << std::endl;
	std::cout << "current radio state: " << ap.getRadioState() << std::endl;
	return 0;
}
