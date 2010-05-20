/*
 * lib/ap_service.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef AP_SERVICE_H
#define AP_SERVICE_H

#include <ez430_watch/service.h>
#include <boost/asio.hpp>

namespace ez430 {
namespace protocol {

class ApService: public Service
{
	public:
		ApService(boost::asio::serial_port& sp);

		virtual bool     getMotion(MotionData& d);
		
		virtual bool     requestSyncData();
		virtual bool     getSyncData(SyncData& d);
		virtual bool     setSyncData(const SyncData& d);
		virtual bool     exitWatchSyncMode();

	private:
		boost::asio::serial_port& _serialPort;

		bool syncBufferIsReady();
};

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

} // namespace protocol
} // namespace ez430

#endif /* AP_SERVICE_H */
