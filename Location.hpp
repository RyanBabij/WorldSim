#pragma once
#ifndef WORLDSIM_LOCATION_HPP
#define WORLDSIM_LOCATION_HPP

/* WorldSim: Location.hpp
	#include "Location.hpp"

	Abstract locations. Later will be built into physical locations.

*/

#include "Stockpile.hpp"

#include <string>
#include <vector>

class Character;

class Location: public HasResourceRequirement
{
private:
	Vector <Character*> vCharacter; // Characters in this location.
	Vector <Location*> vLinkedLocations; // Locations you can travel to from here.


public:
	enumLocation type;
	Vector <ItemType> vCanMake; // Items this location can make.

	bool isOutside;

	int capacity; // How many people can occupy the location. -1 = unlimited
	int nIngress; // How many people can enter at once.
	int darkness; // Dark areas spawn enemies

	int maxBranches;

	Location();

	void link(Location* location);
	bool putCharacter(Character* c);
	void removeCharacter(Character* c);
	virtual std::string getName();
	virtual ResourceRequirement getResourceRequirement();
	int availableBranches();
	
	bool hasRoom(); // returns true if there is room for another character here
};


// Far beyond the Settlement
class Location_Wilderness: public Location
{
public:
	Location_Wilderness();

	virtual std::string getName() override;
};

class Location_Settlement_Exterior: public Location
{
public:
	Location_Settlement_Exterior();

	virtual std::string getName() override;
};

class Location_Settlement_Walls: public Location
{
public:
	Location_Settlement_Walls();

	virtual std::string getName() override;
};

class Location_Main_Hall: public Location
{
public:
	Location_Main_Hall();

	virtual std::string getName() override;
};

class Location_Hall: public Location
{
public:
	Location_Hall();

	virtual std::string getName() override;
};

class Location_Dwelling: public Location
{
public:
	Location_Dwelling();

};

class Location_Mine: public Location
{
public:
	int size;
	int nIron;
	int nCopper;

	Location_Mine();

	virtual std::string getName() override;
};

class Location_Farm: public Location
{
public:

	Location_Farm();

	virtual std::string getName() override;
};

class Location_Weaponsmith: public Location
{
public:

	Location_Weaponsmith();

	virtual std::string getName() override;
};

class LocationManager
{
public:
	Vector <Location*> vLocation;
	
	World_Biome* biome;

	LocationManager();

	void buildDwarvenFortress();
	void putCharacter(Character* c, enumLocation location);
	Vector <Location*>* getLocation(enumLocation location);
	int totalAvailableBranches();
	Location* getBuildableBranch();
	int getMiningCapacity();
	int getFarmingCapacity();
	Location* addLocation(enumLocation locationType);
	bool has(enumLocation locationType);
	bool canBuild(enumLocation locationType);
	
	// Stores resource requirements for all locations in central location
	// Compiler will optimize it anyway
	static ResourceRequirement getResourceRequirement(const enumLocation locationType);
	
	Location* getLocationToMake(ItemType item);
	
	void printAll();
	std::string toString();
	
private:
	std::string locationTypeToString(enumLocation type)
	{
		return locationToString(type);
	}
	
};

#endif // WORLDSIM_LOCATION_HPP
