/*
 * service.h
 * Copyright © 2010 François-Xavier 'Bombela' Bourlet <bombela@gmail.com>
 *
*/

#pragma once
#ifndef SERVICE_H
#define SERVICE_H

/**
* @namespace ez430
* Global namespace of the library
*/
namespace ez430 {

/**
* @namespace protocol
* Global namespace for the protocole
*/
namespace protocol {

/**
* @struct SyncData
* Representation of the data in the watch
*/
struct SyncData
{
	bool  useMetric;
	int   hour;
	int   minute;
	int   second;
	int   year;
	int   month;
	int   day;
	int   alarmHour;
	int   alarmMinute;
	float temperature;
	int   altitude;
};

/**
* @struct MotionData
* Contains all information the watch send per each packet of ACC or PPT
*/
struct MotionData
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
};

/**
* @class Service
* This class can manipulate data on watch easier
*/
class Service
{
	public:
		virtual ~Service() {};

        /**
        * @brief Get the Motion from the connected device
	    * @return If True getMotion succeed otherwise return false
        */
		virtual bool     getMotion(MotionData& d)       = 0;
		
        /**
        * @brief Ask Synchonisation Data from the watch
	    * @return If True requestSyncData succeed otherwise return false
        */
		virtual bool     requestSyncData()              = 0;

        /**
        * @brief Get the Synchonisation Data
	    * @return If True getSyncData succeed otherwise return false
        */
		virtual bool     getSyncData(SyncData& d)       = 0;

        /**
        * @brief Set the synchronisation on the watch
	    * @return If True setSyncData succeed otherwise return false
        */
		virtual bool     setSyncData(const SyncData& d) = 0;

		/**
	    * @brief Exit the sync mode on the watch
	    * @return If True exit succeed otherwise return false
	    */
		virtual bool     exitWatchSyncMode()            = 0;
};

} // namespace protocol
} // namespace ez430

#endif /* SERVICE_H */
