#pragma once
#ifndef WORLD_EVENTS_CPP
#define WORLD_EVENTS_CPP

/* WorldSim: World_Events.cpp
	#include "World_Events.cpp"

	Store notable events in the world.
	
*/

#include "World_Event.hpp"

#include "Settlement.hpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

Event::Event(): eventType(EVENT_NONE), eventDescription("?EVENT?")
{}

Event::Event(std::string _eventDescription, EVENT_TYPE _eventType):
eventType(_eventType), eventDescription(std::move(_eventDescription))
{}

Event::~Event()
{
    // Proper cleanup of vEvent if necessary
}

std::string Event::getEventTypeStr() const
{
	switch (eventType)
	{
		case EVENT_NONE:
			return "NONE (PLACEHOLDER)";
		case EVENT_SETTLEMENT_SPLIT:
			return "SETTLEMENT SPLIT";
		case EVENT_WORLD_CREATED:
			return "WORLD CREATED";
		case EVENT_NEW_LEADER:
			return "NEW LEADER";
		case EVENT_DEITY_ACT:
			return "DEITY ACT";
		default:
			return "??? EVENT TYPE ???";
	}
}

std::string Event::getColumn(std::string _column)
{
	if (_column == "date")
	{
		return "date"; // Placeholder for actual date handling
	}
	else if (_column == "type")
	{
		return getEventTypeStr();
	}
	else if (_column == "description")
	{
		return eventDescription;
	}
	return "<EVENT UNKNOWN COLUMN>";
}

std::string Event::getColumnType(std::string _column)
{
	// Placeholder for actual type handling
	return "string";
}

std::string Event::getLongDescription()
{
	switch (eventType)
	{
		case EVENT_WORLD_CREATED:
			return "On the year 0 the world was created. This has made a lot of people very angry and been widely regarded as a bad move.";
		case EVENT_SETTLEMENT_SPLIT:
			return "On the year X some Dwarves from the settlement of Y decided to leave and attempt to create their own settlement.";
		default:
			return "??? EVENT DESCRIPTION GOES HERE ???";
	}
}

class Event_Settlement_Split: public Event
{
	// Reasons for Settlement Split:
	// * Overpopulation
	// * etc
	
	Settlement* baseSettlement;
	Tribe* splittingTribe;
	
	public:
	
	Event_Settlement_Split(std::string _eventDescription, EVENT_TYPE _eventType, Settlement* _baseSettlement, Tribe* _splittingTribe): Event(_eventDescription, _eventType), baseSettlement(_baseSettlement), splittingTribe(_splittingTribe)
	{
	}
	std::string getLongDescription()
	{
		return "On the year X some Dwarves from the settlement of " + baseSettlement->getName() + " decided to leave and attempt to create their own settlement.";
	}
};

class Event_World_Created: public Event
{
	// Event when a Deity creates the world.
	
	public:
	
	Event_World_Created()
	{
	}
	
	
	
	
	
};

class EventManager
{
	public:
	
	Vector <Event*> vEvent;

	EventManager()
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
	
	void addEvent(Event* _event)
	{
		vEvent.push(_event);
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
