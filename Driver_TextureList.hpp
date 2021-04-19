#pragma once
#ifndef WORLDSIM_DRIVER_TEXTURE_LIST_HPP
#define WORLDSIM_DRIVER_TEXTURE_LIST_HPP

#include <string>

#define THREADED_TEXTURE_LOADING
#if defined WILDCAT_THREADING || defined THREADED_TEXTURE_LOADING
	#include <thread>
#endif

#include <Graphics/Texture/Texture.hpp>

#include <string>

/* WorldSim: Driver_TextureList.hpp
#include "Driver_TextureList.hpp"

	Declare textures for classes that need them.
*/

static Texture TEX_TITLE;
static Texture TEX_OPTIONS_BACKGROUND;
static Texture TEX_LOAD_GAME_BACKGROUND;
static Texture TEX_NEW_GAME_BACKGROUND;

static Texture TEX_GUI_INCREMENT;
static Texture TEX_GUI_DECREMENT;
static Texture TEX_GUI_EXPAND;
static Texture TEX_GUI_TILE_SELECTION;


static Texture TEX_GUI_MINIMAP_BASE_TEXTUREMODE;
const std::string PATH_TEX_GUI_MINIMAP_BASE_TEXTUREMODE = "Textures/GUI/Minimap/Base_TextureMode.png";
static Texture TEX_GUI_MINIMAP_BASE_LANDMODE;
const std::string PATH_TEX_GUI_MINIMAP_BASE_LANDMODE = "Textures/GUI/Minimap/Base_LandMode.png";

Texture TEX_GUI_CENTER_CAMERA;
const std::string PATH_TEX_GUI_CENTER_CAMERA = "Textures/GUI/CenterCamera.png";

Texture TEX_NPC;
Texture TEX_NPC_DED;

Texture TEX_TEST_404;
const std::string PATH_TEX_TEST_404 = "Textures/Test/404.png";

Texture TEX_WORLD_ARTIFACT_GEMS;
const std::string PATH_TEX_WORLD_ARTIFACT_GEMS = "Textures/World/Artifact/Gems2.png";

Texture TEX_WORLD_ARTIFACT_BATTLE;
const std::string PATH_TEX_WORLD_ARTIFACT_BATTLE = "Textures/World/Artifact/BattleSite.png";


//GUI
Texture TEX_GUI_TILE_SELECTION_FULL;
const std::string PATH_TEX_GUI_TILE_SELECTION_FULL = "Textures/GUI/Grey33A.png";

// WORLD UNIT TEXTURES
Texture TEX_WORLD_UNIT_NOMAD_01;
const std::string PATH_TEX_WORLD_UNIT_NOMAD_01 = "Textures/World/Unit/Nomad/00.png";

Texture TEX_WORLD_UNIT_DWARF_01;
const std::string PATH_TEX_WORLD_UNIT_DWARF_01 = "Textures/World/Unit/Dwarf/00.png";

Texture TEX_WORLD_UNIT_ELF_01;
const std::string PATH_TEX_WORLD_UNIT_ELF_01 = "Textures/World/Unit/Elf/00.png";

Texture TEX_WORLD_SETTLEMENT_DWARFFORT_01;
const std::string PATH_TEX_WORLD_SETTLEMENT_DWARFFORT_01 = "Textures/World/Settlement/DwarfFort/03.png";

Texture TEX_WORLD_SETTLEMENT_TOWN_URBAN01;
const std::string PATH_TEX_WORLD_SETTLEMENT_TOWN_URBAN01 = "Textures/World/Settlement/Town/01.png";

Texture TEX_WORLD_TERRAIN_GRASS_00;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_00 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_GRASS_01;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_01 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_GRASS_02;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_02 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_GRASS_03;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_03 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_GRASS_04;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_04 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_GRASS_05;
const std::string PATH_TEX_WORLD_TERRAIN_GRASS_05 = "Textures/World/Terrain/Grass/05.png";

