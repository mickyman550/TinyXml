#include "stdafx.h"
#include "Sys_DateTime.h"

#include <assert.h>
#include <math.h>

namespace Sys
{

const int DateTime::aiDAYSTOMONTH365[13] = {0, 0x1f, 0x3b, 90, 120, 0x97, 0xb5, 0xd4, 0xf3, 0x111, 0x130, 0x14e, 0x16d};
const int DateTime::aiDAYSTOMONTH366[13] = {0, 0x1f, 60, 0x5b, 0x79, 0x98, 0xb6, 0xd5, 0xf4, 0x112, 0x131, 0x14f, 0x16e};

// Constructor
DateTime::DateTime()
: m_i64Ticks(0)
{
}

// Constructor
DateTime::DateTime(__int64 i64Ticks)
{
	assert(i64Ticks >= 0 && i64Ticks <= i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Constructor
DateTime::DateTime(int iYear, int iMonth, int iDay)
{
	m_i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay);
}

// Constructor
DateTime::DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds)
{
	m_i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay) + DateTime::TimeToTicks(iHours, iMinutes, iSeconds);
}

// Constructor
DateTime::DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, int iMilliseconds)
{
	__int64 i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay) + DateTime::TimeToTicks(iHours, iMinutes, iSeconds);

	assert(iMilliseconds >= 0 && iMilliseconds < 1000);
	i64Ticks += iMilliseconds * 10000;
	assert(i64Ticks >= DateTime::i64MINVALUE && i64Ticks <= DateTime::i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Constructor
DateTime::DateTime(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, double dMilliseconds)
{
	__int64 i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay) + DateTime::TimeToTicks(iHours, iMinutes, iSeconds);
	//assert(dMilliseconds >= 0 && dMilliseconds < 1000);
	i64Ticks += __int64( (dMilliseconds * 10000.0) + 0.5);
	assert(i64Ticks >= DateTime::i64MINVALUE && i64Ticks <= DateTime::i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Set the time and date
void DateTime::SetTimeDate(int iYear, int iMonth, int iDay, int iHours, int iMinutes, int iSeconds, double dMilliseconds)
{
	__int64 i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay) + DateTime::TimeToTicks(iHours, iMinutes, iSeconds);
	assert(dMilliseconds >= 0 && dMilliseconds < 1000);
	i64Ticks += __int64( (dMilliseconds * 10000.0) + 0.5);
	assert(i64Ticks >= DateTime::i64MINVALUE && i64Ticks <= DateTime::i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Set the date (without affecting the time).
void DateTime::SetDate(int iYear, int iMonth, int iDay)
{
	__int64 iTime = m_i64Ticks % i64TICKSPERDAY;
	m_i64Ticks = DateTime::DateToTicks(iYear, iMonth, iDay) + iTime;

}

// Set the time (without affecting the date)
void DateTime::SetTime(int iHours, int iMinutes, int iSeconds, double dMilliseconds)
{
	__int64 i64Time = DateTime::TimeToTicks(iHours, iMinutes, iSeconds);
	assert(dMilliseconds >= 0 && dMilliseconds < 1000);
	i64Time += __int64( (dMilliseconds * 10000.0) + 0.5);
	m_i64Ticks = (m_i64Ticks - (m_i64Ticks % i64TICKSPERDAY)) + i64Time;
}

// Set the time (without affecting the date). 
// (This internally does a dSecondOfTheDay mod 86400 so it does not effect the day)
void DateTime::SetTime(double dSecondOfTheDay)
{
	__int64 i64TimeOfDay = __int64( (fmod(dSecondOfTheDay, 86400.0) * double(i64TICKSPERSECOND)) + 0.5);
	m_i64Ticks = (m_i64Ticks - (m_i64Ticks % i64TICKSPERDAY)) + i64TimeOfDay; 
}

// Add ticks
DateTime DateTime::AddTicks(__int64 i64Ticks) const
{
	assert(i64Ticks < (DateTime::i64MAXVALUE - m_i64Ticks) && i64Ticks > 0 - m_i64Ticks);
	return DateTime(i64Ticks + m_i64Ticks);
}

// Add days.
DateTime DateTime::AddDays(double dDays)
{
	return add(dDays, i64MILLISPERDAY);
}

// Get day month year
void DateTime::GetDate(int * piYear, int * piMonth, int * piDay) const
{
	int iNum2 = int(m_i64Ticks / i64TICKSPERDAY);
	int iNum3 = iNum2 / iDAYSPER400YEARS;
	iNum2 -= (iNum3 * iDAYSPER400YEARS);
	int iNum4 = iNum2 / iDAYSPER100YEARS;
	if (iNum4 == 4)
	{
		iNum4 = 3;
	}
	iNum2 -= iNum4 * iDAYSPER100YEARS;
	int iNum5 = iNum2 / iDAYSPER4YEARS;
	iNum2 -= iNum5 * iDAYSPER4YEARS;
	int iNum6 = iNum2 / iDAYSPERYEAR;
	if (iNum6 == 4)
	{
		iNum6 = 3;
	}

	*piYear = (((((iNum3 * 400) + (iNum4 * 100)) + (iNum5 * 4)) + iNum6) + 1);
	
	iNum2 -= iNum6 * iDAYSPERYEAR;
	int iDayOfYear = iNum2 + 1;

	const int * pDayArray = (iNum6 == 3 && (iNum5 != 0x18 || iNum4 == 3)) ? DateTime::aiDAYSTOMONTH366 : DateTime::aiDAYSTOMONTH365;
	int iIndex = iNum2 >> 6;
    
	while (iNum2 >= pDayArray[iIndex])
	{
		iIndex++;
	}
	*piMonth = iIndex;
	*piDay = iDayOfYear - pDayArray[iIndex - 1];
}


// Get day month year day of week
void DateTime::GetDate(int * piYear, int * piMonth, int * piDay, int * piDayOfWeek) const
{
	GetDate(piYear, piMonth, piDay);
	*piDayOfWeek = GetDayOfWeek();
}

// Get the time.
void DateTime::GetTime(int * piHour, int * piMinute, int * piSecond) const
{
	__int64 i64Time = m_i64Ticks % i64TICKSPERDAY;
	int iTime = int(i64Time / i64TICKSPERSECOND);
	*piHour = iTime / 3600;
	iTime -= *piHour * 3600;
	*piMinute = iTime / 60;
	iTime -= *piMinute * 60;
	*piSecond = iTime;
}

// Get the time.
void DateTime::GetTime(int * piHour, int * piMinute, int * piSecond, double * pdMilliseconds) const
{
	__int64 i64Time = m_i64Ticks % i64TICKSPERDAY;
	*piHour = int(i64Time / (3600 * i64TICKSPERSECOND));
	i64Time -= *piHour * 3600 * i64TICKSPERSECOND;
	*piMinute = int(i64Time / (60 * i64TICKSPERSECOND));
	i64Time -= *piMinute * 60 * i64TICKSPERSECOND;
	*piSecond = int(i64Time / i64TICKSPERSECOND);
	i64Time -= *piSecond * i64TICKSPERSECOND;
	*pdMilliseconds = double(i64Time) / i64TICKPERMILLISECOND;
}

// Get the time. Returns the second of the day.
double DateTime::GetTimeSecondsOfDay() const
{
	return double(m_i64Ticks % i64TICKSPERDAY) / i64TICKSPERSECOND;
}

// Returns true if year is a leap year.
bool DateTime::IsLeapYear(int iYear)
{
	assert(iYear >= 1 && iYear <= 9999);

    if ((iYear % 4) != 0)
    {
        return false;
    }
    if ((iYear % 100) == 0)
    {
        return ((iYear % 400) == 0);
    }
    return true;
}

	// Operators.
bool DateTime::operator < (const DateTime dateTime) const
{
	return m_i64Ticks < dateTime.m_i64Ticks;
}

bool DateTime::operator <= (const DateTime dateTime) const
{
	return m_i64Ticks <= dateTime.m_i64Ticks;
}

bool DateTime::operator > (const DateTime dateTime) const
{
	return m_i64Ticks > dateTime.m_i64Ticks;
}

bool DateTime::operator >= (const DateTime dateTime) const
{
	return m_i64Ticks >= dateTime.m_i64Ticks;
}

bool DateTime::operator == (const DateTime dateTime) const
{
	return m_i64Ticks == dateTime.m_i64Ticks;
}

bool DateTime::operator != (const DateTime dateTime) const
{
	return m_i64Ticks != dateTime.m_i64Ticks;
}

TimeSpan DateTime::operator - (const DateTime dateTime) const
{
	return TimeSpan(m_i64Ticks - dateTime.m_i64Ticks);
}

// Returns the date in ticks.
 __int64 DateTime::DateToTicks(int iYear, int iMonth, int iDay)
{
	assert(iYear >= 1 && iYear <= 9999 && iMonth >= 1 && iMonth <= 12);

	const int * pDayArray = DateTime::IsLeapYear(iYear) ? DateTime::aiDAYSTOMONTH366 : DateTime::aiDAYSTOMONTH365;
    assert(iDay >= 1 && iDay <= (pDayArray[iMonth] - pDayArray[(iMonth - 1)]));

	int iNum = iYear - 1;
	int iNum2 = (((((((iNum * 365) + (iNum / 4)) - (iNum / 100)) + (iNum / 400)) + pDayArray[(iMonth - 1)]) + iDay) - 1);
    return (iNum2 * i64TICKSPERDAY);
}

// Returns the time in ticks.
__int64 DateTime::TimeToTicks(int iHours, int iMinutes, int iSeconds)
{
	assert(iHours >= 0 && iHours < 24 && iMinutes >= 0 && iMinutes < 60 && iSeconds >= 0 && iSeconds < 60);
	return (iHours * 3600 + iMinutes * 60 + iSeconds) * DateTime::i64TICKSPERSECOND;
}

// Add time
DateTime DateTime::add(double dValue, int iScale)
{
	__int64 iNum = __int64(((dValue * iScale) + ((dValue >= 0) ? 0.5 : -0.5)));
	assert(iNum > -i64MAXMILLIS && iNum < i64MAXMILLIS);

   return AddTicks((iNum * i64TICKPERMILLISECOND));
}

} //namespace Sys