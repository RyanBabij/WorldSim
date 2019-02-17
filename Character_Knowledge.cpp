#pragma once
#ifndef WORLDSIM_CHARACTER_KNOWLEDGE_CPP
#define WORLDSIM_CHARACTER_KNOWLEDGE_CPP

/* WorldSim: Character_Knowledge
  #include "Character_Knowledge.cpp"
  
  Implementation of Character_Knowledge.hpp
*/

#include "Character_Knowledge.hpp"

Character_Knowledge::Character_Knowledge()
{

}

void Character_Knowledge::init()
{
}

  // Adds the tile to the Character's knowledge.
void Character_Knowledge::addTile( World_Local* _map, int _x, int _y)
{
  if ( _map == 0 ) { return; }
  
  if ( _x < 0 || _y < 0 || _x >= LOCAL_MAP_SIZE || _y >= LOCAL_MAP_SIZE )
  { return; }
  
  for ( int i=0; i<vMapsVisited.size(); ++i)
  {
    if ( vMapsVisited(i) == _map )
    {
      // Flip fog bit
      (*vaTileVisited(i))(_x,_y) = 1;
      return;
    }
  }
  
  // If we are here, the map doesn't exist in knowledge yet, and we must add it.
  
  vMapsVisited.push(_map);
  
  auto aFog = new ArrayS2 <char>;
  aFog->init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
    // Flip fog bit
  (*aFog)(_x,_y) = 1;
  
  vaTileVisited.push(aFog);
}


  //returns true if the Character has seen this tile.
char Character_Knowledge::hasSeen( World_Local* _map, int _x, int _y)
{
  if ( _map == 0 ) { return false; }
  
  if ( _x < 0 || _y < 0 || _x >= LOCAL_MAP_SIZE || _y >= LOCAL_MAP_SIZE )
  { return 0; }
  
  for ( int i=0; i<vMapsVisited.size(); ++i)
  {
    if ( vMapsVisited(i) == _map )
    {
      return (*vaTileVisited(i))(_x,_y);
    }
  }
  return 0;
}

void Character_Knowledge::updateLOS()
{
  for ( int i=0; i<vMapsVisited.size(); ++i)
  {
    (*vaTileVisited(i)).fill(0);
  }
}


#endif