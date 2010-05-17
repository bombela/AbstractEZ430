/*
 * ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <iostream>
#include <ez430_watch/ap.h>
#include <boost/asio.hpp>
#include "ap_proto.h"

namespace ez430 {

using namespace protocol::ap;

template <typename T>
boost::asio::const_buffers_1 buffer(const T& packet)
{
	return boost::asio::buffer(&packet, sizeof(packet));
}

template <typename T>
boost::asio::mutable_buffers_1 buffer(T& packet)
{
	return boost::asio::buffer(&packet, sizeof(packet));
}

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
			retrieveProductId();
		}

		bool isOpen() const
		{
			return _serialPort.is_open();
		}

		void close()
		{
			_serialPort.close();
		}
		
		int getProductId() const
		{
			return _productId;
		}

		AccessPoint::RadioState getRadioState()
		{
			packet::Status packet
				= createPacket<packet::Status>(packet::BM_GET_STATUS);
			write(_serialPort, buffer(packet));
			read(_serialPort, buffer(packet));
		
			switch (packet.status)
			{
				case packet::Status::HW_SIMPLICITI_TRYING_TO_LINK:
					return AccessPoint::LINKING;

				case packet::Status::HW_SIMPLICITI_LINKED:
					return AccessPoint::LINKED;
				
				case packet::Status::HW_IDLE:
				case packet::Status::HW_SIMPLICITI_STOPPED:
				default:
						return AccessPoint::STOPPED;
			}

		}

	private:
		boost::asio::io_service  _ioService;
		boost::asio::serial_port _serialPort;
		int                      _productId;

		void retrieveProductId()
		{
			packet::ProductId packet
				= createPacket<packet::ProductId>(packet::BM_GET_PRODUCT_ID);
			write(_serialPort, buffer(packet));
			read(_serialPort, buffer(packet));
			_productId = packet.productId;
		}
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
int AccessPoint::getProductId() const { return _impl->getProductId(); }
AccessPoint::RadioState AccessPoint::getRadioState() { return _impl->getRadioState(); }

} // namespace ez
