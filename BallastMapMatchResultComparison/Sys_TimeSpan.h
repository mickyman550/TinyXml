/***************************************************************************************************
Sys_TimeSpan

Class for holding a timespan

Resolution of 100 nanoseconds. (1 Tick = 100 nanoseconds)

****************************************************************************************************
Authors :
David Soane (DAS)
****************************************************************************************************
Ver: By:  Date :      Comment :
1    DAS  2009-04-16  First version
***************************************************************************************************/
#ifndef SYS_TIMESPAN_H
#define SYS_TIMESPAN_H

namespace Sys
{

class TimeSpan
{
public:
	static const __int64 i64MINVALUE = -0x2bca2875f4373fff;
	static const __int64 i64MAXVALUE = 0x2bca2875f4373fff;

	// Constructors
	TimeSpan(__int64 i64Ticks);
	TimeSpan(int iDays, int iHours, int iMinutes, int iSeconds);
	TimeSpan(int iDays, int iHours, int iMinutes, int iSeconds, int iMilliseconds);

	// Get ticks
	__int64 GetTicks() { return m_i64Ticks; }

	// Get days, hours, seconds
	void GetTime(int * piDays, int * piHours, int * piMinutes, int * piSeconds);

	// Add ticks
	TimeSpan AddTicks(__int64 i64Ticks) const;

	// Operators.
	bool operator < (const TimeSpan timeSpan) const;
	bool operator <= (const TimeSpan timeSpan) const;
	bool operator > (const TimeSpan timeSpan) const;
	bool operator >= (const TimeSpan timeSpan) const;
	bool operator == (const TimeSpan timeSpan) const;
	bool operator != (const TimeSpan timeSpan) const;
	TimeSpan operator +(const TimeSpan timeSpan) const;
	TimeSpan operator -(const TimeSpan timeSpan) const;

private:
	__int64 m_i64Ticks;

	static const __int64 i64TICKSPERDAY = 864000000000;
	static const __int64 i64TICKSPERSECOND = 10000000;
	static const __int64 i64TICKPERMILLISECOND = 0x2710;

};

} // namespace Sys

#endif