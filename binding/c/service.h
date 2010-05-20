/*
 * service.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef SERVICE_H
#define SERVICE_H

namespace ez430 {
namespace protocol {

struct SyncData
{
	bool  useMetric;
	int   hour;
	int   minute;
	int   second;
	int   year;
	int   month;
	int   day;
	int   alarmHour;
	int   alarmMinute;
	float temperature;
	int   altitude;
};

struct MotionData
{
	enum Button { NONE, STAR, NUM, UP };

	Button button;

	bool   haveAcc;
	int    x;
	int    y;
	int    z;
};

class Service
{
	public:
		virtual ~Service() {};

		virtual bool     getMotion(MotionData& d)       = 0;
		
		virtual bool     requestSyncData()              = 0;
		virtual bool     getSyncData(SyncData& d)       = 0;
		virtual bool     setSyncData(const SyncData& d) = 0;
		virtual bool     exitWatchSyncMode()            = 0;
};

} // namespace protocol
} // namespace ez430

#endif /* SERVICE_H */
