#pragma once
#ifndef WORLDSIM_LOCALTILE_CPP
#define WORLDSIM_LOCALTILE_CPP

#include "LocalTile.hpp"

#include <Game/WorldGenerator/Biome.hpp>

#include "Creature.hpp"

/*
	Local tile data gets stored here. Render stuff can't go here because that depends on neighboring tiles.
*/

LocalTile::LocalTile()
{
  hasFloor = 0;
  bWall=0;
  nFish=-1;
  footprint=0;
  height=0;
  isLand=false;
  isCave=false;
  seed=0;
  
  bCliff=false;
  
  nGems=0;
  nMetal=0;
  
  shotOverlay=false;
  
  objStatic=0;
}

LocalTile::~LocalTile()
{
}

  // Push object to master list and generic object list.
void LocalTile::add(WorldObject* _object)
{
  vObject.push(_object);
  vObjectGeneric.push(_object);
}
  // Push object to Item list and master list.
void LocalTile::add(Item* _item)
{
  vItem.push(_item);
  vObject.push(_item);
}

void LocalTile::add(Character* _character)
{
  vCharacter.push(_character);
  vObject.push(_character);
}
void LocalTile::add(Creature* _creature)
{
  vCreature.push(_creature);
  vObject.push(_creature);
}


  // Remove object from master list and generic object list.
void LocalTile::remove(WorldObject* _object)
{
   vObject.erase(_object);
   vObjectGeneric.erase(_object);
}
  // Remove object from Item list and master list.
void LocalTile::remove(Item* _item)
{
   vItem.erase(_item);
   vObject.erase(_item);
}
  // Remove object from Character list and master list.
void LocalTile::remove(Character* _character)
{
   vCharacter.erase(_character);
   vObject.erase(_character);
}
  // Remove object from Creature list and master list.
void LocalTile::remove(Creature* _creature)
{
   vCreature.erase(_creature);
   vObject.erase(_creature);
}


void LocalTile::clearObjects()
{
   for ( int i=0; i<vObject.size();++i)
   {
      delete vObject(i);
   }
   vObject.clear();
}


    // Returns true is this tile has an object that can block line of sight.
bool LocalTile::hasViewBlocker()
{
   if (baseTerrain==UNDERGROUND)
   {
      return true;
   }
   if ( objStatic != 0 && objStatic->blockLOS != 0 )
   {
      return true;
   }


   if (bWall != 0)
   {
      return true;
   }
   for ( int i=0; i<vObject.size();++i)
   {
      if (vObject(i)->blocksView )
      {
         return true;
      }
   }
   return false;
}

    // Returns true is this tile has an object that can block movement.
bool LocalTile::hasMovementBlocker()
{
   if (baseTerrain==UNDERGROUND)
   {
      return true;
   }

   for ( int i=0; i<vObject.size();++i)
   {
      if (vObject(i)->blocksMovement)
      {
         return true;
      }
   }
   return false;
}


std::string LocalTile::getName()
{
   if ( baseTerrain == NOTHING )
   {
      return "Nothing";
   }
   else if ( baseTerrain == OCEAN )
   {
      return "Ocean";
   }
   else if (baseTerrain == FOREST)
   {
      return "Forest";
   }
   else if (baseTerrain == DESERT)
   {
      return "Desert";
   }
   else if (baseTerrain == UNDERGROUND)
   {
      return "Underground";
   }

   else if ( baseTerrain == GRASSLAND )
   {
      return "Grass";
   }
   else
   {
      return "Ground";
   }
}

    /* INHERITED FUNCTIONS */
