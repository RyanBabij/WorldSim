#pragma once
#ifndef WORLDSIM_CREATURE_KNOWLEDGE_CPP
#define WORLDSIM_CREATURE_KNOWLEDGE_CPP

/* WorldSim: Creature_Knowledge
  #include "Creature_Knowledge.cpp"
  
  Implementation of Creature_Knowledge.hpp
*/

#include "Creature_Knowledge.hpp"

Creature_Knowledge::Creature_Knowledge()
{
  vVisibleTiles=0;
}

void Creature_Knowledge::init()
{
  vVisibleTiles=0;
}

  // Adds the tile to the Character's knowledge.
void Creature_Knowledge::addTile( World_Local* _map, int _x, int _y)
{
  // if ( _map == 0 ) { return; }
  
  // if ( _x < 0 || _y < 0 || _x >= LOCAL_MAP_SIZE || _y >= LOCAL_MAP_SIZE )
  // { return; }
  
  // for ( int i=0; i<vMapsVisited.size(); ++i)
  // {
    // if ( vMapsVisited(i) == _map )
    // {
      // // Flip fog bit
      // (*vaTileVisited(i))(_x,_y) = 2;
      // return;
    // }
  // }
  
  // // If we are here, the map doesn't exist in knowledge yet, and we must add it.
  
  // vMapsVisited.push(_map);
  
  // auto aFog = new ArrayS2 <char>;
  // aFog->init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
    // // Flip fog bit
  // (*aFog)(_x,_y) = 1;
  
  // vaTileVisited.push(aFog);
}

  //returns true if the Character has seen this tile.
char Creature_Knowledge::hasSeen( World_Local* _map, int _x, int _y)
{
  // if ( _map == 0 ) { return false; }
  
  // if ( _x < 0 || _y < 0 || _x >= LOCAL_MAP_SIZE || _y >= LOCAL_MAP_SIZE )
  // { return 0; }
  
  // for ( int i=0; i<vMapsVisited.size(); ++i)
  // {
    // if ( vMapsVisited(i) == _map )
    // {
      // return (*vaTileVisited(i))(_x,_y);
    // }
  // }
  return 0;
}

void Creature_Knowledge::updateLOS()
{
  // if ( vVisibleTiles!=0)
  // { vVisibleTiles->clear();
  // }
  
  
  //vVisibleTiles = world.getVisibleTiles(20,20,MAX_VIEW_RANGE/2);
}


#endif