Texture TEX_WORLD_TERRAIN_RIVER_FULL;
Texture TEX_WORLD_TERRAIN_RIVER_EAST;
Texture TEX_WORLD_TERRAIN_RIVER_SOUTH;
Texture TEX_WORLD_TERRAIN_RIVER_WEST;
Texture TEX_WORLD_TERRAIN_RIVER_NORTH;
const std::string PATH_TEX_WORLD_TERRAIN_RIVER_EAST = "Textures/World/Terrain/River/River-East.png";
const std::string PATH_TEX_WORLD_TERRAIN_RIVER_FULL = "Textures/World/Terrain/River/River-Full.png";

Texture TEX_ALCHEMY;
const std::string PATH_TEX_ALCHEMY = "Textures/World/Terrain/Alchemy/05.png";

Texture TEX_WORLD_TERRAIN_OCEAN_00;
const std::string PATH_TEX_WORLD_TERRAIN_OCEAN_00 = "Textures/World/Terrain/Ocean/01.png";

Texture TEX_WORLD_TERRAIN_ICE;
const std::string PATH_TEX_WORLD_TERRAIN_ICE = "Textures/World/Terrain/Ice/Ice.png";

Texture TEX_WORLD_TERRAIN_SNOW;
const std::string PATH_TEX_WORLD_TERRAIN_SNOW = "Textures/World/Terrain/Snow/Snow.png";

Texture TEX_WORLD_TERRAIN_STEPPE;
const std::string PATH_TEX_WORLD_TERRAIN_STEPPE = "Textures/World/Terrain/Steppe/Steppe.png";

Texture TEX_WORLD_TERRAIN_SWAMP;
const std::string PATH_TEX_WORLD_TERRAIN_SWAMP = "Textures/World/Terrain/Wetland/Swamp.png";



Texture TEX_WORLD_TERRAIN_FOREST_TREES;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_TREES = "Textures/World/Terrain/Forest/00.png";

Texture TEX_WORLD_TERRAIN_JUNGLE;
const std::string PATH_TEX_WORLD_TERRAIN_JUNGLE = "Textures/World/Terrain/Jungle/01.png";

// FLORA
Texture TEX_WORLD_TERRAIN_FOREST_TREE;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_TREE = "Textures/World/Terrain/Forest/tree.png";

Texture TEX_WORLD_TERRAIN_FOREST_SAPLING;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_SAPLING = "Textures/World/Terrain/Forest/Sapling.png";

Texture TEX_FLORA_PLANT;
const std::string PATH_TEX_FLORA_PLANT = "Textures/Flora/Plant-small.png";

Texture TEX_FLORA_BLACKWEED;
const std::string PATH_TEX_FLORA_BLACKWEED = "Textures/Flora/Blackweed/Blackweed.png";

Texture TEX_FLORA_REDWEED;
const std::string PATH_TEX_FLORA_REDWEED = "Textures/Flora/Redweed/Redweed.png";

Texture TEX_FLORA_BLUEWEED;
const std::string PATH_TEX_FLORA_BLUEWEED = "Textures/Flora/Blueweed/Blueweed.png";

Texture TEX_FLORA_RED_MUSHROOM;
const std::string PATH_TEX_FLORA_RED_MUSHROOM = "Textures/Flora/RedMushroom/RedMushroom.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_00;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_00 = "Textures/World/Terrain/Mountain/01.png";

Texture TEX_WORLD_TERRAIN_HILL;
const std::string PATH_TEX_WORLD_TERRAIN_HILL = "Textures/World/Terrain/Mountain/Hills.png";

Texture TEX_WORLD_TERRAIN_UNDERGROUND_00;
const std::string PATH_TEX_WORLD_TERRAIN_UNDERGROUND_00 = "Textures/World/Terrain/Underground/00.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI = "Textures/World/Terrain/Mountain/Rockyboi.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD = "Textures/World/Terrain/Mountain/RockyboiGold.png";

Texture TEX_CREATURE_DEER;
const std::string PATH_TEX_CREATURE_DEER = "Textures/Creature/Deer/Deer.png";
Texture TEX_CREATURE_DEER_DED;
const std::string PATH_TEX_CREATURE_DEER_DED = "Textures/Creature/Deer/Deer-ded.png";

Texture TEX_CREATURE_DOE;
const std::string PATH_TEX_CREATURE_DOE = "Textures/Creature/Deer/Doe.png";
Texture TEX_CREATURE_DOE_DED;
const std::string PATH_TEX_CREATURE_DOE_DED = "Textures/Creature/Deer/Doe-ded.png";

