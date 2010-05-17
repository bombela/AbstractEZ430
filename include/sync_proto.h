/*
 * sync_proto.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef SYNC_PROTO_H
#define SYNC_PROTO_H

namespace ez430 {
namespace protocol {
namespace packet {
namespace sync {

enum {
	SYNC_AP_CMD_NOP                      = 1u,
	SYNC_AP_CMD_GET_STATUS				 = 2u,
	SYNC_AP_CMD_SET_WATCH                = 3u,
	SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_1 = 4u,
	SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_2 = 5u,
	SYNC_AP_CMD_ERASE_MEMORY             = 6u,
	SYNC_AP_CMD_EXIT                     = 7u
};

struct PACKED(Sync): Base
{
	union
	{
		uint8_t syncCmd; // request
		uint8_t status;  // response
	};
	uint8_t  useMetric:1;
	uint8_t  hour:7;
	uint8_t  minute;
	uint8_t  second;
	uint16_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  alarmHour;
	uint8_t  alarmMinute;
	uint16_t temperature; // *10
	uint16_t altitude;
	uint8_t  padding[5];
};

} // namespace sync
} // namespace packet
} // namespace protocol
} // namespace ez430

#endif /* SYNC_PROTO_H */
