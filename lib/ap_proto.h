/*
 * protocol.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <ez430_watch/packed.h>
#include <ostream>

namespace ez430 {
namespace protocol {
namespace ap {
namespace packet {

enum {
	MAGIC    = 0xFF,
	MIN_SIZE = 3,
	MAX_SIZE = 32
};

enum {
	BM_GET_STATUS             = 0x00,
	BM_RESET                  = 0x01,
	BM_GET_PRODUCT_ID         = 0x20,

	BM_START_SIMPLICITI       = 0x07,
	BM_GET_SIMPLICITIDATA     = 0x08,
	BM_STOP_SIMPLICITI        = 0x09,
	
	BM_SYNC_SEND_COMMAND      = 0x31,
	BM_SYNC_GET_BUFFER_STATUS = 0x32,
	BM_SYNC_READ_BUFFER	      = 0x33
};

struct PACKED Base
{
	uint8_t magic;
	uint8_t cmd;
	uint8_t size;

	bool isValid() { return magic == MAGIC; }

	void print(std::ostream& os) const
	{
		os << std::hex
			<< "magic=" << (unsigned int)magic
			<< " cmd=" << (unsigned int)cmd
			<< " size=" << (unsigned int)size
			<< std::dec;
	}
};

struct PACKED Status: Base
{
	enum {
		HW_IDLE                      = 0x00,
		HW_SIMPLICITI_STOPPED        = 0x01,
		HW_SIMPLICITI_TRYING_TO_LINK = 0x02,
		HW_SIMPLICITI_LINKED         = 0x03,
		HW_BLUEROBIN_STOPPED         = 0x04,
		HW_BLUEROBIN_TRANSMITTING    = 0x05,
		HW_ERROR                     = 0x05,
		HW_NO_ERROR                  = 0x06,
		HW_NOT_CONNECTED             = 0x07,
		HW_SIMPLICITI_LINK_TIMEOUT   = 0x08,
		HW_WBSL_TRYING_TO_LINK       = 0x09,
		HW_WBSL_LINKED               = 0x0A,
		HW_WBSL_ERROR                = 0x0B,
		HW_WBSL_STOPPED              = 0x0C,
		HW_WBSL_LINK_TIMEOUT         = 0x0D
	};

	uint8_t status;
};

struct PACKED Motion: Base
{
	enum {
		SIMPLICITI_BUTTON_STAR  = 0x10,
		SIMPLICITI_BUTTON_NUM   = 0x20,
		SIMPLICITI_BUTTON_UP    = 0x30
	};

	enum {
		SIMPLICITI_MOUSE_EVENTS = 0x01,
		SIMPLICITI_KEY_EVENTS   = 0x02,
	};

	enum {
		EMPTY_STATUS = 0xFF
	};

	union
	{
		struct
		{
			uint8_t mode:4;
			uint8_t button:4;
		};
		uint8_t status;
	};

	int8_t x;
	int8_t y;
	int8_t z;
	
	bool isEmpty() { return status == EMPTY_STATUS; }
};

struct PACKED SyncStatus: Base
{
	enum {
		SYNC_BUFFER_EMPTY = 0,
		SYNC_BUFFER_READY = 1
	};

	uint8_t syncStatus;
};

struct PACKED ProductId: Base
{
	uint32_t productId;
	
	void print(std::ostream& os) const
	{
		Base::print(os);
		os << std::hex << " productId=" << productId << std::dec;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const T& packet)
{
	std::ostream::sentry init(os);
	if (init)
	{
		packet.print(os);
	}
	return os;
}

} // namespace packet

template <typename T>
T createPacket(uint8_t cmd)
{
	T packet;

	packet.magic = packet::MAGIC;
	packet.cmd = cmd;
	packet.size = sizeof(packet);
	return packet;
}

} // namespace ap
} // namespace protocol
} // namespace ez430

#endif /* PROTOCOL_H */
