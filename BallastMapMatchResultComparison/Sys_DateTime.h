/***************************************************************************************************
Sys_DateTime

Class for holding the time and performing date and time calculations

Resolution of 100 nanoseconds. (1 Tick = 100 nanoseconds) (0.0000001 seconds)

Year can be 1 - 9999 (1 AD to 9999 AD)
Month is 1 - 12 (1 - Jan)
Day is 1 - 31 
Hours is 0 - 23
Minutes is 0 - 59
Seconds is 0 - 59
Day of week is 0 - 6   (0 - Sunday)

LEAP SECONDS ARE NOT ACCOUNTED FOR!
****************************************************************************************************
Authors :
David Soane (DAS)
****************************************************************************************************
Ver: By:  Date :      Comment :
1    DAS  2009-04-16  First version
2    DAS  2009-06-18  Added SetTime(double dSecondOfTheDay);
***************************************************************************************************/
#ifndef SYS_DATETIME_H
#define SYS_DATETIME_H

#include "Sys_TimeSpan.h"

namespace Sys
{


class DateTime
{
public:
	static const __int64 i64MINVALUE = 0;
    static const __int64 i64MAXVALUE = 0x2bca2875f4373fff;

	// Constructor
	DateTime();
	DateTime(__int64 i64Ticks);
	DateTime(int iYear, int iMonth, int iDay);
	DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds);
	DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, int iMilliseconds);
	DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, double dMilliseconds);

	// Set the time and date
	void SetTimeDate(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, double dMilliseconds);

	// Set the date (without affecting the time).
	void SetDate(int iYear, int iMonth, int iDay);

	// Set the time (without affecting the date)
	void SetTime(int iHours, int iMinutes, int iSeconds, double dMilliseconds);

	// Set the time (without affecting the date). 
	// (This internally does a dSecondOfTheDay mod 86400 so it does not effect the day)
	void SetTime(double dSecondOfTheDay);

	// Add ticks
	DateTime AddTicks(__int64 i64Ticks) const;

	// Add days
	DateTime AddDays(double dDays);

	// Get day month year
	void GetDate(int * piYear, int * piMonth, int * piDay) const;

	// Get day month year day of week
	void GetDate(int * piYear, int * piMonth, int * piDay, int * piDayOfWeek) const;

	// Get the time.
	void GetTime(int * piHour, int * piMinute, int * piSecond) const;

	// Get the time.
	void GetTime(int * piHour, int * piMinute, int * piSecond, double * pdMilliseconds) const;

	// Get the time. Returns the second of the day.
	double GetTimeSecondsOfDay() const;

	// Returns the day of the week.
	int GetDayOfWeek() const { return int(((m_i64Ticks / i64TICKSPERDAY) + 1) % 7); }

	__int64 GetTicks() const { return m_i64Ticks; }

	// Returns true if year is a leap year.
	static bool IsLeapYear(int iYear);

	// Operators.
	bool operator < (const DateTime dateTime) const;
	bool operator <= (const DateTime dateTime) const;
	bool operator > (const DateTime dateTime) const;
	bool operator >= (const DateTime dateTime) const;
	bool operator == (const DateTime dateTime) const;
	bool operator != (const DateTime dateTime) const;
	TimeSpan operator - (const DateTime dateTime) const;

private:
	// Returns the date in ticks.
	static __int64 DateToTicks(int iYear, int iMonth, int iDay);

	// Returns the time in ticks.
	static __int64 TimeToTicks(int iHours, int iMinutes, int iSeconds);

	// Add 
	DateTime add(double dValue, int iScale);

	__int64 m_i64Ticks;		// The time in ticks.

	static const int aiDAYSTOMONTH365[13];
	static const int aiDAYSTOMONTH366[13];
	static const __int64 i64TICKSPERSECOND = 10000000;
	static const __int64 i64TICKSPERDAY = 864000000000;
	static const int iDAYSPER100YEARS = 0x8eac;
    static const int iDAYSPER400YEARS = 0x23ab1;
    static const int iDAYSPER4YEARS = 0x5b5;
    static const int iDAYSPERYEAR = 0x16d;
	static const __int64 i64TICKPERMILLISECOND = 0x2710;
	static const __int64 i64MILLISPERDAY = 0x5265c00;
	static const __int64 i64MAXMILLIS = 0x11efae44cb400;
};

} //namespace sys

#endif