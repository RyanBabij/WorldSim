#pragma once
#ifndef WORLDSIM_LOCATION_CPP
#define WORLDSIM_LOCATION_CPP

/* WorldSim: Location.cpp
	#include "Location.cpp"
*/

#include "Location.hpp"

// Location class implementations
Location::Location()
{
	isOutside = false;
	capacity = 0;
	nIngress = 0;
	darkness = 0;
	maxBranches = 0;
}

void Location::link(Location* location)
{
	vLinkedLocations.push(location);
	location->vLinkedLocations.push(this);
}

bool Location::putCharacter(Character* c)
{
	if ( hasRoom() == false )
	{
		return false;
	}
	
	if ( c->location != nullptr )
	{
		c->location->removeCharacter(c);
	}
	
	vCharacter.push(c);
	c->location = this;
	return true;
}

bool Location::hasRoom()
{
	if ( capacity == -1 || capacity > vCharacter.size() )
	{
		return true;
	}
	return false;
}

void Location::removeCharacter(Character* c)
{
	vCharacter.remove(c);
	c->location = 0;
}

std::string Location::getName()
{
	return "unknown";
}

ResourceRequirement Location::getResourceRequirement()
{
	return LocationManager::getResourceRequirement(type);
}

int Location::availableBranches()
{
	return maxBranches - vLinkedLocations.size();
}


// Location_Settlement_Exterior class implementations
Location_Settlement_Exterior::Location_Settlement_Exterior()
{
	type = LOCATION_OUTSIDE;
	isOutside = true;
	maxBranches = 100;
	capacity = -1;
}

std::string Location_Settlement_Exterior::getName()
{
	return "outside";
}

Location_Wilderness::Location_Wilderness()
{
	type = LOCATION_WILDERNESS;
	isOutside = true;
	maxBranches = 4;
	capacity = -1;
}

std::string Location_Wilderness::getName()
{
	return "wilderness";
}

// Location_Settlement_Walls class implementations
Location_Settlement_Walls::Location_Settlement_Walls()
{
	type = LOCATION_WALLS;
	maxBranches = 2;
	capacity = 12;
}

std::string Location_Settlement_Walls::getName()
{
	return "walls";
}

// Location_Main_Hall class implementations
Location_Main_Hall::Location_Main_Hall()
{
	type = LOCATION_MAIN_HALL;
	maxBranches = 4;
	capacity = 50;
}

std::string Location_Main_Hall::getName()
{
	return "main hall";
}

// Location_Hall class implementations
Location_Hall::Location_Hall()
{
	type = LOCATION_HALL;
	maxBranches = 4;
	capacity = 25;
}

std::string Location_Hall::getName()
{
	return "hall";
}

// Location_Dwelling class implementations
Location_Dwelling::Location_Dwelling()
{
	type = LOCATION_DWELLING;
	maxBranches = 1;
}

// Location_Mine class implementations
Location_Mine::Location_Mine()
{
	type = LOCATION_MINE;
	maxBranches = 1;
	size = 0;
	nIron = 0;
	nCopper = 0;
	capacity=5;
}

std::string Location_Mine::getName()
{
	return "mine";
}

Location_Farm::Location_Farm()
{
	type = LOCATION_FARM;
	maxBranches = 1;
	capacity=3;
}

std::string Location_Farm::getName()
{
	return "farm";
}

Location_Weaponsmith::Location_Weaponsmith()
{
	type = LOCATION_WEAPONSMITH;
	maxBranches = 1;
	capacity=3;
}

std::string Location_Weaponsmith::getName()
{
	return "weaponsmith";
}

// LocationManager class implementations
LocationManager::LocationManager()
{
	// Constructor body (if needed)
	biome=0;
}

void LocationManager::buildDwarvenFortress()
{
	Location_Settlement_Exterior* exterior = new Location_Settlement_Exterior();
	vLocation.push(exterior);

	Location_Settlement_Walls* walls = new Location_Settlement_Walls();
	vLocation.push(walls);
	walls->link(exterior);

	Location_Main_Hall* hall = new Location_Main_Hall();
	vLocation.push(hall);
	hall->link(walls);
	
	Location_Wilderness* wild = new Location_Wilderness();
	vLocation.push(wild);
	wild->link(exterior);
}

void LocationManager::putCharacter(Character* c, enumLocation location)
{
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == location && vLocation(i)->putCharacter(c))
		{
		}
	}
}

