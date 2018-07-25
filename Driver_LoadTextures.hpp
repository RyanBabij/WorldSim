#pragma once
#ifndef LOAD_TEXTURES_HPP
#define LOAD_TEXTURES_HPP

#include <string>

#include <Graphics/Texture/TextureLoader.hpp>
#include <Graphics/Texture/Texture.hpp>

/*
	0221215653: TextureList and LoadTextures have been merged.
*/

/*
	Called from Driver::init().
	These vars are global.
	All texture refs start with TEX_, and then use the path and filename from the textures folder.
	Requires OpenGL headers.
*/


Texture TEX_TITLE;
Texture TEX_OPTIONS_BACKGROUND;
Texture TEX_LOAD_GAME_BACKGROUND;
Texture TEX_NEW_GAME_BACKGROUND;

Texture TEX_GUI_INCREMENT;
Texture TEX_GUI_DECREMENT;
Texture TEX_GUI_EXPAND;


Texture TEX_NPC;

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

Texture TEX_WORLD_TERRAIN_FOREST_TREE;
const std::string PATH_TEX_WORLD_TERRAIN_FOREST_TREE = "Textures/World/Terrain/Forest/tree.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_00;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_00 = "Textures/World/Terrain/Mountain/01.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI = "Textures/World/Terrain/Mountain/Rockyboi.png";

Texture TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD;
const std::string PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD = "Textures/World/Terrain/Mountain/RockyboiGold.png";

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

void loadTextureVerbose(const std::string _path, Texture* _texture)
{
	std::cout<<"Loading: "<<_path<<".\n";
	
	if(loadTextureNearestNeighbour(_path,_texture)==false)
	{ std::cout<<"Error loading "<<_path<<".\n"; }
}

void loadTextures()
{
  
		// LOAD MENU TEXTURES
	//loadTextureVerbose("Textures/Menu/TitleImage-CR.png",&TEX_TITLE);
	loadTextureVerbose("Textures/Menu/Black.png",&TEX_TITLE);
	loadTextureVerbose("Textures/Menu/Black.png",&TEX_OPTIONS_BACKGROUND);
	loadTextureVerbose("Textures/Menu/Black.png",&TEX_LOAD_GAME_BACKGROUND);
	loadTextureVerbose("Textures/Menu/Black.png",&TEX_NEW_GAME_BACKGROUND);
	
		// LOAD GUI TEXTURES
	loadTextureVerbose("Textures/GUI/Increment.png",&TEX_GUI_INCREMENT);
	loadTextureVerbose("Textures/GUI/Decrement.png",&TEX_GUI_DECREMENT);
	loadTextureVerbose("Textures/GUI/Expand.png",&TEX_GUI_EXPAND);
	
		// LOAD LOCAL OBJECT TEXTURES
	loadTextureVerbose("Textures/NPC.png",&TEX_NPC);
	
		// LOAD WORLD OBJECT TEXTURES
	loadTextureVerbose(PATH_TEX_WORLD_UNIT_NOMAD_01,&TEX_WORLD_UNIT_NOMAD_01);
	loadTextureVerbose(PATH_TEX_WORLD_UNIT_DWARF_01,&TEX_WORLD_UNIT_DWARF_01);
	loadTextureVerbose(PATH_TEX_WORLD_UNIT_ELF_01,&TEX_WORLD_UNIT_ELF_01);
  
	loadTextureVerbose(PATH_TEX_WORLD_SETTLEMENT_DWARFFORT_01,&TEX_WORLD_SETTLEMENT_DWARFFORT_01);
	loadTextureVerbose(PATH_TEX_WORLD_SETTLEMENT_TOWN_URBAN01,&TEX_WORLD_SETTLEMENT_TOWN_URBAN01);
	
		// LOAD WORLD TERRAIN TEXTURES
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_00,&TEX_WORLD_TERRAIN_GRASS_00);
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_01,&TEX_WORLD_TERRAIN_GRASS_01);
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_02,&TEX_WORLD_TERRAIN_GRASS_02);
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_03,&TEX_WORLD_TERRAIN_GRASS_03);
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_04,&TEX_WORLD_TERRAIN_GRASS_04);
	// loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_GRASS_05,&TEX_WORLD_TERRAIN_GRASS_05);
	
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
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_00,&TEX_WORLD_TERRAIN_MOUNTAIN_00);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI,&TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI);
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD,&TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_DESERT_00,&TEX_WORLD_TERRAIN_DESERT_00);
	
	loadTextureVerbose(PATH_TEX_WORLD_TERRAIN_DESERT_01,&TEX_WORLD_TERRAIN_DESERT_01);
	
	loadTextureVerbose(PATH_TEX_WORLD_TEST_00,&TEX_WORLD_TEST_00);
  
  loadTextureVerbose(PATH_TEX_TEST_404,&TEX_TEST_404);
  loadTextureVerbose(PATH_TEX_WORLD_ARTIFACT_GEMS,&TEX_WORLD_ARTIFACT_GEMS);
  loadTextureVerbose(PATH_TEX_WORLD_ARTIFACT_BATTLE,&TEX_WORLD_ARTIFACT_BATTLE);
}

#endif
