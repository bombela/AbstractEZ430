/*
 * binding/python/ap.cpp
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#include <boost/python.hpp>

#include <ez430_watch/ap.h>
#include <ez430_watch/service.h>

using namespace boost::python;
using namespace ez430;
using namespace ez430::protocol;

struct ServiceWrap: Service, wrapper<Service>
{
	virtual bool getMotion(MotionData& d) { return get_override("getMotion")(d); };
	virtual bool getSyncData(SyncData& d) { return get_override("getSyncData")(d); };
	virtual bool setSyncData(const SyncData& d) { return get_override("setSyncData")(d); };
	virtual bool exitWatchSync() { return get_override("exitWatchSync"); };
};

BOOST_PYTHON_MODULE(ez430)
{
	class_<ServiceWrap, boost::noncopyable>("Service")
		;

	enum_<AccessPoint::RadioState>("RadioState")
		.value("STOPPED", AccessPoint::STOPPED)
		.value("LINKING", AccessPoint::LINKING)
		.value("LINKED", AccessPoint::LINKED)
	;

	class_<AccessPoint>("AccessPoint")
		.def(init<std::string>())
		.def("open", &AccessPoint::open)
		.def("isOpen", &AccessPoint::isOpen)
		.def("close", &AccessPoint::close)
		.def("getProductId", &AccessPoint::getProductId)
		.def("startRadio", &AccessPoint::startRadio)
		.def("stopRadio", &AccessPoint::stopRadio)
		.def("getRadioState", &AccessPoint::getRadioState)
	//	.def("getService", &AccessPoint::getService, return_internal_reference<1>())
	;
}
