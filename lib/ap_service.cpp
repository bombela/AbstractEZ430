/*
 * lib/ap_service.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include "ap_service.h"
#include "ap_proto.h"

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

bool ApService::getSyncData(SyncData& d)
{
}

bool ApService::setSyncData(const SyncData& d)
{
}

bool ApService::exitWatchSync()
{
}

} // namespace protocol
} // namespace ez430
