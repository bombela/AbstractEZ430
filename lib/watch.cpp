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
			protocol::SyncData d;
			retrieveSyncData(d);

			Time t;
			t.hour = d.hour;
			t.minute = d.minute;
			t.second = d.second;
			return t;
		}
		bool     setTime(Time)
		{
			throw "not implemented";
		}
		
		Date     getDate()
		{
			throw "not implemented";
		}
		bool     setDate(Date)
		{
			throw "not implemented";
		}

		Time     getAlarm()
		{
			throw "not implemented";
		}
		bool     setAlarm(Time)
		{
			throw "not implemented";
		}

		bool     setSystemDateAndTime()
		{
			throw "not implemented";
		}

		float    getTemperature()
		{
			throw "not implemented";
		}
		bool     setTemperature(float offset)
		{
			throw "not implemented";
		}

		int      getAltitude()
		{
			throw "not implemented";
		}
		bool     setAltitude(float offset)
		{
			throw "not implemented";
		}

		Watch::Unit getUnitSystem()
		{
			throw "not implemented";
		}
		bool     setUnitSystem(Watch::Unit)
		{
			throw "not implemented";
		}

		bool     exitWatchSyncMode() { _service.exitWatchSyncMode(); }

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
float Watch::getTemperature() { return _impl->getTemperature(); }
bool Watch::setTemperature(float t) { _impl->setTemperature(t); }
int Watch::getAltitude() { return _impl->getAltitude(); }
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
	const std::locale locale;
	const std::time_put<char>& tmput
		= std::use_facet<std::time_put<char> >(locale);

	tmput.put(os, os, ' ', &tm, pattern, pattern + std::strlen(pattern) - 1);
}

} // namespace anonymous

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	std::ostream::sentry init(os);
	if (init)
	{
		std::tm tm;
		tm.tm_year = date.year;
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
