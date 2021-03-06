/*
 * $Id$
 *
 * Classes related to date/time and timezones.
 * Part of edelib.
 * Copyright (c) 2005-2007 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "econfig.h"

EDELIB_NAMESPACE {

class TimeZone
{
	private:
		char* pathval;
		char* zoneval;
		char* tzone;
		unsigned long timeval;

		bool load(const char* path, const char* zone);
		bool load_local(void);
		void clear(void);
	public:
		TimeZone();
		~TimeZone();
		bool set(const char* path, const char* zone);
		unsigned long time(void);
		const char* get_timezone(void);
};

enum DateType {
	DATE_LOCAL = 0,   ///< use local date
	DATE_UTC          ///< use UTC date
};

/**
 * \class Date
 * \brief A class for date manipulation.
 *
 * This class represents a calendar date. It has ability to read
 * system date or to use and manipulate with filled year/month/day
 * values.
 *
 * Setting date is via set() member, and it accept year/month/day
 * form. This is sample:
 * \code
 *   Date mydate;
 *   mydate.set(2006, 1, 1);
 *   // following will be true
 *   mydate.day() == 1;
 *   mydate.month() == 1;
 *   mydate.year() == 1;
 * \endcode
 *
 * set() can also accept abbreviations for months or shorthands for
 * current year/month/day (in this case will consult system date)
 * \code
 *   ...
 *   mydate.set(2006, Date::Jun, 10);
 *   // or use curren date
 *   mydate.set(Date::YearNow, Date::MonthNow, Date::DayNow);
 * \endcode
 *
 * set() will check input, and in case of bad date (either abnormal input 
 * values or setting feb/29 in non leap year) will trigger assert.
 *
 * There is a one catch using abbreviations and valid date. For example,
 * setting:
 * \code
 *   mydate.set(0,1,0)
 * \endcode
 * is perfectly valid, since Date::YearNow and Date::DayNow enumerations
 * are zero; in this case, function will behave as it was set:
 * \code
 *   mydate.set(Date::YearNow, Date::Jan, Date::DayNow);
 * \endcode
 *
 * Date contains operators for comparrisons, so this can be done with those:
 * \code
 *   Date d1,d2;
 *   d1.set(2007, 1, 1);
 *   d2.set(2007, 2, 1);
 *
 *   d1 != d2;   // will be true
 *   d1 <= d2;   // also true
 *
 *   d2 = d1;    // assign d1 values
 *   d1 == d2;   // true
 *   ...
 * \endcode
 */
class Date
{
	private:
		unsigned char dayval;
		unsigned char monthval;
		unsigned short yearval;

	public:
		enum Month { 
			Jan = 1,   ///< January
			Feb,       ///< February
			Mar,       ///< March
			Apr,       ///< April
			May,       ///< May 
			Jun,       ///< June
			Jul,       ///< July
			Aug,       ///< August
			Sep,       ///< September
			Oct,       ///< October
			Nov,       ///< November
			Dec,       ///< December
			MonthNow   ///< Current system month
		};

		enum Day {
			DayNow = 0  ///< Current system date
		};

		enum Year { 
			YearNow = 0 ///< Current system year
		};

		/**
		 * Construct empty date. All walues will be zero. This is
		 * not valid, so is_valid() member will return false.
		 */
		Date();

		/**
		 * Construct date via previously declared value
		 */
		Date(const Date& d);

		/**
		 * Assign previously declared value to this. Current content
		 * will be replaced
		 */
		Date& operator=(const Date& d);

		/**
		 * Clear internal data
		 */
		~Date();

		/**
		 * Fill internal data. Data should be valid date (including leap year)
		 * or assertion will be triggered.
		 *
		 * Input can be either in form of numbers or abbreviations for month.
		 *
		 * \return todo
		 * \param y fill with year value
		 * \param m fill with month value; should be 1 <= m <= 12
		 * \param d fill with day value; should be 1 <= d <= 30 (or 31 for months this days number
		 * or 28 if is non-leap year)
		 * \param t fill with localized or UTC date values
		 */
		bool set(unsigned short y, unsigned char m, unsigned char d, DateType t = DATE_LOCAL);

		/**
		 * Tries to set system date with current values. This function
		 * will try to call settimeofday() and if not exists, it will fallback to
		 * stime(). If both functions are not present, it will return false.
		 *
		 * Since these functions requires priviledged user, if they are present
		 * but user is not allow to set date, this function will return false too.
		 * Contrary this, it will return true if able to set date.
		 */
		bool system_set(void);

