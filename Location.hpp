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


#include "Resource.cpp"


class Location: public HasResourceRequirement
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
		
		int maxBranches;
	

		Location()
		{
			isOutside=false;
			capacity=0;
			nIngress=0;
			darkness=0;
			maxBranches=0;
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
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,0);
		}
		
		int availableBranches()
		{
			return maxBranches - vLinkedLocations.size();
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
			maxBranches=100;
		}
		
		virtual std::string getName()
		{
			return "outside";
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,0);
		}
};

// Defensive walls or perimeter of the Settlement
class Location_Settlement_Walls: public Location
{
	public:
		
		Location_Settlement_Walls()
		{
			type=LOCATION_WALLS;
			maxBranches=2; // Walls only branch between inside and outside the location.
		}
	
		virtual std::string getName()
		{
			return "walls";
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,0);
		}
	
};

class Location_Main_Hall: public Location
{
	public:
		Location_Main_Hall()
		{
			type=LOCATION_MAIN_HALL;
			maxBranches=4;
		}
	
		virtual std::string getName()
		{
			return "main hall";
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,25);
		}
};

class Location_Hall: public Location
{
	public:
		Location_Hall()
		{
			type=LOCATION_HALL;
			maxBranches=4;
		}
	
		virtual std::string getName()
		{
			return "hall";
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,25);
		}
};

class Location_Dwelling: public Location
{
	public:
		Location_Dwelling()
		{
			type=LOCATION_DWELLING;
			// Dwellings should only branch to a hall.
			maxBranches=1;
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,9);
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
			type=LOCATION_MINE;
			// Mine shafts can only branch off from a hall.
			maxBranches=1;
		}
		
		virtual ResourceRequirement getResourceRequirement()
		{
			return ResourceRequirement(0,0);
		}
};

#include "Character.hpp"

class LocationManager
{
	public:
		Vector <Location*> vLocation;
	
		// enum LOCATION_TYPE
		// {
			// FARM,
			// FACTORY,
			// HUNTING,
			// GUARDING,
			// HALL,
			// MINES,
			// SCHOOL,
			// DUNGEON_RAID,
			// EXPLORING,	
			// ENUM_COUNT // Useful for iterating over the enum or generating random values
		// };
	 
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
		
		Vector <Location*>* getLocation(enumLocation location)
		{
			Vector <Location*>* vMatchLocation = new Vector <Location*>();
			for (int i=0;i<vLocation.size();++i)
			{
				if ( vLocation(i)->type == location )
				{
					vMatchLocation->push(vLocation(i));
				}
			}
			return vMatchLocation;
		}
		
		
		int totalAvailableBranches()
		{
			int totalFreeBranches = 0;
			for (int i=0; i<vLocation.size(); ++i)
			{
				totalFreeBranches += vLocation(i)->availableBranches();
			}
			return totalFreeBranches;
		}
		
		// Get location with a free branch to build off.
		Location* getBuildableBranch()
		{
			for (int i=0; i<vLocation.size(); ++i)
			{
				if (vLocation(i)->availableBranches() > 0)
				{
					// can build here
					return vLocation(i);
				}
			}
			return nullptr;
		}
		
		void addLocation(enumLocation locationType)
		{
			Location* buildable = getBuildableBranch();
			if (buildable == nullptr)
			{
				// No buildable branch available
				return;
			}

			Location* newLocation = nullptr;

			switch (locationType)
			{
				case LOCATION_OUTSIDE:
					newLocation = new Location_Settlement_Exterior();
					break;
				case LOCATION_WALLS:
					newLocation = new Location_Settlement_Walls();
					break;
				case LOCATION_MAIN_HALL:
					newLocation = new Location_Main_Hall();
					break;
				case LOCATION_HALL:
					newLocation = new Location_Hall();
					break;
				case LOCATION_DWELLING:
					newLocation = new Location_Dwelling();
					break;
				case LOCATION_MINE:
					newLocation = new Location_Mine();
					break;
				// Add cases for other location types
				default:
					// Handle unknown location type
					return;
			}

			if (newLocation != nullptr)
			{
				vLocation.push(newLocation);
				buildable->link(newLocation);
			}
		}

		
};


#endif // WORLDSIM_LOCATION_HPP
