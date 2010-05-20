/*
 * binding/python/ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <sstream>

#include <boost/python.hpp>

#include <ez430_watch/ap.h>
#include <ez430_watch/service.h>
#include <ez430_watch/watch.h>

using namespace boost::python;
using namespace ez430;
using namespace ez430::protocol;

struct ServiceWrap: Service, wrapper<Service>
{
	virtual bool getMotion(MotionData& d) { return get_override("getMotion")(d); }
	virtual bool requestSyncData() { return get_override("requestSyncData"); }
	virtual bool getSyncData(SyncData& d) { return get_override("getSyncData")(d); }
	virtual bool setSyncData(const SyncData& d) { return get_override("setSyncData")(d); }
	virtual bool exitWatchSyncMode() { return get_override("exitWatchSyncMode"); }

	operator Service&() { return *this; }
};

BOOST_PYTHON_MODULE(_ez430)
{
	class_<ServiceWrap, boost::noncopyable>("Service")
		.def("getMotion", &ServiceWrap::getMotion)
		.def("requestSyncData", &ServiceWrap::requestSyncData)
		.def("getSyncData", &ServiceWrap::getSyncData)
		.def("setSyncData", &ServiceWrap::setSyncData)
		.def("exitWatchSyncMode", &ServiceWrap::exitWatchSyncMode)
		;

	enum_<AccessPoint::RadioState>("RadioState")
		.value("STOPPED", AccessPoint::STOPPED)
		.value("LINKING", AccessPoint::LINKING)
		.value("LINKED", AccessPoint::LINKED)
		;

	class_<AccessPoint>("AccessPoint")
		.def(init<const std::string&>())
		.def("open", &AccessPoint::open)
		.def("isOpen", &AccessPoint::isOpen)
		.def("close", &AccessPoint::close)
		.def("getProductId", &AccessPoint::getProductId)
		.def("startRadio", &AccessPoint::startRadio)
		.def("stopRadio", &AccessPoint::stopRadio)
		.def("getRadioState", &AccessPoint::getRadioState)
		.def("getService", &AccessPoint::getService, return_internal_reference<1>())
		;

	enum_<Motion::Button>("Button")
		.value("NONE", Motion::NONE)
		.value("STAR", Motion::STAR)
		.value("NUM", Motion::NUM)
		.value("UP", Motion::UP)
		;

	class_<Motion>("Motion")
		.def_readwrite("button", &Motion::button)
		.def_readwrite("haveAcc", &Motion::haveAcc)
		.def_readwrite("x", &Motion::x)
		.def_readwrite("y", &Motion::y)
		.def_readwrite("z", &Motion::z)
		;

	enum_<Watch::Unit>("Unit")
		.value("METRIC", Watch::METRIC)
		.value("AMERICAN", Watch::AMERICAN)
		;

	class_<Watch>("Watch", init<Service&>())
		.def("getSmooth", &Watch::getSmooth)
		.def("setSmooth", &Watch::setSmooth)
		.def("getMotion", &Watch::getMotion)
		.def("getButton", &Watch::getButton)
		.def("getAlarm", &Watch::getAlarm)
		.def("setAlarm", &Watch::setAlarm)
		.def("setSystemDateAndTime", &Watch::setSystemDateAndTime)
		.def("getTemperature", &Watch::getTemperature)
		.def("setTemperature", &Watch::setTemperature)
		.def("getAltitude", &Watch::getAltitude)
		.def("setAltitude", &Watch::setAltitude)
		.def("getUnitSystem", &Watch::getUnitSystem)
		.def("setUnitSystem", &Watch::setUnitSystem)
		.def("exitWatchSyncMode", &Watch::exitWatchSyncMode)
		;

	def("probePorts", &AccessPoint::probePorts);
}
