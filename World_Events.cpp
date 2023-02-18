#pragma once
#ifndef WORLD_EVENTS_CPP
#define WORLD_EVENTS_CPP

/* WorldSim: World_Events.cpp
	#include "World_Events.cpp"

	Store notable events in the world.
	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include <Container/Table/TableInterface.hpp>

class Event: public TableInterface
{
	public:
	
	std::string eventDescription;
	
	Event()
	{
		eventDescription="?EVENT?";
	}
	Event(std::string _eventDescription)
	{
		eventDescription = _eventDescription;
	}
	virtual ~Event()
	{
	}
	
	/* TABLE INTERFACE */
	std::string getColumn(std::string _column)
	{
		return eventDescription;
	}
	std::string getColumnType(std::string _column)
	{
		// if ( _column == "age" || _column == "territory" || _column == "food" || _column == "strength" || _column == "kills" )
		// {
			// return "number";
		// }
		return "string";
	}
};

class EventsManager
{
	public:
	
	Vector <Event*> vEvent;

	EventsManager()
	{
	}
	
	void clear()
	{
		vEvent.clearPtr();
	}

	void addEvent(std::string eventDescription)
	{
		vEvent.push(new Event(eventDescription));
	}
	
	Event* get(int i)
	{
		if (vEvent.isSafe(i))
		{
			return vEvent(i);
		}
		return 0;
	}
	
	int size()
	{
		return vEvent.size();
	}
	
};


#endif