Texture* LocalTile::currentTexture()
{
   if ( hasFloor==1 )
   {
      return &TEX_LOCAL_FLOOR;
   }
   else if (hasFloor==2)
   {
      return &TEX_FLOOR_WOOD;
   }

   //enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
   if ( baseTerrain == NOTHING )
   {
      return &TEX_WORLD_TEST_00;
   }
   else if ( baseTerrain == OCEAN )
   {
      return &TEX_WORLD_TERRAIN_OCEAN_00;
   }
   else if (baseTerrain == FOREST)
   {
      return &TEX_WORLD_TERRAIN_FOREST_TREE;
   }
   else if (baseTerrain == DESERT)
   {
      return &TEX_WORLD_TERRAIN_DESERT_00;
   }
   else if (baseTerrain == UNDERGROUND)
   {
      return &TEX_WORLD_TERRAIN_UNDERGROUND_00;
   }
   else if (baseTerrain == SNOW)
   {
      return &TEX_WORLD_TERRAIN_SNOW;
   }
   else if (baseTerrain == ICE)
   {
      return &TEX_WORLD_TERRAIN_SNOW;
   }
   else if (baseTerrain == STEPPES)
   {
      return &TEX_WORLD_TERRAIN_STEPPE;
   }
   else // GRASSLAND
   {
      if ( seed==0 || seed %4 == 0 )
      {
         return &TEX_WORLD_TERRAIN_GRASS_00;
      }
      else if ( seed %4 == 1 )
      {
         return &TEX_WORLD_TERRAIN_GRASS_01;
      }
      else if ( seed %4 == 2 )
      {
         return &TEX_WORLD_TERRAIN_GRASS_02;
      }
      return &TEX_WORLD_TERRAIN_GRASS_03;
   }

   return &TEX_WORLD_TEST_00;
}


   // Push in rendering order, aka terrain, static, objects, overlays
Vector <Texture*> * LocalTile::currentTextures()
{
   // reserve how much space we think we'll need on this vector
   auto vTexture = new Vector <Texture*> (2+vObject.size());
   
   // push base terrain
   vTexture->push(currentTexture());

   // push improvements
   if (nGems>0)
   {
      vTexture->push(&TEX_WORLD_ARTIFACT_GEMS);
   }
   if (nMetal>0)
   {
      vTexture->push(&TEX_WORLD_TERRAIN_ORE_METAL);
   }
   if (isCave)
   {
      vTexture->push(&TEX_WORLD_TERRAIN_CAVE_ENTRANCE);
   }
   if ( hasFloor==1 )
   {
      vTexture->push(&TEX_LOCAL_FLOOR);
   }
   else if (hasFloor==2)
   {
      vTexture->push(&TEX_FLOOR_WOOD);
   }
   if ( shotOverlay )
   {
      vTexture->push(&TEX_GUI_TILE_SELECTION_FULL);
   }
   
   // push static
   if (objStatic)
   {
      vTexture->push(objStatic->currentTexture());
   }
   
   // push footprint
   if (footprint)
   {
      vTexture->push(footprint->currentTexture());
   }
   
   for(unsigned short int i=0;i<vObject.size();++i)
   {
      vTexture->push(vObject(i)->currentTexture());
   }

   return vTexture;
}

bool LocalTile::canTravelNorth()
{
   if ((bWall & 0b10000000) == 0)
   {
      return true;
   }
   return false;
}
bool LocalTile::canTravelEast()
{
   if ((bWall & 0b01000000) == 0)
   {
      return true;
   }
   return false;
}
bool LocalTile::canTravelSouth()
{
   if ((bWall & 0b00100000) == 0)
   {
      return true;
   }
   return false;
}
bool LocalTile::canTravelWest()
{
   if ((bWall & 0b00010000) == 0)
   {
      return true;
   }
   return false;
}

