/*
 * lib/watch.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <ez430_watch/watch.h>
#include <stdexcept>
#include <boost/thread.hpp>

namespace ez430 {

class Implementation
{
	public:
		Implementation(protocol::Service& service):
			_service(service),
			_lastButton(Motion::NONE)
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
			return reinterpret_cast<Motion&>(md);
		}

		Motion::Button getButton()
		{
			protocol::MotionData md;
			if (_service.getMotion(md))
				return static_cast<Motion::Button>(md.button);
			return Motion::NONE;
		}

	private:
		protocol::Service& _service;
		Motion::Button     _lastButton;
};

// PIMPL IDIOM

Watch::~Watch() { delete _impl; }
Watch::Watch(protocol::Service& service): _impl(new Implementation(service)) {}
Motion Watch::getMotion() { return _impl->getMotion(); }
Motion::Button Watch::getButton() { return _impl->getButton(); }

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

} // namespace ez430
