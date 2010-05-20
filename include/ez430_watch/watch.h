/*
 * include/watch.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef WATCH_H
#define WATCH_H

#include <ez430_watch/service.h>
#include <vector>
#include <string>
#include <ostream>

namespace ez430 {

struct Date
{
	int year;
	int month;
	int day;
	
	friend std::ostream& operator<<(std::ostream& os, const Date&);
};

struct Time
{
	int hour;
	int minute;
	int second;
	
	friend std::ostream& operator<<(std::ostream& os, const Time&);
};

struct Motion
{
	enum Button { NONE, STAR, NUM, UP };

	Button button;

	bool   haveAcc;
	int    x;
	int    y;
	int    z;

	friend std::ostream& operator<<(std::ostream& os, const Motion& motion);
};

class Watch
{
	public:
		enum Unit { METRIC, AMERICAN };

		~Watch();
		Watch(protocol::Service& service);

		float getSmooth() const;
		void setSmooth(float value);
		
		Motion         getMotion();
		Motion::Button getButton();

		Time     getTime();
		bool     setTime(Time);
		
		Date     getDate();
		bool     setDate(Date);

		Time     getAlarm();
		bool     setAlarm(Time);

		bool     setSystemDateAndTime();

		float    getTemperature();
		bool     setTemperature(float);

		int      getAltitude();
		bool     setAltitude(float);

		Unit     getUnitSystem();
		bool     setUnitSystem(Unit);

		bool     exitWatchSyncMode();

		void     setTimeout();
	
	private:
		class Implementation *_impl;
};

} // namespace watch

#endif /* WATCH_H */