Texture TEX_CREATURE_DEER_FOOTPRINT;
const std::string PATH_TEX_CREATURE_DEER_FOOTPRINT = "Textures/Creature/Deer/Deer-footprint.png";

Texture TEX_CREATURE_BAT;
const std::string PATH_TEX_CREATURE_BAT = "Textures/Creature/Bat/Zubat.png";
Texture TEX_CREATURE_BAT_DED;
const std::string PATH_TEX_CREATURE_BAT_DED = "Textures/Creature/Bat/Zubat-ded.png";


Texture TEX_WORLD_TERRAIN_DESERT_00;
Texture TEX_WORLD_TERRAIN_DESERT_01;
const std::string PATH_TEX_WORLD_TERRAIN_DESERT_00 = "Textures/World/Terrain/Desert/00.png";
const std::string PATH_TEX_WORLD_TERRAIN_DESERT_01 = "Textures/World/Terrain/Desert/06.png";

Texture TEX_WORLD_TERRAIN_COAST_00;
Texture TEX_WORLD_TERRAIN_COAST_01;
Texture TEX_WORLD_TERRAIN_COAST_02;
Texture TEX_WORLD_TERRAIN_COAST_03;
const std::string PATH_TEX_WORLD_TERRAIN_COAST_00 = "Textures/World/Terrain/Coast/00.png";
const std::string PATH_TEX_WORLD_TERRAIN_COAST_01 = "Textures/World/Terrain/Coast/01.png";

Texture TEX_WORLD_TERRAIN_COAST_CORNER_00;
Texture TEX_WORLD_TERRAIN_COAST_CORNER_01;
Texture TEX_WORLD_TERRAIN_COAST_CORNER_02;
Texture TEX_WORLD_TERRAIN_COAST_CORNER_03;
const std::string PATH_TEX_WORLD_TERRAIN_COAST_CORNER_00 = "Textures/World/Terrain/Coast/02.png";

Texture TEX_WORLD_TERRAIN_COAST_PENINSULA_00;
Texture TEX_WORLD_TERRAIN_COAST_PENINSULA_01;
Texture TEX_WORLD_TERRAIN_COAST_PENINSULA_02;
Texture TEX_WORLD_TERRAIN_COAST_PENINSULA_03;
const std::string PATH_TEX_WORLD_TERRAIN_COAST_PENINSULA_00 = "Textures/World/Terrain/Coast/03.png";

Texture TEX_WORLD_TERRAIN_ISLAND_00;
const std::string PATH_TEX_WORLD_TERRAIN_ISLAND_00 = "Textures/World/Terrain/Island/00.png";

Texture TEX_WORLD_TERRAIN_CAVE_ENTRANCE;
const std::string PATH_TEX_WORLD_TERRAIN_CAVE_ENTRANCE = "Textures/World/Terrain/Cave/Cave-Entrance.png";

Texture TEX_WORLD_TEST_00;
const std::string PATH_TEX_WORLD_TEST_00 = "Textures/Test/00.png";

// OBJECTS

Texture TEX_OBJECT_STUMP;
const std::string PATH_TEX_OBJECT_STUMP = "Textures/Object/Stump32.png";

Texture TEX_OBJECT_PLANK;
const std::string PATH_TEX_OBJECT_PLANK = "Textures/Object/Plank.png";

//WALL
Texture TEX_WALL_GREYBRICK_SOUTH;
const std::string PATH_TEX_WALL_GREYBRICK_SOUTH = "Textures/Object/Wall/BrickWall-S.png";
Texture TEX_WALL_GREYBRICK_SE;
const std::string PATH_TEX_WALL_GREYBRICK_SE = "Textures/Object/Wall/BrickWall-SE-inside.png";

//FLOOR
Texture TEX_FLOOR_WOOD;
const std::string PATH_TEX_FLOOR_WOOD = "Textures/Object/Floor/Floor-Wood.png";

// ITEMS

Texture TEX_ITEM_SWORD;
const std::string PATH_TEX_ITEM_SWORD = "Textures/Item/Sword.png";

Texture TEX_ITEM_KNIFE;
const std::string PATH_TEX_ITEM_KNIFE = "Textures/Item/Knife.png";

