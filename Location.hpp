#pragma once
#ifndef WORLDSIM_LOCATION_HPP
#define WORLDSIM_LOCATION_HPP

/* WorldSim: Location.hpp
	#include "Location.hpp"

	Abstract locations. Later will be built into physical locations.

*/

// Structures will require wood or stone to build.

class Structure
{
	// FARM, SCHOOL, MINE SHAFT
	public:
	// ENUM TYPE
	int capacity; // How many people can occupy the location.
	int nIngress; // How many people can enter at once.
	
	Structure()
	{
		capacity=0;
	}
};

class Location
{
	private:
		Vector <Character*> vCharacter; // Characters in this location.
		Vector <Location*> vLinkedLocations; // Locations you can travel to from here.
		
		bool isOutside;
		bool isWilderness;
	
	public:
		Location()
		{
		}
		
		void link(Location* location)
		{
			vLinkedLocations.push(location);
			location->vLinkedLocations.push(this);
		}
			
};	


// Exterior of the settlement, basically the world tile without including the settlement.
class Location_Settlement_Exterior: public Location
{
	public:
	Location_Settlement_Exterior()
	{
	}
};

// Defensive walls or perimeter of the Settlement
class Location_Settlement_Walls: public Location
{
	public:
	
	Location_Settlement_Walls()
	{
	}
	
};

class Location_Main_Hall: public Location
{
	public:
	Location_Main_Hall()
	{
	}
};

class Location_Dwelling: public Location
{
	public:
	Location_Dwelling()
	{
	}
};

class Location_Mine: public Location
{
	public:
	
	int size;
	int nIron;
	int nCopper;
	
	
	Location_Mine()
	{
	}
};



class LocationManager
{
	public:
		Vector <Location*> vLocation;
	
		enum LOCATION_TYPE
		{
			FARM,
			FACTORY,
			HUNTING,
			GUARDING,
			HALL,
			MINES,
			SCHOOL,
			DUNGEON_RAID,
			EXPLORING,	
			ENUM_COUNT // Useful for iterating over the enum or generating random values
		};
	 
		LocationManager()
		{
		}
		
		void buildDwarvenFortress()
		{
			Location_Settlement_Exterior* exterior = new Location_Settlement_Exterior();
			vLocation.push(exterior);
			//vLocation.push(new Location_Settlement_Exterior());
			//vLocation.push(new Location_Settlement_Exterior());
		}
		
};


#endif // WORLDSIM_LOCATION_HPP
