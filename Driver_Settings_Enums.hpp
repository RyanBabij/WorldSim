#pragma once
#ifndef WORLDSIM_DRIVER_SETTINGS_ENUMS_CPP
#define WORLDSIM_DRIVER_SETTINGS_ENUMS_CPP

/* WorldSim: Driver_Settings_Enums
#include "Driver_Settings_Enums.cpp"

	Lists of values go here. Functions and sets related to the enums also go here.
*/

#include <Container/Vector/Vector.hpp>

#include <unordered_map>


// MAIN MENU ENUMS
enum enumMenu
{
	MENU_UNKNOWN,
	MENU_TITLE,
	MENU_OPTIONS,
	MENU_LOADGAME,
	MENU_WORLDGENERATOR,
	MENU_WORLDSIMULATOR,
	MENU_ADVENTUREMODE
};

// Should be MENU_TITLE but I'm bypassing it for now because the main menu has no function atm.
enumMenu activeMenu = MENU_WORLDGENERATOR;

// DIRECTION ENUMS
enum enumDirection
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

// RACE ENUM
enum enumRace
{
	NONE=0,
	HUMAN=1,
	DWARVEN=2,
	ELVEN=3
};

// BASE TERRAIN ENUM (I DON'T KNOW IF THIS IS BEING USED)
enum class eLocalTerrain
{
	G=0,
	GRASS=1,
	WATER=2
};

enum enumCauseOfDeath
{
	UNKNOWN=0,
	STARVATION=1,
	MASSACRE=2,
	COMBAT=3,
	OLD_AGE=4,
	SMITED=5
};

const std::string enumCauseOfDeathStr [5] =
{
	"unknown",
	"starvation",
	"massacre",
	"combat",
	"old age"
};

enum enumLocation
{
	LOCATION_UNKNOWN,
	LOCATION_NONE,
	LOCATION_OUTSIDE,
	LOCATION_WALLS,
	LOCATION_HALL,
	LOCATION_MAIN_HALL,
	LOCATION_MINE,
	LOCATION_DWELLING,
	LOCATION_FARM,
	LOCATION_WILDERNESS,
	LOCATION_WEAPONSMITH,
	LOCATION_COUNT
};

const std::string enumLocationStr[LOCATION_COUNT] =
{
	"unknown",
	"none",
	"outside",
	"walls",
	"hall",
	"main hall",
	"mine",
	"dwelling",
	"farm",
	"wilderness"
};

// Function to convert enumLocation to a string
const std::string locationToString(enumLocation location)
{
	switch (location)
	{
		case LOCATION_NONE: return "None";
		case LOCATION_OUTSIDE: return "Outside";
		case LOCATION_WALLS: return "Walls";
		case LOCATION_HALL: return "Hall";
		case LOCATION_MAIN_HALL: return "Main Hall";
		case LOCATION_MINE: return "Mine";
		case LOCATION_DWELLING: return "Dwelling";
		case LOCATION_FARM: return "Farm";
		case LOCATION_WILDERNESS: return "Wilderness";
		case LOCATION_WEAPONSMITH: return "Weaponsmith";
		default: return "Unknown";
	}
}

enum enumResource
{
	RESOURCE_UNKNOWN,
	RESOURCE_IRON,
	RESOURCE_GOLD,
	RESOURCE_SILVER,
	RESOURCE_GEM,
	RESOURCE_DIAMOND,
	RESOURCE_ADAMANTINE,
	RESOURCE_COAL,
	RESOURCE_STONE,
	RESOURCE_WOOD,
	RESOURCE_MAGICKA_CRYSTAL,
	RESOURCE_STEEL,
	RESOURCE_BRONZE,
	RESOURCE_ADAMANTIUM,
	RESOURCE_FOOD,
	RESOURCE_COUNT
};

const std::string enumResourceStr[RESOURCE_COUNT] =
{
	"unknown",
	"iron",
	"gold",
	"silver",
	"gem",
	"diamond",
	"adamantine",
	"coal",
	"stone",
	"wood",
	"magicka crystal",
	"steel",
	"bronze",
	"adamantium",
	"food"
};

enum enumIntermediate
{
	INTERMEDIATE_NONE,
	INTERMEDIATE_FIBRE,
	INTERMEDIATE_GUT,
	INTERMEDIATE_MEAT,
	INTERMEDIATE_BONE,
	INTERMEDIATE_COUNT
};

