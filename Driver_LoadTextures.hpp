#pragma once
#ifndef WORLDSIM_LOAD_TEXTURES_HPP
#define WORLDSIM_LOAD_TEXTURES_HPP

#include <string>

#define THREADED_TEXTURE_LOADING
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  #include <thread>
#endif

#include <Graphics/Texture/TextureLoader.hpp>
#include <Graphics/Texture/Texture.hpp>

/* WorldSim: Driver_LoadTextures.hpp
  #include "Driver_LoadTextures.hpp"

	Called from Driver::init().
	These vars are global.
	All texture refs start with TEX_, and then use the path and filename from the textures folder.
	Requires OpenGL headers.
*/

void SetColor(int value){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
}


static Texture TEX_TITLE;
static Texture TEX_OPTIONS_BACKGROUND;
static Texture TEX_LOAD_GAME_BACKGROUND;
static Texture TEX_NEW_GAME_BACKGROUND;

static Texture TEX_GUI_INCREMENT;
static Texture TEX_GUI_DECREMENT;
static Texture TEX_GUI_EXPAND;
static Texture TEX_GUI_TILE_SELECTION;

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

Texture TEX_ALCHEMY;
const std::string PATH_TEX_ALCHEMY = "Textures/World/Terrain/Alchemy/05.png";

Texture TEX_WORLD_TERRAIN_OCEAN_00;
const std::string PATH_TEX_WORLD_TERRAIN_OCEAN_00 = "Textures/World/Terrain/Ocean/01.png";

Texture TEX_WORLD_TERRAIN_FOREST_TREES;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_TREES = "Textures/World/Terrain/Forest/00.png";

  // FLORA
Texture TEX_WORLD_TERRAIN_FOREST_TREE;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_TREE = "Textures/World/Terrain/Forest/tree.png";

Texture TEX_WORLD_TERRAIN_FOREST_SAPLING;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_SAPLING = "Textures/World/Terrain/Forest/Sapling.png";

Texture TEX_FLORA_PLANT;
const std::string PATH_TEX_FLORA_PLANT = "Textures/Flora/Plant-small.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_00;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_00 = "Textures/World/Terrain/Mountain/01.png";

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
  
Texture TEX_ITEM_LONGBOW;
const std::string PATH_TEX_ITEM_LONGBOW = "Textures/Item/Longbow.png";

Texture TEX_ITEM_FISHROD;
const std::string PATH_TEX_ITEM_FISHROD = "Textures/Item/Fishrod.png";

Texture TEX_ITEM_SHOVEL;
const std::string PATH_TEX_ITEM_SHOVEL = "Textures/Item/Shovel.png";

Texture TEX_ITEM_AXE;
const std::string PATH_TEX_ITEM_AXE = "Textures/Item/Axe.png";

Texture TEX_OBJECT_LOG;
const std::string PATH_TEX_OBJECT_LOG = "Textures/Object/Log.png";

Texture TEX_OBJECT_FISH;
const std::string PATH_TEX_OBJECT_FISH = "Textures/Object/Fish-32.png";

Texture TEX_OBJECT_CAMPFIRE [5];
const std::string PATH_TEX_OBJECT_CAMPFIRE [5] = { "Textures/Object/Campfire-64-1.png", "Textures/Object/Campfire-64-2.png", "Textures/Object/Campfire-64-3.png", "Textures/Object/Campfire-64-4.png", "Textures/Object/Campfire-64-5.png" };

  // FOOD
  
Texture TEX_ITEM_FOOD_COOKED_FISH;
const std::string PATH_TEX_ITEM_FOOD_COOKED_FISH = "Textures/Item/Food/CookedFish.png";


  // FLORA
  
Texture TEX_FLORA_ADDERTONGUE;
const std::string PATH_TEX_FLORA_ADDERTONGUE = "Textures/Flora/AdderTongue-32.png";

  // CRAFTING
Texture TEX_CRAFTING_FLAX;
const std::string PATH_TEX_CRAFTING_FLAX = "Textures/Crafting/FlaxBundle.png";

  // LOCAL MAP
  
Texture TEX_LOCAL_FLOOR;
const std::string PATH_TEX_LOCAL_FLOOR = "Textures/Local/Building/Floor.png";

Texture TEX_LOCAL_SIGN;
const std::string PATH_TEX_LOCAL_SIGN = "Textures/Local/Sign.png";

  //MARKERS
