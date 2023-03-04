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
	enum EVENT_TYPE { EVENT_NONE, EVENT_WORLD_CREATED, EVENT_ITEM_CREATED };
	EVENT_TYPE eventType;
	
	Event()
	{
		eventDescription="?EVENT?";
		eventType=EVENT_NONE;
	}
	Event(std::string _eventDescription, EVENT_TYPE _eventType)
	{
		eventDescription = _eventDescription;
		eventType = _eventType;
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
	
	std::string getLongDescription()
	{
		if (eventType==EVENT_WORLD_CREATED)
		{
			return "On the year 0 the world was created. This has made a lot of people very angry and been widely regarded as a bad move.";
		}
		return "EVENT DESCRIPTION";
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

	void addEvent(std::string eventDescription, Event::EVENT_TYPE _eventType)
	{
		vEvent.push(new Event(eventDescription, _eventType));
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
