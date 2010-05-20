/*
 * lib/ap_service.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include "ap_service.h"
#include "ap_proto.h"
#include "sync_proto.h"
#include <cstring>
#include <boost/thread.hpp>

#include <iostream> // TODO REMOVE ME
#include <iomanip> // TODO REMOVE ME

namespace ez430 {
namespace protocol {

namespace sync {

sync::packet::Sync createPacket(uint8_t cmd, uint8_t syncCmd)
{
	sync::packet::Sync packet = ap::createPacket<sync::packet::Sync>(cmd);
	packet.syncCmd = syncCmd;
	std::memset(packet.padding, 0, sizeof(packet.padding));
	return packet;
}

} // namespace sync

ApService::ApService(boost::asio::serial_port& sp):
	_serialPort(sp)
{
}

bool ApService::getMotion(MotionData& d)
{
	ap::packet::Motion packet =
		ap::createPacket<ap::packet::Motion>(ap::packet::BM_GET_SIMPLICITIDATA);
	boost::asio::write(_serialPort, buffer(packet));
	boost::asio::read(_serialPort, buffer(packet));
	
	if (packet.isEmpty())
		return false;

	switch (packet.button)
	{
		case ap::packet::Motion::SIMPLICITI_BUTTON_STAR:
			d.button = MotionData::STAR;
			break;

		case ap::packet::Motion::SIMPLICITI_BUTTON_NUM:
			d.button = MotionData::NUM;
			break;

		case ap::packet::Motion::SIMPLICITI_BUTTON_UP:
			d.button = MotionData::UP;
			break;

		default:
			d.button = MotionData::NONE;
	}

	d.haveAcc = (packet.mode == ap::packet::Motion::SIMPLICITI_MOUSE_EVENTS);
	if (d.haveAcc)
	{
		d.x = packet.x;
		d.y = packet.y;
		d.z = packet.z;
	}

	return true;
}

bool ApService::requestSyncData()
{
	sync::packet::Sync packet =
		sync::createPacket(ap::packet::BM_SYNC_SEND_COMMAND,
				sync::packet::SYNC_AP_CMD_GET_STATUS);
	boost::asio::write(_serialPort, buffer(packet));
	boost::asio::read(_serialPort, buffer(packet));
	return true;
}

bool ApService::getSyncData(SyncData& d)
{
	if (!syncBufferIsReady())
		return false;
	
	sync::packet::Sync packet =
		sync::createPacket(ap::packet::BM_SYNC_READ_BUFFER,
				sync::packet::SYNC_AP_CMD_GET_STATUS);
	boost::asio::write(_serialPort, buffer(packet));
	boost::asio::read(_serialPort, buffer(packet));

	d.useMetric = packet.useMetric;
	d.hour = packet.hour;
	d.minute = packet.minute;
	d.second = packet.second;
	d.year = packet.year;
	d.month = packet.month;
	d.day = packet.day;
	d.alarmHour = packet.alarmHour;
	d.alarmMinute = packet.alarmMinute;
	d.temperature = packet.temperature / 10.f;
	d.altitude = packet.altitude;
	return true;
}

bool ApService::setSyncData(const SyncData& d)
{
	sync::packet::Sync packet =
		sync::createPacket(ap::packet::BM_SYNC_SEND_COMMAND,
				sync::packet::SYNC_AP_CMD_SET_WATCH);

	packet.useMetric = d.useMetric;
	packet.hour = d.hour;
	packet.minute = d.minute;
	packet.second = d.second;
	packet.year = d.year;
	packet.month = d.month;
	packet.day = d.day;
	packet.alarmHour = d.alarmHour;
	packet.alarmMinute = d.alarmMinute;
	packet.temperature = d.temperature * 10.f;
	packet.altitude = d.altitude;

	boost::asio::write(_serialPort, buffer(packet));
	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	boost::asio::read(_serialPort, buffer(packet));
	return packet.status == 3 && packet.cmd == ap::packet::BM_ACK;
}

bool ApService::exitWatchSyncMode()
{
	sync::packet::Sync packet =
		sync::createPacket(ap::packet::BM_SYNC_SEND_COMMAND,
				sync::packet::SYNC_AP_CMD_EXIT);
	boost::asio::write(_serialPort, buffer(packet));
	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	boost::asio::read(_serialPort, buffer(packet));
	return packet.status;
}

bool ApService::syncBufferIsReady()
{
	ap::packet::Status packet =
		ap::createPacket<ap::packet::Status>(ap::packet::BM_SYNC_GET_BUFFER_STATUS);
	boost::asio::write(_serialPort, buffer(packet));
	boost::asio::read(_serialPort, buffer(packet));
	return packet.status;
}

} // namespace protocol
} // namespace ez430