const std::string enumIntermediateStr[INTERMEDIATE_COUNT] =
{
	"none",
	"fibre",
	"gut",
	"meat",
	"bone"
};

const std::string intermediateToString(enumIntermediate intermediate)
{
	if (intermediate >= 0 && intermediate < INTERMEDIATE_COUNT)
	{
		return enumIntermediateStr[intermediate];
	}
	return "Unknown";
}


const std::string resourceToString(enumResource resource)
{
	switch (resource)
	{
		case RESOURCE_FOOD: return "Food";
		case RESOURCE_IRON: return "Iron";
		case RESOURCE_GOLD: return "Gold";
		case RESOURCE_SILVER: return "Silver";
		case RESOURCE_GEM: return "Gem";
		case RESOURCE_DIAMOND: return "Diamond";
		case RESOURCE_ADAMANTINE: return "Adamantine";
		case RESOURCE_COAL: return "Coal";
		case RESOURCE_STONE: return "Stone";
		case RESOURCE_WOOD: return "Wood";
		case RESOURCE_MAGICKA_CRYSTAL: return "Magicka Crystal";
		case RESOURCE_STEEL: return "Steel";
		case RESOURCE_BRONZE: return "Bronze";
		case RESOURCE_ADAMANTIUM: return "Adamantium";
		default: return "Unknown";
	}
}

enum enumJob
{
	JOB_UNKNOWN,
	JOB_FARMING,
	JOB_MINING,
	JOB_PRODUCTION,
	JOB_WOODCUTTING,
	JOB_PROSPECTING,
	JOB_HUNTING,
	JOB_COUNT
};

const std::string enumJobStr[JOB_COUNT] =
{
	"unknown",
	"farming",
	"mining",
	"production",
	"woodcutting",
	"prospecting",
	"hunting"
};

// Item Category needs to be changed to Item Action, and then Item objects need to have a table of their value at each
// action.

// Melee hunting - Can only catch certain animals which can be ambushed or will approach

enum ItemAction
{
	ITEM_ACTION_NONE,
	ITEM_ACTION_HUNTING_MELEE,
	ITEM_ACTION_HUNTING_RANGED,
	ITEM_ACTION_FARMING,
	ITEM_ACTION_WOODCUTTING,
	ITEM_ACTION_MINING,
	ITEM_ACTION_COUNT
};

const std::string actionToString(ItemAction action)
{
	switch (action)
	{
		case ITEM_ACTION_NONE: return "None";
		case ITEM_ACTION_HUNTING_MELEE: return "Hunting melee";
		case ITEM_ACTION_HUNTING_RANGED: return "Hunting ranged";
		case ITEM_ACTION_FARMING: return "Farming";
		// Add more cases as needed
		default: return "Unknown";
	}
}


enum ItemType
{
	ITEM_NONE,
	ITEM_HAND,
	ITEM_HOE,
	ITEM_SWORD,
	ITEM_KNIFE,
	ITEM_LONGBOW,
	ITEM_SPEAR,
	ITEM_PICKAXE,
	ITEM_FISHROD,
	ITEM_SHOVEL,
	ITEM_AXE,
	ITEM_LOG,
	ITEM_FISH,
	ITEM_CAMPFIRE,
	ITEM_PLANK,
	ITEM_WALL,
	ITEM_FLOOR,
	ITEM_PLANTFIBRE,
	ITEM_DEERPELT,
	ITEM_DEERMEAT,
	ITEM_HUMANSKIN,
	ITEM_HUMANMEAT,
	ITEM_LEATHERCLOTHES,
	ITEM_WATERSKIN,
	ITEM_SHELTER,
	ITEM_MARKER_RED,
	ITEM_MARKER_GREEN_SMALL,
	ITEM_COUNT
};

const std::string enumItemTypeStr[ITEM_COUNT] =
{
	"none",
	"hand",
	"hoe",
	"sword",
	"knife",
	"longbow",
	"spear",
	"pickaxe",
	"fishrod",
	"shovel",
	"axe",
	"log",
	"fish",
	"campfire",
	"plank",
	"wall",
	"floor",
	"plantfibre",
	"deerpelt",
	"deermeat",
	"humanskin",
	"humanmeat",
	"leatherclothes",
	"waterskin",
	"shelter",
	"marker_red",
	"marker_green_small"
};

