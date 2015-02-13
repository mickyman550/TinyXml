#pragma once

#include <windows.h>
#include <math.h>
#include "Sys_DateTime.h"

namespace TimeCodeConverter
{

#define SECS_PER_HOUR 3600.0
#define HOURS_PER_DAY 24
#define DAYS_PER_WEEK 7
#define SECS_PER_DAY SECS_PER_HOUR*HOURS_PER_DAY
#define SECS_PER_WEEK SECS_PER_DAY*DAYS_PER_WEEK
#define SECS_PER_YEAR SECS_PER_DAY*365	//no leap year
#define TICKS_PER_SECOND 10000000.0
#define TICKS_PER_YEAR SECS_PER_YEAR*TICKS_PER_SECOND

	static void TicksToSecondsOfWeek(__int64 i64Ticks, double *pdSecsOfWeek)
	{
		Sys::DateTime dateTime(i64Ticks);
		int iStartYear, iStartMonth, iStartDay;
		dateTime.GetDate(&iStartYear, &iStartMonth, &iStartDay);
		Sys::DateTime StartDate(iStartYear, iStartMonth, iStartDay);
		int iSecsPerWeek = SECS_PER_DAY * DAYS_PER_WEEK;
		__int64 i64TicksPerWeek = TICKS_PER_SECOND*iSecsPerWeek;
		__int64 i64StartDayTicks = StartDate.GetTicks();
		__int64 i64NumWeeks = floor(double(i64StartDayTicks) / double(i64TicksPerWeek));
		__int64 i64SecondsOfWeek = (i64StartDayTicks - i64NumWeeks*i64TicksPerWeek) / double(TICKS_PER_SECOND);
		__int64 i64LtcDayOfWeek = i64SecondsOfWeek / __int64(SECS_PER_DAY) + 1;
		__int64 i64LtcWeekStart = StartDate.AddDays(-i64LtcDayOfWeek).GetTicks();	//ticks up to start of week.
		__int64 i64TicksOfWeek = i64Ticks - i64LtcWeekStart;
		*pdSecsOfWeek = double(i64TicksOfWeek) / double(TICKS_PER_SECOND);		//seconds of the week.
	}

	static void TicksToSecondsOfDay(__int64 i64Ticks, double *pdSecsOfDay)
	{
		double dSecsOfWeek;
		TicksToSecondsOfWeek(i64Ticks, &dSecsOfWeek);
		*pdSecsOfDay = dSecsOfWeek - SECS_PER_DAY*floor(dSecsOfWeek / double(SECS_PER_DAY));
	}

};