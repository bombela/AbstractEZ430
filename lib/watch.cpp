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

	private:
		protocol::Service& _service;
		Motion::Button     _lastButton;
		float              _smooth;
		int                _lastX;
		int                _lastY;
		int                _lastZ;
};

// PIMPL IDIOM

Watch::~Watch() { delete _impl; }
Watch::Watch(protocol::Service& service): _impl(new Implementation(service)) {}
Motion Watch::getMotion() { return _impl->getMotion(); }
Motion::Button Watch::getButton() { return _impl->getButton(); }
float Watch::getSmooth() const { return _impl->getSmooth(); }
void Watch::setSmooth(float value) { _impl->setSmooth(value); }

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
