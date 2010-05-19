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
	virtual bool getMotion(MotionData& d) { return get_override("getMotion")(d); };
	virtual bool getSyncData(SyncData& d) { return get_override("getSyncData")(d); };
	virtual bool setSyncData(const SyncData& d) { return get_override("setSyncData")(d); };
	virtual bool exitWatchSync() { return get_override("exitWatchSync"); };

	operator Service&() { return *this; }
};

BOOST_PYTHON_MODULE(_ez430)
{
	class_<ServiceWrap, boost::noncopyable>("Service")
		.def("getMotion", &ServiceWrap::getMotion)
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

	class_<Watch>("Watch", init<Service&>())
		.def("getMotion", &Watch::getMotion)
		.def("getButton", &Watch::getButton)
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
}
