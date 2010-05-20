/*
 * lib/ap_service.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include "ap_service.h"
#include "ap_proto.h"
#include "sync_proto.h"

#include <iostream> // TODO REMOVE ME

namespace ez430 {
namespace protocol {

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
		sync::createPacket<sync::packet::Sync>(ap::packet::BM_SYNC_SEND_COMMAND,
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
		sync::createPacket<sync::packet::Sync>(ap::packet::BM_SYNC_READ_BUFFER,
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

bool ApService::setSyncData(const SyncData&)
{
	throw "not implemented";
}

bool ApService::exitWatchSyncMode()
{
	sync::packet::Sync packet =
		sync::createPacket<sync::packet::Sync>(ap::packet::BM_SYNC_SEND_COMMAND,
				sync::packet::SYNC_AP_CMD_EXIT);
	boost::asio::write(_serialPort, buffer(packet));
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
