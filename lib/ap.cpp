/*
 * ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/ap.h>
#include <boost/asio.hpp>

namespace ez430 {

class Implementation
{
	public:
		~Implementation()
		{
			close();
		}

		Implementation():
			_serialPort(_ioService)
		{
		}

		Implementation(const std::string& line):
			_serialPort(_ioService)
		{
			open(line);
		}

		void open(const std::string& line)
		{
			if (isOpen())
				close();
			_serialPort.open(line);
		}

		bool isOpen() const
		{
			return _serialPort.is_open();
		}

		void close()
		{
			_serialPort.close();
		}

	private:
		boost::asio::io_service  _ioService;
		boost::asio::serial_port _serialPort;
};

AccessPoint::~AccessPoint()
{
	delete _impl;
}

AccessPoint::AccessPoint():
	_impl(new Implementation) {}

AccessPoint::AccessPoint(const std::string& line):
	_impl(new Implementation(line)) {}

void AccessPoint::open(const std::string& line) { _impl->open(line); }
bool AccessPoint::isOpen() const { return _impl->isOpen(); }
void AccessPoint::close() { _impl->close(); }

} // namespace ez
