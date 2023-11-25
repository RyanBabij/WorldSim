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

void Location::putCharacter(Character* c)
{
	vCharacter.push(c);
	c->location = this;
}

std::string Location::getName()
{
	return "unknown";
}

ResourceRequirement Location::getResourceRequirement()
{
	return ResourceRequirement(0, 0);
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
}

std::string Location_Settlement_Exterior::getName()
{
	return "outside";
}

ResourceRequirement Location_Settlement_Exterior::getResourceRequirement()
{
	return ResourceRequirement(0, 0);
}

// Location_Settlement_Walls class implementations
Location_Settlement_Walls::Location_Settlement_Walls()
{
	type = LOCATION_WALLS;
	maxBranches = 2;
}

std::string Location_Settlement_Walls::getName()
{
	return "walls";
}

ResourceRequirement Location_Settlement_Walls::getResourceRequirement()
{
	return ResourceRequirement(0, 0);
}

// Location_Main_Hall class implementations
Location_Main_Hall::Location_Main_Hall()
{
	type = LOCATION_MAIN_HALL;
	maxBranches = 4;
}

std::string Location_Main_Hall::getName()
{
	return "main hall";
}

ResourceRequirement Location_Main_Hall::getResourceRequirement()
{
	return ResourceRequirement(0, 25);
}

// Location_Hall class implementations
Location_Hall::Location_Hall()
{
	type = LOCATION_HALL;
	maxBranches = 4;
}

std::string Location_Hall::getName()
{
	return "hall";
}

ResourceRequirement Location_Hall::getResourceRequirement()
{
	return ResourceRequirement(0, 25);
}

// Location_Dwelling class implementations
Location_Dwelling::Location_Dwelling()
{
	type = LOCATION_DWELLING;
	maxBranches = 1;
}

ResourceRequirement Location_Dwelling::getResourceRequirement()
{
	return ResourceRequirement(0, 9);
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

ResourceRequirement Location_Mine::getResourceRequirement()
{
	return ResourceRequirement(0, 0);
}

std::string Location_Mine::getName()
{
	return "mine";
}

// LocationManager class implementations
LocationManager::LocationManager()
{
	// Constructor body (if needed)
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
}

void LocationManager::putCharacter(Character* c, enumLocation location)
{
	for (int i = 0; i < vLocation.size(); ++i)
	{
		if (vLocation(i)->type == location)
		{
			c->location = vLocation(i);
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

void LocationManager::addLocation(enumLocation locationType)
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

void LocationManager::printAll()
{
	for (int i = 0; i < vLocation.size(); ++i)
	{
		Location* loc = vLocation(i);
		if (loc != nullptr)
		{
			std::cout<<"Location: "<<loc->getName()<<"\n";
		}
	}
}


#endif // WORLDSIM_LOCATION_CPP
