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
  currentGoal.set(-1,-1);
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
      (*vaTileVisited(i))(_x,_y) = 2;
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

  // Adds the tile to the Character's knowledge.
  // Overloaded to allow absolute coordinates.
void Character_Knowledge::addTile( unsigned long int _x, unsigned long int _y)
{
  if ( _x > world.maximumX || _y > world.maximumY )
  { return; }

  int _gX = 0;
  int _gY = 0;
  int _lX = 0;
  int _lY = 0;

  world.absoluteToRelative(_x, _y, &_gX, &_gY, &_lX, &_lY);
  
  World_Local* _map = world(_gX,_gY);
  
  if (_map == 0) { return; }

  for ( int i=0; i<vMapsVisited.size(); ++i)
  {
    if ( vMapsVisited(i) == _map )
    {
      // Flip fog bit
      (*vaTileVisited(i))(_lX,_lY) = 2;
      return;
    }
  }
  
  // // If we are here, the map doesn't exist in knowledge yet, and we must add it.
  
  vMapsVisited.push(_map);
  
  auto aFog = new ArrayS2 <char>;
  aFog->init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
  // Flip fog bit
  (*aFog)(_lX,_lY) = 1;
  
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
    (*vaTileVisited(i)).replaceFill(2,1);
  }
}


#endif