Vector <Location*>* LocationManager::getLocation(enumLocation location)
{
	Vector <Location*>* vMatchLocation = new Vector <Location*>();
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == location)
		{
			vMatchLocation->push(vLocation(i));
		}
	}
	return vMatchLocation;
}

int LocationManager::totalAvailableBranches()
{
	int totalFreeBranches = 0;
	for (int i = 0; i < vLocation.size(); ++i)
	{
		totalFreeBranches += vLocation(i)->availableBranches();
	}
	return totalFreeBranches;
}

Location* LocationManager::getBuildableBranch()
{
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->availableBranches() > 0)
		{
			return vLocation(i);
		}
	}
	return nullptr;
}

int LocationManager::getMiningCapacity()
{
	int totalMiningCapacity = 0;
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == LOCATION_MINE)
		{
			totalMiningCapacity += static_cast<Location_Mine*>(vLocation(i))->capacity;
		}
	}
	return totalMiningCapacity;
}

int LocationManager::getFarmingCapacity()
{
	int totalFarmingCapacity = 0;
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == LOCATION_FARM)
		{
			totalFarmingCapacity += static_cast<Location_Farm*>(vLocation(i))->capacity;
		}
	}
	return totalFarmingCapacity;
}

Location* LocationManager::addLocation(enumLocation locationType)
{
	Location* buildable = getBuildableBranch();
	if (buildable == nullptr)
	{
		// No buildable branch available
		return nullptr;
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
		case LOCATION_FARM:
			newLocation = new Location_Farm();
			break;
		case LOCATION_WEAPONSMITH:
			newLocation = new Location_Weaponsmith();
			break;
		// Add cases for other location types
		default:
			// Handle unknown location type
			return nullptr;
	}

	if (newLocation != nullptr)
	{
		vLocation.push(newLocation);
		buildable->link(newLocation);
	}
	return newLocation;
}

bool LocationManager::has(enumLocation locationType)
{
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == locationType)
		{
			return true; // Location of the specified type exists
		}
	}
	return false; // No location of the specified type found
}

ResourceRequirement LocationManager::getResourceRequirement(enumLocation locationType)
{
	switch (locationType)
	{
		case LOCATION_OUTSIDE:
			return ResourceRequirement(/* Resources for LOCATION_OUTSIDE */);
		case LOCATION_WALLS:
			return ResourceRequirement(RESOURCE_STONE, 12); // Example requirement
		case LOCATION_MAIN_HALL:
			return ResourceRequirement(RESOURCE_STONE, 50); // Example requirement
		case LOCATION_HALL:
			return ResourceRequirement(RESOURCE_STONE, 25); // Example requirement
		case LOCATION_DWELLING:
			return ResourceRequirement(RESOURCE_STONE, 9); // Example requirement
		case LOCATION_MINE:
			return ResourceRequirement(/* Resources for LOCATION_MINE */);
		case LOCATION_FARM:
			return ResourceRequirement(/* Resources for LOCATION_FARM */);
		// Add cases for other location types
		default:
			return ResourceRequirement( /* NONE */ ); // Return empty requirement for unknown types
	}

}


std::string LocationManager::toString()
{
	std::map<enumLocation, int> locationCounts;
	std::stringstream ss;

	// Count the occurrences of each location type
	for (int i = 0; i < vLocation.size(); ++i)
	{
		Location* loc = vLocation(i);
		if (loc != nullptr)
		{
			locationCounts[loc->type]++;
		}
	}

	// Iterate through the map and build the string
	for (const auto& pair : locationCounts)
	{
		std::string locationName = locationTypeToString(pair.first);
		ss << locationName << " x" << pair.second << "\n";
	}

	return ss.str();
}


void LocationManager::printAll()
{
	std::map<enumLocation, int> locationCounts;

	// Count the occurrences of each location type
	for (int i = 0; i < vLocation.size(); ++i)
	{
		Location* loc = vLocation(i);
		if (loc != nullptr)
		{
			locationCounts[loc->type]++;
		}
	}

	// Iterate through the map and print the results
	for (const auto& pair : locationCounts)
	{
		std::string locationName = locationTypeToString(pair.first);
		std::cout << locationName << " x" << pair.second << "\n";
	}
}

#endif // WORLDSIM_LOCATION_CPP