Texture TEX_MARKER_RED;
const std::string PATH_TEX_MARKER_RED = "Textures/Marker/RedDot.png";
  // PORTRAITS
  
Texture TEX_PORTRAIT_SNEK;
const std::string PATH_TEX_PORTRAIT_SNEK = "Textures/Portrait/Snek.png";

Texture TEX_PORTRAIT_LING;
const std::string PATH_TEX_PORTRAIT_LING = "Textures/Portrait/Ling.png";

void loadTextureVerbose(const std::string _path, Texture* _texture)
{
	std::cout<<"Loading: "<<_path<<".\n";
	
	if(loadTextureNearestNeighbour(_path,_texture)==false)
	{ std::cout<<"Error loading "<<_path<<".\n"; }
}

void preloadTextureVerbose(const std::string _path, Texture* _texture)
{

	
	if(loadTextureNearestNeighbour(_path,_texture)==false)
	{
	SetColor(4);
    std::cout<<"Loading: "<<_path<<": FAIL.\n";
  SetColor(7);
  }
  else
  {
    std::cout<<"Loading: "<<_path<<": SUCCESS.\n";
  }

}


void loadTextures() // Testing some multithreading here. Probably shouldn't because a texture atlas would be better
{
  
//#undef THREADED_TEXTURE_LOADING
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  std::thread t1( []
  {
#endif
      // LOAD MENU TEXTURES
    //loadTextureVerbose("Textures/Menu/TitleImage-CR.png",&TEX_TITLE);
    preloadTextureVerbose("Textures/Menu/Title.png",&TEX_TITLE);
    preloadTextureVerbose("Textures/Menu/Black.png",&TEX_OPTIONS_BACKGROUND);
    preloadTextureVerbose("Textures/Menu/Black.png",&TEX_LOAD_GAME_BACKGROUND);
    preloadTextureVerbose("Textures/Menu/Black.png",&TEX_NEW_GAME_BACKGROUND);
    
    preloadTextureVerbose(PATH_TEX_WORLD_SETTLEMENT_DWARFFORT_01,&TEX_WORLD_SETTLEMENT_DWARFFORT_01);
    preloadTextureVerbose(PATH_TEX_WORLD_SETTLEMENT_TOWN_URBAN01,&TEX_WORLD_SETTLEMENT_TOWN_URBAN01);
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  });
  
  std::thread t2( []
  {
#endif
      // LOAD GUI TEXTURES
    preloadTextureVerbose("Textures/GUI/Increment.png",&TEX_GUI_INCREMENT);
    preloadTextureVerbose("Textures/GUI/Decrement.png",&TEX_GUI_DECREMENT);
    preloadTextureVerbose("Textures/GUI/Expand.png",&TEX_GUI_EXPAND);
    preloadTextureVerbose("Textures/GUI/HotbarSelection32.png",&TEX_GUI_TILE_SELECTION );
    
      // LOAD LOCAL OBJECT TEXTURES
    preloadTextureVerbose("Textures/NPC.png",&TEX_NPC);
    preloadTextureVerbose("Textures/NPC-ded.png",&TEX_NPC_DED);
    
  // LOAD WORLD OBJECT TEXTURES
    preloadTextureVerbose(PATH_TEX_WORLD_UNIT_NOMAD_01,&TEX_WORLD_UNIT_NOMAD_01);
    preloadTextureVerbose(PATH_TEX_WORLD_UNIT_DWARF_01,&TEX_WORLD_UNIT_DWARF_01);
    preloadTextureVerbose(PATH_TEX_WORLD_UNIT_ELF_01,&TEX_WORLD_UNIT_ELF_01);
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  });
#endif
    
    
#if defined THREAD_ALL || defined THREADED_TEXTURE_LOADING
  t1.join();
  t2.join();
#endif

  bindNearestNeighbour(&TEX_TITLE);
  bindNearestNeighbour(&TEX_OPTIONS_BACKGROUND);
  bindNearestNeighbour(&TEX_LOAD_GAME_BACKGROUND);
  bindNearestNeighbour(&TEX_NEW_GAME_BACKGROUND);
  
  bindNearestNeighbour(&TEX_GUI_INCREMENT);
  bindNearestNeighbour(&TEX_GUI_DECREMENT);
  bindNearestNeighbour(&TEX_GUI_EXPAND);
  bindNearestNeighbour(&TEX_GUI_TILE_SELECTION);
  
  bindNearestNeighbour(&TEX_NPC);
  bindNearestNeighbour(&TEX_NPC_DED);
  
  bindNearestNeighbour(&TEX_WORLD_UNIT_NOMAD_01);
  bindNearestNeighbour(&TEX_WORLD_UNIT_DWARF_01);
  bindNearestNeighbour(&TEX_WORLD_UNIT_ELF_01);
  
  bindNearestNeighbour(&TEX_WORLD_SETTLEMENT_DWARFFORT_01);
  bindNearestNeighbour(&TEX_WORLD_SETTLEMENT_TOWN_URBAN01);

	
	loadTextureRotate(PATH_TEX_WORLD_TERRAIN_GRASS_05,&TEX_WORLD_TERRAIN_GRASS_00,&TEX_WORLD_TERRAIN_GRASS_01,&TEX_WORLD_TERRAIN_GRASS_02,&TEX_WORLD_TERRAIN_GRASS_03);
	loadTextureRotate(PATH_TEX_WORLD_TERRAIN_COAST_01,&TEX_WORLD_TERRAIN_COAST_00,&TEX_WORLD_TERRAIN_COAST_01,&TEX_WORLD_TERRAIN_COAST_02,&TEX_WORLD_TERRAIN_COAST_03);
	loadTextureRotate(PATH_TEX_WORLD_TERRAIN_COAST_CORNER_00,&TEX_WORLD_TERRAIN_COAST_CORNER_00,&TEX_WORLD_TERRAIN_COAST_CORNER_01,&TEX_WORLD_TERRAIN_COAST_CORNER_02,&TEX_WORLD_TERRAIN_COAST_CORNER_03);
	loadTextureRotate(PATH_TEX_WORLD_TERRAIN_COAST_PENINSULA_00,&TEX_WORLD_TERRAIN_COAST_PENINSULA_00,&TEX_WORLD_TERRAIN_COAST_PENINSULA_01,&TEX_WORLD_TERRAIN_COAST_PENINSULA_02,&TEX_WORLD_TERRAIN_COAST_PENINSULA_03);
	
	//loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_COAST_00,&TEX_WORLD_TERRAIN_COAST_00);
	//loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_COAST_01,&TEX_WORLD_TERRAIN_COAST_01);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_ISLAND_00,&TEX_WORLD_TERRAIN_ISLAND_00);

	loadTextureVerbose(PATH_TEX_ALCHEMY,&TEX_ALCHEMY);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_OCEAN_00,&TEX_WORLD_TERRAIN_OCEAN_00);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_FOREST_TREES,&TEX_WORLD_TERRAIN_FOREST_TREES);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_FOREST_TREE,&TEX_WORLD_TERRAIN_FOREST_TREE);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_FOREST_SAPLING,&TEX_WORLD_TERRAIN_FOREST_SAPLING);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_00,&TEX_WORLD_TERRAIN_MOUNTAIN_00);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI,&TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD,&TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD);
  
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_UNDERGROUND_00,&TEX_WORLD_TERRAIN_UNDERGROUND_00);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_DESERT_00,&TEX_WORLD_TERRAIN_DESERT_00);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_DESERT_01,&TEX_WORLD_TERRAIN_DESERT_01);
	
	loadTextureVerbose(PATH_TEX_WORLD_TEST_00,&TEX_WORLD_TEST_00);
  
  loadTextureVerbose(PATH_TEX_TEST_404,&TEX_TEST_404);
  loadTextureVerbose(PATH_TEX_WORLD_ARTIFACT_GEMS,&TEX_WORLD_ARTIFACT_GEMS);
  loadTextureVerbose(PATH_TEX_WORLD_ARTIFACT_BATTLE,&TEX_WORLD_ARTIFACT_BATTLE);
  
  
  loadTextureVerbose(PATH_TEX_LOCAL_FLOOR,&TEX_LOCAL_FLOOR);
  
  loadTextureVerbose(PATH_TEX_LOCAL_SIGN,&TEX_LOCAL_SIGN);
  loadTextureVerbose(PATH_TEX_GUI_CENTER_CAMERA,&TEX_GUI_CENTER_CAMERA);
  
  loadTextureVerbose(PATH_TEX_CREATURE_DEER,&TEX_CREATURE_DEER);
  loadTextureVerbose(PATH_TEX_CREATURE_DEER_DED,&TEX_CREATURE_DEER_DED);
  loadTextureVerbose(PATH_TEX_CREATURE_DOE,&TEX_CREATURE_DOE);
  loadTextureVerbose(PATH_TEX_CREATURE_DOE_DED,&TEX_CREATURE_DOE_DED);
  loadTextureVerbose(PATH_TEX_CREATURE_DEER_FOOTPRINT,&TEX_CREATURE_DEER_FOOTPRINT);
  
  loadTextureVerbose(PATH_TEX_PORTRAIT_SNEK,&TEX_PORTRAIT_SNEK);
  loadTextureVerbose(PATH_TEX_PORTRAIT_LING,&TEX_PORTRAIT_LING);
  
  // ITEMS
  loadTextureVerbose(PATH_TEX_ITEM_SWORD,&TEX_ITEM_SWORD);
  loadTextureVerbose(PATH_TEX_ITEM_LONGBOW,&TEX_ITEM_LONGBOW);
  loadTextureVerbose(PATH_TEX_ITEM_FISHROD,&TEX_ITEM_FISHROD);
  loadTextureVerbose(PATH_TEX_ITEM_SHOVEL,&TEX_ITEM_SHOVEL);
  loadTextureVerbose(PATH_TEX_ITEM_AXE,&TEX_ITEM_AXE);
  loadTextureVerbose(PATH_TEX_OBJECT_LOG,&TEX_OBJECT_LOG);
  loadTextureVerbose(PATH_TEX_OBJECT_FISH,&TEX_OBJECT_FISH);
  
  for (int i=0;i<5;++i)
  {
    loadTextureVerbose(PATH_TEX_OBJECT_CAMPFIRE[i],&TEX_OBJECT_CAMPFIRE[i]);
  }

  
  // FOOD
  loadTextureVerbose(PATH_TEX_ITEM_FOOD_COOKED_FISH,&TEX_ITEM_FOOD_COOKED_FISH);
  
  //Campfire-64-1.png
  
  //FLORA
  loadTextureVerbose(PATH_TEX_FLORA_ADDERTONGUE,&TEX_FLORA_ADDERTONGUE);
  loadTextureVerbose(PATH_TEX_FLORA_PLANT,&TEX_FLORA_PLANT);
  
  //OBJECTS
  loadTextureVerbose(PATH_TEX_OBJECT_STUMP,&TEX_OBJECT_STUMP);
  loadTextureVerbose(PATH_TEX_OBJECT_PLANK,&TEX_OBJECT_PLANK);
    // WALL
  loadTextureVerbose(PATH_TEX_WALL_GREYBRICK_SOUTH,&TEX_WALL_GREYBRICK_SOUTH);
  loadTextureVerbose(PATH_TEX_WALL_GREYBRICK_SE,&TEX_WALL_GREYBRICK_SE);
  
    // FLOOR
  loadTextureVerbose(PATH_TEX_FLOOR_WOOD,&TEX_FLOOR_WOOD);
  
  // CRAFTING
  loadTextureVerbose(PATH_TEX_CRAFTING_FLAX,&TEX_CRAFTING_FLAX);
  
  //MARKERS
  loadTextureVerbose(PATH_TEX_MARKER_RED,&TEX_MARKER_RED);
  
  //loadTextureMipmapRotate("Textures/Object/Wall/BrickWall-S.png", &TEX_WALL_GREYBRICK_SOUTH, &TEX_WALL_GREYBRICK_WEST, &TEX_WALL_GREYBRICK_NORTH, &TEX_WALL_GREYBRICK_EAST);
  //loadTextureMipmapRotate("Textures/Wall/Brick/window.png", &TEX_WINDOW_SOUTH, &TEX_WINDOW_WEST, &TEX_WINDOW_NORTH, &TEX_WINDOW_EAST);
  //loadTextureMipmapRotate("Textures/Wall/Brick/se.png", &TEX_WALL_GREYBRICK_NORTHWEST, &TEX_WALL_GREYBRICK_NORTHEAST, &TEX_WALL_GREYBRICK_SOUTHEAST, &TEX_WALL_GREYBRICK_SOUTHWEST);

  Png pNYA;
  pNYA.loadHex("89504e470d0a1a0a0000000d4948445200000099000000840103000000b9da1bbc00000006504c5445000000ffffffa5d99fdd0000047f4944415448896dd6cb8b1c451cc0f15a17dd8b66050f5e12fbe0410521e02528494a6f82a0070f1e3ce4e0614f820f306449b64a826e4e06141131a4ff841c040f79d81b363011d4be29123733eb869460dc9e49b3e9ee74757d3d54f54cf73a0dc3cc7c7abae6f7a8ae6a0140298490b487168c84100a6ce484586eb1800a8045b01d8c011adab35a90e31440115ea0053555f802598b1529c380718b96246983912dba35a582f9c1d1028e394fc5e88969f07cd7f8ffa92fbc3c43535f2900cad34fcd30055c069307cb338c0b320ac045338cee91d1486a9bcef0c82ea97303eabf67e89efb97d49d7c9b72a98327b6c8b85201a68b49c6ad2b3db47238dc690c7d54b76fafb8cd4c4dc867b8fce7d32f9407e1620fb7161f55f12d2e876ea005e509b7b07269b9e0a1da9e61facb41954615b5ba39c51d73e380bab3ec78a87e9e1579a738a0dcefb04727f7edea55c5ad096f84bea105944ec04870c4764a67d7001aa29b72861c0f7dff5275f02b0558dc635dfc34019a7a1c5ae4b1324093bd1675d19e02f6b616e9a27b5ec2e56beb3de4d925c6acbdd7c7b3e79c7ce04ef57173a4d5765bcd1677eaa564e09cece1a48c64d6b0dec50d2149a8197772771f018a0c163af55c0520a17c464e315e0d635d1fbf3545b90a87870dee4d655ab46a95861aeeaff8d9a0b5a06295fa10f04f420d8c11829c53aa58bfadf8c1dfb0038f67d4121fe2de050b4e3a2d28b8b6f631036c02162cdb5a60f8edc7fb6ac8c32138c89dd482cc3176b12253e014a6440bd2066bdf87af01a7480d5ab050d07012def118a768e13ef886ef2968222093c4095a8c8ee744e4ec296028910a2d4c54d9b1acb7c70089640db4d8c48d46aabe1a2d024abad3a0c54fb8d11e1b3e1ba46d408b01e5e7071e39865b8086c4d6a045ca5d11950a6a68904dee31de5d3a7f08622885ac72d0c2a098f0192490435580162307e0408169317b1cc01db509a45017a0c590d911b718775042be1f9d82dc80eeaccf344061408b6486f53c344091ed1b33ede1fa34224ce673675c4e230a68c07016c2ca9f65a045a9c86119fc94f3484eeda10048d3b04dd43e9a0a201e821664d47e2bb000491c30f7d800c804b420a5f0633a05288ff17a0a46fa381d0ab4e0ea58c2e484cfa8711e8f7e02ec463ef7ca015aa05e5121c51cf206d0c2a94b8acc291fa8a9002d2cc3a36436249fe6801615c3aba46742996283af5276815fab50509906345bdcad7d3f9c92018b3fb8e3d76d533a056891510d98f8bd2bcb2df869e30ef3a4bffb4d5a798c2123acc69bd2784c607b1870ec5765dfe217238fb5df383dbef47a0440e37bad450a2c8435d60e021af01d06c26aa74509b0e171ba413b09d4b1c724203b805b215424600ef0ed3cbce7316dd100dc573ef916cf0334b2873e361bf72eb7aa13761ed0bffb53d32796621e865d95dee5937968ba3898876d9cf9bc5f5a39b3dd282071784a02c6fc1fb33b533cdfeecb6c9c9ee23a37fc67a7ca295e8cbe508c2418cace98f097ba8ea3f3cc100325153b5d94404e69d57e3410d141277d7a710f1590faf6f431de871640ce41a77cd5a658e2db6680d01afd1f175a0c4816121ff30000000049454e44ae4260827265616c6c79206d616b657320796f75206e7961");
  
  NYA="\n\n";
  for (int _nyaY=0;_nyaY<pNYA.nY;++_nyaY)
  {for (int _nyaX=0;_nyaX<pNYA.nX;++_nyaX){
        (pNYA.getPixel3D(_nyaX,_nyaY,0) == 0) ? NYA+="*" : NYA+=" ";  
    } NYA+="\n";}
    
}

#endif
