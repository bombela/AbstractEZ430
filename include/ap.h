/*
 * ap.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef AP_H
#define AP_H

namespace ez430 {

class AccessPoint
{
	public:
		enum RadioState { STOPPED, LINKING, LINKED };

		~AccessPoint();
		AccessPoint();
		AccessPoint(std::string line);

		void open(std::string line);
		bool isOpen() const;

		void startRadio();
		void stopRadio();
		
		State getRadioState();

		WatchService& getService();

		static std::vector<std::string> probeLines();

	private:
		class Implementation *_impl;
};

} // namespace ez430

#endif /* AP_H */
