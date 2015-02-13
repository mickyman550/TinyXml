#include "stdafx.h"
#include "Sys_Timespan.h"

#include <assert.h>

namespace Sys
{

// Constructor
TimeSpan::TimeSpan(__int64 i64Ticks)
{
	assert(i64Ticks >= i64MINVALUE && i64Ticks <= i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Constructor
TimeSpan::TimeSpan(int iDays, int iHours, int iMinutes, int iSeconds)
{
	__int64 i64Ticks = (iDays * 86400 + iHours * 3600 + iMinutes * 60 + iSeconds) * i64TICKSPERSECOND;
	assert(i64Ticks >= i64MINVALUE && i64Ticks <= i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Constructor
TimeSpan::TimeSpan(int iDays, int iHours, int iMinutes, int iSeconds, int iMilliseconds)
{
	__int64 i64Ticks = ((iDays * 86400 + iHours * 3600 + iMinutes * 60 + iSeconds) * i64TICKSPERSECOND) + iMilliseconds * i64TICKPERMILLISECOND;
	assert(i64Ticks >= i64MINVALUE && i64Ticks <= i64MAXVALUE);
	m_i64Ticks = i64Ticks;
}

// Add ticks
TimeSpan TimeSpan::AddTicks(__int64 i64Ticks) const
{
	assert(i64Ticks <= (TimeSpan::i64MAXVALUE - m_i64Ticks) && i64Ticks >= (TimeSpan::i64MINVALUE - m_i64Ticks));
	return TimeSpan(i64Ticks + m_i64Ticks);
}

// Get days, hours, seconds
void TimeSpan::GetTime(int * piDays, int * piHours, int * piMinutes, int * piSeconds)
{
	__int64 i64Ticks = m_i64Ticks / i64TICKSPERSECOND;
	*piDays = int(i64Ticks / 86400);
	i64Ticks -= *piDays * 86400;
	*piHours = int(i64Ticks / 3600);
	i64Ticks -= *piHours * 3600;
	*piMinutes = int(i64Ticks / 60);
	i64Ticks -= *piMinutes * 60;
	*piSeconds = int(i64Ticks);
}

// Operators.
bool TimeSpan::operator < (const TimeSpan timeSpan) const
{
	return m_i64Ticks < timeSpan.m_i64Ticks;
}

bool TimeSpan::operator <= (const TimeSpan timeSpan) const
{
	return m_i64Ticks <= timeSpan.m_i64Ticks;
}

bool TimeSpan::operator > (const TimeSpan timeSpan) const
{
	return m_i64Ticks > timeSpan.m_i64Ticks;
}

bool TimeSpan::operator >= (const TimeSpan timeSpan) const
{
	return m_i64Ticks >= timeSpan.m_i64Ticks;
}

bool TimeSpan::operator == (const TimeSpan timeSpan) const
{
	return m_i64Ticks == timeSpan.m_i64Ticks;
}

bool TimeSpan::operator != (const TimeSpan timeSpan) const
{
	return m_i64Ticks != timeSpan.m_i64Ticks;
}

TimeSpan TimeSpan::operator +(const TimeSpan timeSpan) const
{
	return AddTicks(timeSpan.m_i64Ticks);
}

TimeSpan TimeSpan::operator -(const TimeSpan timeSpan) const
{
	return AddTicks(-timeSpan.m_i64Ticks);
}

} // namespace Sys