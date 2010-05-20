/*
 * ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <iostream>
#include <ez430_watch/ap.h>
#include <boost/asio.hpp>
#include "ap_proto.h"
#include "ap_service.h"

namespace ez430 {

using namespace protocol::ap;
using protocol::buffer;

class Implementation
{
	public:
		~Implementation()
		{
			close();
		}

		Implementation():
			_serialPort(_ioService),
			_apService(_serialPort)
		{
		}

		Implementation(const std::string& line):
			_serialPort(_ioService),
			_apService(_serialPort)
		{
			open(line);
		}

		void open(const std::string& line)
		{
			if (isOpen())
				close();
			_serialPort.open(line);
			retrieveProductId();

			// Clear AccessPoint motion buffer.
			{ protocol::MotionData m; _apService.getMotion(m); }

			// Clear Sync motion buffer.
			{ protocol::SyncData d; _apService.getSyncData(d); }
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
			boost::asio::write(_serialPort, buffer(packet));
			boost::asio::read(_serialPort, buffer(packet));
		
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
		
		void startRadio()
		{
			packet::Base packet
				= createPacket<packet::Base>(packet::BM_START_SIMPLICITI);
			boost::asio::write(_serialPort, buffer(packet));
			boost::asio::read(_serialPort, buffer(packet));
		}

		void stopRadio()
		{
			packet::Base packet
				= createPacket<packet::Base>(packet::BM_STOP_SIMPLICITI);
			boost::asio::write(_serialPort, buffer(packet));
			boost::asio::read(_serialPort, buffer(packet));
		}

		protocol::Service& getService()
		{
			return _apService;
		}

	private:
		boost::asio::io_service  _ioService;
		boost::asio::serial_port _serialPort;
		int                      _productId;
		protocol::ApService      _apService;

		void retrieveProductId()
		{
			packet::ProductId packet
				= createPacket<packet::ProductId>(packet::BM_GET_PRODUCT_ID);
			boost::asio::write(_serialPort, buffer(packet));
			boost::asio::read(_serialPort, buffer(packet));
			_productId = packet.productId;
		}
};

// PIMPL IDIOM
AccessPoint::~AccessPoint() { delete _impl; }
AccessPoint::AccessPoint(): _impl(new Implementation) {} 
AccessPoint::AccessPoint(const std::string& line): _impl(new Implementation(line)) {} 
void AccessPoint::open(const std::string& line) { _impl->open(line); }
bool AccessPoint::isOpen() const { return _impl->isOpen(); }
void AccessPoint::close() { _impl->close(); }
int AccessPoint::getProductId() const { return _impl->getProductId(); }
AccessPoint::RadioState AccessPoint::getRadioState() { return _impl->getRadioState(); }
void AccessPoint::startRadio() { _impl->startRadio(); }
void AccessPoint::stopRadio() { _impl->stopRadio(); }
protocol::Service& AccessPoint::getService() { return _impl->getService(); }

} // namespace ez430
