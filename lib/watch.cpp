/*
 * lib/watch.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/watch.h>
#include <stdexcept>
#include <boost/thread.hpp>
#include <locale>
#include <ctime>
#include <cstring>

namespace ez430 {

class Implementation
{
	public:
		Implementation(protocol::Service& service):
			_service(service),
			_lastButton(Motion::NONE),
			_smooth(0),
			_lastX(0),
			_lastY(0),
			_lastZ(0)
		{
		}
		
		Motion         getMotion()
		{
			protocol::MotionData md;
			int retry = 50;
			while (--retry && !_service.getMotion(md))
				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			if (retry == 0)
				throw std::runtime_error("motion data retrieving timeout excessed");
			
			if (!md.haveAcc || _smooth == 0)
				return reinterpret_cast<Motion&>(md);

			Motion motion;
			motion.button = static_cast<Motion::Button>(md.button);
			motion.haveAcc = md.haveAcc;

			const float newSmooth = 1.f - _smooth;

			motion.x = _lastX = md.x * newSmooth + _lastX * _smooth;
			motion.y = _lastY = md.y * newSmooth + _lastY * _smooth;
			motion.z = _lastZ = md.z * newSmooth + _lastZ * _smooth;
			return motion;
		}

		Motion::Button getButton()
		{
			protocol::MotionData md;
			if (_service.getMotion(md))
				return static_cast<Motion::Button>(md.button);
			return Motion::NONE;
		}

		float getSmooth() const { return _smooth; }
		void  setSmooth(float value)
		{
			if (value < 0)
				_smooth = 0;
			else if (value > 1)
				_smooth = 1;
			else
				_smooth = value;
		}

		Time     getTime()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			Time t;
			t.hour = sd.hour;
			t.minute = sd.minute;
			t.second = sd.second;
			return t;
		}
		bool     setTime(Time t)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.hour = t.hour;
			sd.minute = t.minute;
			sd.second = t.second;
			return _service.setSyncData(sd);
		}
		
		Date     getDate()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			Date d;
			d.year = sd.year;
			d.month = sd.month;
			d.day = sd.day;
			return d;
		}

		bool     setDate(Date d)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.year = d.year;
			sd.month = d.month;
			sd.day = d.day;
			return _service.setSyncData(sd);
		}

		Time     getAlarm()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			Time a;
			a.hour = sd.alarmHour;
			a.minute = sd.alarmMinute;
			a.second = 0;
			return a;
		}
		bool     setAlarm(Time t)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.alarmHour = t.hour;
			sd.alarmMinute = t.minute;
			return _service.setSyncData(sd);
		}

		bool     setSystemDateAndTime()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);
			time_t rawtime;
			struct tm *t = 0;

			time(&rawtime);
			t = localtime(&rawtime);
			sd.hour = t->tm_hour;
			sd.minute = t->tm_min;
			sd.second = t->tm_sec;
			sd.year = t->tm_year;
			sd.month = t->tm_mon;
			sd.day = t->tm_mday;
			return _service.setSyncData(sd);
		}

		float    getTemperature()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			return sd.temperature;
		}

		bool     setTemperature(float offset)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.temperature = offset;
			return _service.setSyncData(sd);			
		}

		int      getAltitude()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			return sd.altitude;
		}
		bool     setAltitude(float offset)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.altitude = offset;
			return _service.setSyncData(sd);			
		}

		Watch::Unit getUnitSystem()
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			return (sd.useMetric == 0) ? Watch::METRIC : Watch::AMERICAN;
		}

		bool     setUnitSystem(Watch::Unit u)
		{
			protocol::SyncData sd;
			retrieveSyncData(sd);

			sd.useMetric = (bool)u;
			return _service.setSyncData(sd);			
		}

		bool     exitWatchSyncMode() { return _service.exitWatchSyncMode(); }

	private:
		protocol::Service& _service;
		Motion::Button     _lastButton;
		float              _smooth;
		int                _lastX;
		int                _lastY;
		int                _lastZ;

		void retrieveSyncData(protocol::SyncData& d)
		{
			_service.requestSyncData();
			int retry = 50;
			while (--retry && !_service.getSyncData(d))
				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			if (retry == 0)
				throw std::runtime_error("syncdata retrieving timeout excessed");
		}
};

// PIMPL IDIOM

Watch::~Watch() { delete _impl; }
Watch::Watch(protocol::Service& service): _impl(new Implementation(service)) {}
Motion Watch::getMotion() { return _impl->getMotion(); }
Motion::Button Watch::getButton() { return _impl->getButton(); }
float Watch::getSmooth() const { return _impl->getSmooth(); }
void Watch::setSmooth(float value) { _impl->setSmooth(value); }
Time Watch::getTime() { return _impl->getTime(); }
bool Watch::setTime(Time t) { return _impl->setTime(t); }
Date Watch::getDate() { return _impl->getDate(); }
bool Watch::setDate(Date t) { return _impl->setDate(t); }
Time Watch::getAlarm() { return _impl->getAlarm(); }
bool Watch::setAlarm(Time t) { return _impl->setAlarm(t); }
bool Watch::setSystemDateAndTime() {return _impl->setSystemDateAndTime(); }
float Watch::getTemperature() { return _impl->getTemperature(); }
bool Watch::setTemperature(float t) { return _impl->setTemperature(t); }
int Watch::getAltitude() { return _impl->getAltitude(); }
bool Watch::setAltitude(float t) { return _impl->setAltitude(t); }
Watch::Unit Watch::getUnitSystem() { return _impl->getUnitSystem(); }
bool Watch::setUnitSystem(Unit u) { return _impl->setUnitSystem(u); }
bool Watch::exitWatchSyncMode() { return _impl->exitWatchSyncMode(); }

// GLOBAL FUNCS

std::ostream& operator<<(std::ostream& os, const Motion& motion)
{
	std::ostream::sentry init(os);
	if (init)
	{
		os << "button=" << motion.button;
		if (motion.haveAcc)
		{
			os << " x=" << motion.x
				<< " y=" << motion.y
				<< " z=" << motion.z;
		}
		else
		{
			os << " (no accel data)";
		}
	}
	return os;
}

namespace {

inline void printDateTime(std::ostream& os, const std::tm& tm, const char* pattern)
{
	std::locale locale;
	const std::time_put<char>& tmput
		= std::use_facet<std::time_put<char> >(locale);

	tmput.put(os, os, ' ', &tm, pattern, pattern + std::strlen(pattern));
}

} // namespace anonymous

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	std::ostream::sentry init(os);
	if (init)
	{
		std::tm tm;
		tm.tm_year = date.year - 1900;
		tm.tm_mon = date.month;
		tm.tm_mday = date.day;
		printDateTime(os, tm, "%x");
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
	std::ostream::sentry init(os);
	if (init)
	{
		std::tm tm;
		tm.tm_hour = time.hour;
		tm.tm_min = time.minute;
		tm.tm_sec = time.second;
		printDateTime(os, tm, "%X");
	}
	return os;
}

} // namespace ez430
