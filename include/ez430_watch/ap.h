/*
 * ap.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef AP_H
#define AP_H

#include <ez430_watch/service.h>

#include <string>
#include <vector>

namespace ez430 {

class AccessPoint
{
	public:
		enum RadioState { STOPPED, LINKING, LINKED };

		~AccessPoint();
		AccessPoint();
		AccessPoint(const std::string& line);

		void open(const std::string& line);
		bool isOpen() const;
		void close();

		int getProductId() const;

		void startRadio();
		void stopRadio();
		
		RadioState getRadioState();

		protocol::Service& getService();

		static std::vector<std::string> probeLines();

	private:
		class Implementation *_impl;
};

} // namespace ez430

#endif /* AP_H */