const std::string itemToString(ItemType itemType)
{
	switch (itemType)
	{
		case ITEM_NONE: return "None";
		case ITEM_HAND: return "Hand";
		case ITEM_HOE: return "Hoe";
		case ITEM_SWORD: return "Sword";
		case ITEM_KNIFE: return "Knife";
		case ITEM_LONGBOW: return "Longbow";
		case ITEM_SPEAR: return "Spear";
		case ITEM_PICKAXE: return "Pickaxe";
		case ITEM_FISHROD: return "Fishrod";
		case ITEM_SHOVEL: return "Shovel";
		case ITEM_AXE: return "Axe";
		case ITEM_LOG: return "Log";
		case ITEM_FISH: return "Fish";
		case ITEM_CAMPFIRE: return "Campfire";
		case ITEM_PLANK: return "Plank";
		case ITEM_WALL: return "Wall";
		case ITEM_FLOOR: return "Floor";
		case ITEM_PLANTFIBRE: return "Plantfibre";
		case ITEM_DEERPELT: return "Deerpelt";
		case ITEM_DEERMEAT: return "Deermeat";
		case ITEM_HUMANSKIN: return "Humanskin";
		case ITEM_HUMANMEAT: return "Humanmeat";
		case ITEM_LEATHERCLOTHES: return "Leatherclothes";
		case ITEM_WATERSKIN: return "Waterskin";
		case ITEM_SHELTER: return "Shelter";
		case ITEM_MARKER_RED: return "Marker Red";
		case ITEM_MARKER_GREEN_SMALL: return "Marker Green Small";
		// Add more cases as needed
		default: return "Unknown";
	}
}

// Define a mapping from ItemType to ItemAction
std::unordered_map<ItemType, ItemAction> ItemActionMap =
{
	{ITEM_HOE, ITEM_ACTION_FARMING},
	{ITEM_SWORD, ITEM_ACTION_HUNTING_MELEE},
	{ITEM_KNIFE, ITEM_ACTION_HUNTING_MELEE},
	{ITEM_LONGBOW, ITEM_ACTION_HUNTING_RANGED},
	// ... Add other item to category mappings
};

// Function to determine if an item is in a particular category
bool isItemInAction(ItemType itemType, ItemAction ItemAction)
{
	auto it = ItemActionMap.find(itemType);
	if (it != ItemActionMap.end())
	{
		return it->second == ItemAction;
	}
	return false; // Item type not found in the map, or not associated with any category
}

// Function to determine the category of an item
ItemAction getActionOfItem(ItemType itemType)
{
	auto it = ItemActionMap.find(itemType);
	if (it != ItemActionMap.end())
	{
		return it->second; // Return the found category
	}
	return ITEM_ACTION_NONE; // Return default category if not found
}

// Assuming ItemType and ItemAction are defined enums
// and ItemActionMap is a predefined map from ItemType to ItemAction

Vector <ItemType> getItemsInAction(ItemAction category)
{
	Vector<ItemType> itemsInCategory;

	for (const auto& itemPair : ItemActionMap)
	{
		if (itemPair.second == category)
		{
			itemsInCategory.push(itemPair.first);
		}
	}

	return itemsInCategory;
}



#define COUT_COLOURS

// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47

enum enumColour
{
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	DEFAULT
};

std::unordered_map<enumColour, std::string> colourMap =
{
	{BLACK,   "\033[30m"},
	{RED,     "\033[31m"},
	{GREEN,   "\033[32m"},
	{YELLOW,  "\033[33m"},
	{BLUE,    "\033[34m"},
	{MAGENTA, "\033[35m"},
	{CYAN,    "\033[36m"},
	{WHITE,   "\033[37m"},
	{DEFAULT,   "\033[0m"}
};

void colour(enumColour colour)
{
	#ifdef COUT_COLOURS
	auto it = colourMap.find(colour);
	if (it != colourMap.end())
	{
		// Set colour
		std::cout << it->second;
	}
	#endif
}



#endif // WORLDSIM_DRIVER_SETTINGS_ENUMS_CPP