Texture TEX_ITEM_LONGBOW;
const std::string PATH_TEX_ITEM_LONGBOW = "Textures/Item/Longbow.png";

Texture TEX_ITEM_FISHROD;
const std::string PATH_TEX_ITEM_FISHROD = "Textures/Item/Fishrod.png";

Texture TEX_ITEM_SHOVEL;
const std::string PATH_TEX_ITEM_SHOVEL = "Textures/Item/Shovel.png";

Texture TEX_ITEM_AXE;
const std::string PATH_TEX_ITEM_AXE = "Textures/Item/Axe.png";

Texture TEX_ITEM_PICKAXE;
const std::string PATH_TEX_ITEM_PICKAXE = "Textures/Item/Pickaxe.png";

Texture TEX_OBJECT_LOG;
const std::string PATH_TEX_OBJECT_LOG = "Textures/Object/Log.png";

Texture TEX_OBJECT_FISH;
const std::string PATH_TEX_OBJECT_FISH = "Textures/Object/Fish-32.png";

Texture TEX_ITEM_WATERSKIN;
const std::string PATH_TEX_ITEM_WATERSKIN = "Textures/Item/Waterskin.png";

Texture TEX_OBJECT_CAMPFIRE [5];
const std::string PATH_TEX_OBJECT_CAMPFIRE [5] = { "Textures/Object/Campfire-64-1.png", "Textures/Object/Campfire-64-2.png", "Textures/Object/Campfire-64-3.png", "Textures/Object/Campfire-64-4.png", "Textures/Object/Campfire-64-5.png" };

Texture TEX_OBJECT_TENT;
const std::string PATH_TEX_OBJECT_TENT = "Textures/Object/Tent.png";

// FOOD

Texture TEX_ITEM_FOOD_COOKED_FISH;
const std::string PATH_TEX_ITEM_FOOD_COOKED_FISH = "Textures/Item/Food/CookedFish.png";

Texture TEX_ITEM_FOOD_STEAK_COOKED;
const std::string PATH_TEX_ITEM_FOOD_STEAK_COOKED = "Textures/Item/Food/Steak-Cooked.png";

Texture TEX_ITEM_FOOD_STEAK_RAW;
const std::string PATH_TEX_ITEM_FOOD_STEAK_RAW = "Textures/Item/Food/Steak-Raw.png";

// FLORA

Texture TEX_FLORA_ADDERTONGUE;
const std::string PATH_TEX_FLORA_ADDERTONGUE = "Textures/Flora/AdderTongue-32.png";

//ORE
Texture TEX_WORLD_TERRAIN_ORE_METAL;
const std::string PATH_TEX_WORLD_TERRAIN_ORE_METAL = "Textures/World/Terrain/Ore/ore-32-3.png";

// CRAFTING
Texture TEX_CRAFTING_FLAX;
const std::string PATH_TEX_CRAFTING_FLAX = "Textures/Crafting/FlaxBundle.png";

Texture TEX_CRAFTING_LEATHER;
const std::string PATH_TEX_CRAFTING_LEATHER = "Textures/Crafting/Leather.png";

Texture TEX_CRAFTING_LEATHER_CLOTHES;
const std::string PATH_TEX_CRAFTING_LEATHER_CLOTHES = "Textures/Crafting/LeatherClothes.png";

// LOCAL MAP

Texture TEX_LOCAL_FLOOR;
const std::string PATH_TEX_LOCAL_FLOOR = "Textures/Local/Building/Floor.png";

Texture TEX_LOCAL_SIGN;
const std::string PATH_TEX_LOCAL_SIGN = "Textures/Local/Sign.png";

//MARKERS
Texture TEX_MARKER_RED;
const std::string PATH_TEX_MARKER_RED = "Textures/Marker/RedDot.png";

Texture TEX_MARKER_GREEN_SMALL;
const std::string PATH_TEX_MARKER_GREEN_SMALL = "Textures/Marker/GreenDotSmall.png";


// PORTRAITS

Texture TEX_PORTRAIT_SNEK;
const std::string PATH_TEX_PORTRAIT_SNEK = "Textures/Portrait/Snek.png";

Texture TEX_PORTRAIT_LING;
const std::string PATH_TEX_PORTRAIT_LING = "Textures/Portrait/Ling.png";

#endif
