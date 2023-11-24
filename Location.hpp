#pragma once
#ifndef WORLDSIM_LOCATION_HPP
#define WORLDSIM_LOCATION_HPP

/* WorldSim: Location.hpp
	#include "Location.hpp"

	Abstract locations. Later will be built into physical locations.

*/

// Structures will require wood or stone to build.

// class Structure
// {
	// // FARM, SCHOOL, MINE SHAFT
	// public:
	// // ENUM TYPE

	
	// Structure()
	// {
		// capacity=0;
		// nIngress=0;
		// canBranch
	// }
// };

enum enumLocation
{
	LOCATION_UNKNOWN=0,
	LOCATION_OUTSIDE=1,
	LOCATION_WALLS=2,
	LOCATION_HALL=3,
	LOCATION_COUNT=4
};

class Location
{
	private:
		Vector <Character*> vCharacter; // Characters in this location.
		Vector <Location*> vLinkedLocations; // Locations you can travel to from here.
		
	public:
		enumLocation type;
		
		bool isOutside;
		
		int capacity; // How many people can occupy the location.
		int nIngress; // How many people can enter at once.
		int darkness; // Dark areas spawn enemies
		
		bool canBranch;
	

		Location()
		{
			isOutside=false;
			capacity=0;
			nIngress=0;
			darkness=0;
			canBranch=false;
		}
		
		void link(Location* location)
		{
			vLinkedLocations.push(location);
			location->vLinkedLocations.push(this);
		}
		
		void putCharacter(Character* c)
		{
			vCharacter.push(c);
		}
		
		virtual std::string getName()
		{
			return "unknown";
		}
			
};	


// Exterior of the settlement, basically the world tile without including the settlement.
class Location_Settlement_Exterior: public Location
{
	public:
		Location_Settlement_Exterior()
		{
			type=LOCATION_OUTSIDE;
			isOutside=true;
		}
		
		virtual std::string getName()
		{
			return "outside";
		}
};

// Defensive walls or perimeter of the Settlement
class Location_Settlement_Walls: public Location
{
	public:
		
		Location_Settlement_Walls()
		{
			type=LOCATION_WALLS;
		}
	
		virtual std::string getName()
		{
			return "walls";
		}
	
};

class Location_Main_Hall: public Location
{
	public:
		Location_Main_Hall()
		{
			type=LOCATION_HALL;
			canBranch=true;
		}
	
		virtual std::string getName()
		{
			return "main hall";
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

#include "Character.hpp"

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
			
			Location_Settlement_Walls* walls = new Location_Settlement_Walls();
			vLocation.push(walls);
			walls->link(exterior);
			
			Location_Main_Hall* hall = new Location_Main_Hall();
			vLocation.push(hall);
			hall->link(walls);

		}
		
		void putCharacter(Character* c, enumLocation location)
		{
			for (int i=0;i<vLocation.size();++i)
			{
				if ( vLocation(i)->type == location )
				{
					c->location = vLocation(i);
				}
			}
			
		}
		
};


#endif // WORLDSIM_LOCATION_HPP