		/**
		 * Check if given year is leap year
		 *
		 * \return true if it is or false if not
		 * \param y is year to check
		 */
		bool leap_year(unsigned short y) const;

		/** Check internal year value if is leap year */
		bool leap_year(void) const { return leap_year(yearval); }

		/**
		 * Validate given year/month/day values, including leap year check
		 *
		 * \return true if date is correct, or false if not
		 * \param y is year
		 * \param m is month
		 * \param d is day
		 */
		bool is_valid(unsigned short y, unsigned char m, unsigned char d) const;

		/** Return internal day value */
		unsigned char  day(void) const    { return dayval; }
		/** Return internal month value */
		unsigned char  month(void) const  { return monthval; }
		/** Return internal year value */
		unsigned short year(void) const   { return yearval; }

		/**
		 * Return day name for internal day value, eg. Monday/Tuesday/...
		 * Returned data <b>must not</b> be modified since is pointing
		 * to static data. You should copy it if you need to modify it.
		 */
		const char*    day_name(void);

		/**
		 * Return month name for internal month value, eg. January/February/...
		 * Returned data <b>must not</b> be modified, since applies as for
		 * day_name().
		 */
		const char*    month_name(void);

		/**
		 * Return number of days in given year and month. Given data must
		 * be valid date
		 *
		 * \return number of days for given year/month
		 * \param y is year to check
		 * \param m is month to check
		 */
		unsigned char  days_in_month(unsigned short y, unsigned char m) const; 

		/** Return number of days of internal year/month value */
		unsigned char  days_in_month() const; 
		/** 
		 * Return day in week (1..7) calculated from internal values.
		 * \note 
		 *   Values are in Western format, so 1 == Sunday, 2 == Monday, ...
		 */
		unsigned char  day_of_week() const;

		/** Return day of year of currently set date */
		unsigned short day_of_year() const;
};

#ifndef SKIP_DOCS
inline bool operator==(const Date& d1, const Date& d2)
{ return (d1.day() == d2.day() && d1.month() == d2.month() && d1.year() == d2.year()); }

inline bool operator>(const Date& d1, const Date& d2)
{
	return (d1.year() > d2.year() || (d1.year() == d2.year() && d1.month() > d2.month()) || 
		(d1.month() == d2.month() && d1.day() > d2.day()));
}
			
inline bool operator!=(const Date& d1, const Date& d2) { return !(d1 == d2); }
inline bool operator>=(const Date& d1, const Date& d2) { return (d1 > d2 || d1 == d2); }
inline bool operator<(const Date& d1, const Date& d2)  { return (!(d1 > d2) && (d1 != d2)); }
inline bool operator<=(const Date& d1, const Date& d2) { return (d1 == d2 || d1 < d2); }
#endif


class Time
{
	private:
		unsigned char hourval;
		unsigned char minval;
		unsigned char secval;
		unsigned short msecval;
	
	public:
		Time();
		Time(const Time& t);
		Time& operator=(const Time& t);

		~Time();
		bool set(unsigned char h, unsigned char m, unsigned char s = 0, unsigned short ms = 0);
		bool is_valid(unsigned char h, unsigned char m, unsigned char s, unsigned short ms);
		void set_now(void);

		bool system_set(void);

		unsigned char hour(void) const { return hourval; }
		unsigned char min(void) const  { return minval; }
		unsigned char sec(void) const  { return secval; }
		unsigned short msec(void) const { return msecval; }
};

#ifndef SKIP_DOCS
inline bool operator==(const Time& t1, const Time& t2)
{ return (t1.hour() == t2.hour() && t1.min() == t2.min() && t1.sec() == t2.sec() && t2.msec() == t2.msec()); }

inline bool operator>(const Time& t1, const Time& t2)
{ return (t1.hour()+t1.min()+t1.sec()+t1.msec() > t2.hour()+t2.min()+t2.sec()+t2.msec()); }

inline bool operator!=(const Time& t1, const Time& t2) { return !(t1 == t2); }
inline bool operator>=(const Time& t1, const Time& t2) { return (t1 > t2 || t1 == t2); }
inline bool operator<(const Time& t1, const Time& t2)  { return (!(t1 > t2) && (t1 != t2)); }
inline bool operator<=(const Time& t1, const Time& t2) { return (t1 == t2 || t1 < t2); }
#endif

}

#endif