std::string LocalTile::getAll(int max)
{
	if (max==0)
	{
		return "";
	}
	if (max==1)
	{
		return getName();
	}
	
	std::string strObjects=getName();
	
	// build vector and perform collation here
	
	Vector <std::string> vStuff;
	Vector <int> vQuantity;
	
			 
	 if (objStatic)
	 {
		 vStuff.push(objStatic->name );
		 vQuantity.push(1);
	 }
	 
	 if ( footprint )
	 {
		 vStuff.push("evidence");
		 vQuantity.push(1);
	 }
	 
	 for (int i=0;i<vCharacter.size();++i)
	 {
		 vStuff.push(vCharacter(i)->getFullName());
		 vQuantity.push(1);
	 }
	 
	 for (int i=0;i<vObject.size();++i)
	 {
		 // todo: collate objects of same name.
		 
		 if (vObject(i)->getName() != "Character" )
		 {
			 vStuff.push(vObject(i)->getName());
			 vQuantity.push(1);
		 }
	 }
	 
	 for (int i=0;i<vStuff.size()&&i<max-2;++i)
	 {
		 if (vQuantity(i) == 1)
		 {
			 strObjects+="\n"+vStuff(i);
		 }
		 else
		 {
			 strObjects+="\n"+DataTools::toString(vQuantity(i))+" "+vStuff(i);
		 }
	 }
	
	 if ( vStuff.size() >= max-2 )
	 {
		 strObjects+="\nAnd "+DataTools::toString(vStuff.size()-max)+" more objects";
	 }
	 
	return strObjects;
}

std::string LocalTile::getSaveData()
{
   std::string saveData = "";

   if ( baseTerrain == OCEAN )
   { saveData+="O"; }
   else if ( baseTerrain == FOREST )
   { saveData+="F"; }
   else if ( baseTerrain == DESERT )
   { saveData+="D"; }
   else if ( baseTerrain == MOUNTAIN )
   { saveData+="M"; }
   else if ( baseTerrain == SNOW )
   { saveData+="S"; }
   else if ( baseTerrain == HILLY )
   { saveData+="H"; }
   else if ( baseTerrain == JUNGLE )
   { saveData+="J"; }
   else if ( baseTerrain == WETLAND )
   { saveData+="W"; }
   else if ( baseTerrain == STEPPES )
   { saveData+="P"; }
   else if ( baseTerrain == ICE )
   { saveData+="I"; }
   else if ( baseTerrain == UNDERGROUND )
   { saveData+="U"; }
   else
   { saveData+="G"; }

   saveData+=(char)height+48;

   saveData+=" ";
   saveData+=DataTools::toString(seed);

   //saveData+=DataTools::toString(height);
   return saveData;
}

std::string LocalTile::getAbstractData()
   {
   std::string saveData = "";

   if ( hasMovementBlocker() )
   {
      return "T";
   }
   return "F";
}

void LocalTile::loadData(std::string _saveData)
{
   if ( _saveData[0] == 'O'  )
   { baseTerrain=OCEAN; }
   else if ( _saveData[0] == 'F' )
   { baseTerrain=FOREST; }
   else if ( _saveData[0] == 'D' )
   { baseTerrain=DESERT; }
   else if ( _saveData[0] == 'M' )
   { baseTerrain=MOUNTAIN; }
   else if ( _saveData[0] == 'S' )
   { baseTerrain=SNOW; }
   else if ( _saveData[0] == 'H' )
   { baseTerrain=HILLY; }
   else if ( _saveData[0] == 'J' )
   { baseTerrain=JUNGLE; }
   else if ( _saveData[0] == 'W' )
   { baseTerrain=WETLAND; }
   else if ( _saveData[0] == 'P' )
   { baseTerrain=STEPPES; }
   else if ( _saveData[0] == 'I' )
   { baseTerrain=ICE; }
   else if ( _saveData[0] == 'U' )
   { baseTerrain=UNDERGROUND; }
   else
   { baseTerrain = GRASSLAND; }

   height = _saveData[1] - 48;

   Vector <std::string> * vToke = DataTools::tokenize(_saveData, " \t\n\r");

   if (vToke && vToke->size()>1)
   {
      seed = DataTools::toUnsignedShort((*vToke)(1));
      delete vToke;
   }
}


#endif
