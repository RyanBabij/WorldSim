#pragma once
#ifndef WORLD_EVENT_HPP
#define WORLD_EVENT_HPP

/* WorldSim: World_Event.hpp
	#include "World_Event.hpp"

	Store notable events in the world.
	
*/

#include <Container/Table/TableInterface.hpp>

#include <string>
#include <vector>

class Event : public TableInterface
{
	public:

	enum EVENT_TYPE
	{
		EVENT_NONE,
		EVENT_WORLD_CREATED,
		EVENT_ITEM_CREATED,
		EVENT_TRIBAL_SPLIT,
		EVENT_TRIBE_BECOME_CIV,
		EVENT_SETTLEMENT_SPLIT,
		EVENT_NEW_LEADER,
		EVENT_DEITY_ACT,
		EVENT_TECH_BREAKTHROUGH
	};

	private:
		EVENT_TYPE eventType;
		std::vector<Event*> vEvent;

	public:
		std::string eventDescription;

		Event();
		Event(std::string _eventDescription, EVENT_TYPE _eventType);
		virtual ~Event();

		std::string getEventTypeStr() const;
		std::string getColumn(std::string _column) override;
		std::string getColumnType(std::string _column) override;
		 
		virtual std::string getLongDescription();
};


#endif