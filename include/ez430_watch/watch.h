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

namespace ez430 {

struct Date
{
	int year;
	int month;
	int day;
};

struct Time
{
	int hour;
	int minute;
	int second;
};

struct Motion
{
	enum Button { NONE, STAR, NUM, UP };

	Button button;

	bool   haveAcc;
	int    x;
	int    y;
	int    z;
};

class Watch
{
	public:
		enum Unit { METRIC, AMERICAN };

		~Watch();
		Watch(protocol::Service& service);

		Motion         getMotion();
		Motion::Button getButton();

		Time     getTime();
		bool     setTime(Time);
		
		Date     getDate();
		bool     setDate(Date);

		Time     getAlarm();
		Time     setAlarm(Time);

		bool     setSystemDateAndTime();

		float    getTemperature();
		bool     setTemperature(float offset);

		int      getAltitude();
		bool     setAltitude(float offset);

		Unit     getUnitSystem();
		bool     setUnitSystem(Unit);
		
		static std::vector<std::string> probePorts();
	
	private:
		class Implementation *_impl;
};

} // namespace watch

#endif /* WATCH_H */
