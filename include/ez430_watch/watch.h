/*
 * include/watch.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef WATCH_H
#define WATCH_H

#include <ez430_watch/service.h>
#include <vector>
#include <string>
#include <ostream>

/**
* @namespace ez430
* Global namespace of the library
*/
namespace ez430 {

struct Date
{
	int year;
	int month;
	int day;
	
    /**
    * @brief This function make easier the representation of the data of class Date
    * @return the stream
    */
	friend std::ostream& operator<<(std::ostream& os, const Date&);
};

struct Time
{
	int hour;
	int minute;
	int second;
	
    /**
    * @brief This function make easier the representation of the data of class Time
    * @return the stream
    */
	friend std::ostream& operator<<(std::ostream& os, const Time&);
};

/**
* @struct Motion
* Contains all information the watch send per each packet of ACC or PPT
*/
struct Motion
{
    /**
    * @enum Button
    * Represent the state of the Button on the watch
    */
	enum Button {
	    NONE, /**<Means no button have been pressed in this frame */
	    STAR, /**<Means star (*) button have been pressed in this frame */
	    NUM, /**<Means num (#) button have been pressed in this frame */
	    UP /**<Means up (^) button have been pressed in this frame */
	};

	Button button;

	bool   haveAcc;
	int    x;
	int    y;
	int    z;

    /**
    * @brief This function make easier the representation of the data of class Motion
    * @return the stream
    */
	friend std::ostream& operator<<(std::ostream& os, const Motion& motion);
};

/**
* @class Watch
* Main watch class allow user to access all data and functionnality of the watch
*/
class Watch
{
	public:
	    /**
	    * @enum Unit
	    * List all type of Unit the watch handle
	    */
		enum Unit {
		    METRIC, /**<Represent the Metric unit system */
		    AMERICAN /**<Represent the Americain unit system */
		};

		~Watch();
		Watch(protocol::Service& service);

	    /**
	    * @brief Get the level of smoothiness
	    */
		float    getSmooth() const;

	    /**
	    * @brief Set a value to soften the income motion data
	    * @param value : value in % between 0.0 and 1.0 for example 0.8 means 80%
	    */
		void     setSmooth(float value);
		
	    /**
	    * @brief Get the reading watch timeout in milisecond
	    */
		unsigned getTimeout() const;

	    /**
	    * @brief Set the reading watch timeout in milisecond
	    */
		void     setTimeout(unsigned);
		
	    /**
	    * @brief Try to get the motion from the watch without timeout
	    * @return If True the motion is pass in parameter is modified with the watch information, otherwise return false
	    */
		bool           tryGetMotion(Motion&);

	    /**
	    * @brief Get Motion from the watch with timeout
	    * @return The actual ACC data of the wath
	    */
		Motion         getMotion();

	    /**
	    * @brief Get the pressed button from the watch with timeout
	    * @return The actual Button pressed of the wath
	    */
		Motion::Button getButton();

	    /**
	    * @brief Get time from the watch
	    * @return Return the actual time in the watch
	    */
		Time     getTime();

	    /**
	    * @brief Set time on the watch
	    */
		bool     setTime(Time);
		
		/**
	    * @brief Get date from the watch
	    * @return Return the actual date in the watch
	    */
		Date     getDate();

		/**
	    * @brief Set date on the watch
	    * @return If True date modification succeed otherwise return false
	    */
		bool     setDate(Date);

		/**
	    * @brief Get alarm time from the watch
	    * @return Return the actual alarm time in the watch
	    */
		Time     getAlarm();

		/**
	    * @brief Set alarm time into the watch
	    * @return If True alarm time modification succeed otherwise return false
	    */
		bool     setAlarm(Time);

		/**
	    * @brief Set watch time and date at the system time and date
	    * @return If True modification succeed otherwise return false
	    */
		bool     setSystemDateAndTime();

		/**
	    * @brief Get the actual temperature in the watch
	    */
		float    getTemperature();

		/**
	    * @brief Set watch temperature
	    * @return If True modification succeed otherwise return false
	    */
		bool     setTemperature(float);

		/**
	    * @brief Get the actual altitude in the watch
	    */
		int      getAltitude();

		/**
	    * @brief Set watch altitude
	    * @return If True modification succeed otherwise return false
	    */
		bool     setAltitude(float);

		/**
	    * @brief Get the Unit system used by the watch
	    */
		Unit     getUnitSystem();

		/**
	    * @brief Set watch Unit System
	    * @return If True modification succeed otherwise return false
	    */
		bool     setUnitSystem(Unit);

		/**
	    * @brief Exit the sync mode on the watch
	    * @return If True exit succeed otherwise return false
	    */
		bool     exitWatchSyncMode();

	private:
		class Implementation *_impl;
};

} // namespace watch

#endif /* WATCH_H */
