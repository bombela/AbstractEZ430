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


/**
* @namespace ez430
* Global namespace of the library
*/
namespace ez430 {

/**
* @class AccessPoint
* Specific namespace of the AccessPoint
*/
class AccessPoint
{
	public:
        /**
        * @enum RadioState
        * Represents the different states of the Access Point
        */
		enum RadioState {
		    STOPPED, /**<The Access Point is Stopped */
		    LINKING, /**<The Access Point is in Linking */
		    LINKED /**<The Access Point is Linked */
		};

		~AccessPoint();
		AccessPoint();

        /**
        * @brief Constructor
        * @param line : Is the serial port name
        */
		AccessPoint(const std::string& line);

        /**
        * @brief Open a serial port
        * @param line : path of the serial port
        */
		void open(const std::string& line);

        /**
        * @brief Test if the serial port is open
        * @return True if the serial port is open, false otherwise
        */
		bool isOpen() const;

        /**
        * @brief Close the Access Point serial port
        */
		void close();

        /**
        * @brief Get the product id
        * @return Return the product id
        */
		int getProductId() const;

        /**
        * @brief Start the Access Point radio
        */
		void startRadio();

        /**
        * @brief Stop the Access Point radio
        */
		void stopRadio();
		
        /**
        * @brief Get the status of the Access Point
        * @return
        */
		RadioState getRadioState();

        /**
        * @brief Get the Access Point service
        * @return 
        */
		protocol::Service& getService();

        /**
        * @brief Get the serial port list of Access Point connected
        * @return List of relieable Access Point
        */
		static std::vector<std::string> probePorts();

	private:
        /**
        * @class Implementation
        * PIMPL Idiom
        */
		class Implementation *_impl;
};

} // namespace ez430

#endif /* AP_H */
