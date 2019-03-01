#pragma once
#ifndef WORLDSIM_CALENDAR_HPP
#define WORLDSIM_CALENDAR_HPP

/* WorldSim: GuildCalendar.hpp
  #include "GuildCalendar.hpp"

    I'm going with the following simple approach:

    60 seconds per minute.
    60 minutes per hour.
    24 hours per day.
    30 days per month.
    12 months per year.
    
    Some scaling can be implemented by lowering seconds per minute.
*/

class GuildCalendar
{
	public:
	
	
	int year, month, day, hour, minute, second;
	Vector <std::string> vMonthName;
  
  int secondsPerMinute;
	
	GuildCalendar()
	{
		year=0;
		month=0;
		day=0;
		hour=CALENDAR_INITIAL_HOUR;
		minute=CALENDAR_INITIAL_MINUTE;
		second=0;
    
    secondsPerMinute=CALENDAR_SECONDS_PER_MINUTE;
	}
	
	void set(const int _year, const int _month, const int _day, const int _hour, const int _minute, const int _second)
	{
		year=_year;
		month=_month;
		day=_day;
		hour=_hour;
		minute=_minute;
		second=_second;
	}
	
	//void setMonthNames (const std::string _m1, const std::string
	void addMonthName(const std::string _monthName)
	{
		vMonthName.push(_monthName);
	}
	
	void advanceSeconds(const int _seconds)
	{
		second+=_seconds;
		normalise();
	}
	void advanceSecond(const int _seconds) { advanceSeconds(_seconds); }
	void advanceMinutes(const int _minutes)
	{
		minute+=_minutes;
		normalise();
	}
	void advanceMinute(const int _minutes) { advanceMinutes(_minutes); }
	void advanceHours(const int _hours)
	{
		hour+=_hours;
		normalise();
	}
	void advanceHour(const int _hours) { advanceHours(_hours); }
	void advanceDays(const int _days)
	{
		day+=_days;
		normalise();
	}
	void advanceDay(const int _days) { advanceDays(_days); }
	
	void normalise()
	{
		/* Turn excess seconds into minutes. */
		if(second>=secondsPerMinute)
		{
			int nMinutes = second/secondsPerMinute;
			minute+=nMinutes;
			second-=(nMinutes*secondsPerMinute);
		}
		/* Turn excess minutes into hours. */
		if(minute>=60)
		{
			int nHours = minute/60;
			hour+=nHours;
			minute-=(nHours*60);
		}
		/* Turn excess hours into days. */
		if(hour>=24)
		{
			int nDays = hour/24;
			day+=nDays;
			hour-=(nDays*24);
		}
		/* Turn excess days into months. */
		if(day>=30)
		{
			int nMonths = day/30;
			month+=nMonths;
			day-=(nMonths*30);
		}
		/* Turn excess months into years. */
		if(month>=12)
		{
			int nYears = month/12;
			year+=nYears;
			month-=(nYears*12);
		}
	}
	
	std::string toString()
	{
		normalise();
		std::string output = "";
		output+=DataTools::toString(day,2);
		output+="/";
		output+=DataTools::toString(month,2);
		output+="/";
		output+=DataTools::toString(year,4);
		output+=" ";
		output+=DataTools::toString(hour,2);
		output+=":";
		output+=DataTools::toString(minute,2);
		output+=":";
		output+=DataTools::toString(second,2);
		return output;
	}
	
	
};

class GuildDate
{
	public:
	int second, minute, hour, day, month, year;
	
	void set(const int _year, const int _month, const int _day, const int _hour, const int _minute, const int _second)
	{
		year=_year;
		month=_month;
		day=_day;
		hour=_hour;
		minute=_minute;
		second=_second;
	}
	void set (GuildCalendar* c)
	{ set(c->year,c->month,c->day,c->hour,c->minute,c->second); }
	void set (GuildCalendar c)
	{ set(c.year,c.month,c.day,c.hour,c.minute,c.second); }
	void set (GuildDate* c)
	{ set(c->year,c->month,c->day,c->hour,c->minute,c->second); }
	void set (GuildDate c)
	{ set(c.year,c.month,c.day,c.hour,c.minute,c.second); }
	
	int getDistanceYears(GuildCalendar *c)
	{
		int nYears = year - c->year;
		if ( nYears<0 ) { nYears*=-1; }
		return nYears;
	}
	
	bool operator < (GuildDate * date)
	{
		if(year<date->year)
		{ return true; }
		else if(year>date->year)
		{ return false; }
		else
		{
			if (month<date->month)
			{ return true; }
			else if(month>date->month)
			{ return false; }
			else
			{
				if(day<date->day)
				{ return true; }
				else if (day>date->day)
				{ return false; }
				else
				{
					/* Increase resolution to hours and minutes and seconds later. */
				}
			}
		}
		return false;
	}
	
	bool operator < (GuildCalendar * date)
	{
		//std::cout<<"<";
		if(year<date->year)
		{ return true; }
		else if(year>date->year)
		{ return false; }
		else
		{
			// if (month<date->month)
			// { return true; }
			// else if(month>date->month)
			// { return false; }
			// else
			// {
				// if(day<date->day)
				// { return true; }
				// else if (day>date->day)
				// { return false; }
				// else
				// {
					// /* Increase resolution to hours and minutes and seconds later. */
					// return false;
				// }
			// }
		}
		return false;
	}
	

	int getDistanceDays(GuildCalendar* c)
	{
		int nDays=0;
		std::cout<<"Get distance days.\n";
		
		
		GuildDate counter;
		counter.set(this);
		
	
		//int currentYear = year;
		while(counter < c)
		{
			++nDays;
			counter.advanceDay(1);
		}
		
		std::cout<<"\nReturning: "<<nDays<<".\n";
		return nDays;
	}
	int distanceDays ( GuildCalendar* c)
	{ return getDistanceDays(c); }
	
	
	void advanceSeconds(const int _seconds)
	{
		second+=_seconds;
		normalise();
	}
	void advanceSecond(const int _seconds) { advanceSeconds(_seconds); }
	void advanceMinutes(const int _minutes)
	{
		minute+=_minutes;
		normalise();
	}
	void advanceMinute(const int _minutes) { advanceMinutes(_minutes); }
	void advanceHours(const int _hours)
	{
		hour+=_hours;
		normalise();
	}
	void advanceHour(const int _hours) { advanceHours(_hours); }
	void advanceDays(const int _days)
	{
		day+=_days;
		normalise();
	}
	void advanceDay(const int _days) { advanceDays(_days); }
	
	void normalise()
	{
		/* Turn excess seconds into minutes. */
		if(second>=60)
		{
			int nMinutes = second/60;
			minute+=nMinutes;
			second-=(nMinutes*60);
		}
		/* Turn excess minutes into hours. */
		if(minute>=60)
		{
			int nHours = minute/60;
			hour+=nHours;
			minute-=(nHours*60);
		}
		/* Turn excess hours into days. */
		if(hour>=24)
		{
			int nDays = hour/24;
			day+=nDays;
			hour-=(nDays*24);
		}
		/* Turn excess days into months. */
		if(day>=30)
		{
			int nMonths = day/30;
			month+=nMonths;
			day-=(nMonths*30);
		}
		/* Turn excess months into years. */
		if(month>=12)
		{
			int nYears = month/12;
			year+=nYears;
			month-=(nYears*12);
		}
	}
	
};

#endif
