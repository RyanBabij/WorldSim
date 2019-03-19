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
  pathIndex=0;
}

void Creature_Knowledge::init()
{
  currentGoal.set(-1,-1);
}

  // Adds the tile to the Character's knowledge.
void Creature_Knowledge::addTile( World_Local* _map, int _x, int _y)
{
  if ( _map == 0 || _map->isSafe(_x,_y) == false ) { return; }

  vVisibleTiles.push((*_map)(_x,_y));

}

  //returns true if the Character has seen this tile.
char Creature_Knowledge::hasSeen( World_Local* _map, int _x, int _y)
{
  if ( _map == 0 || _map->isSafe(_x,_y) == false )
  {
    return false;
  }
  
  LocalTile* lt = (*_map)(_x,_y);
  if( vVisibleTiles.contains(lt) )
  {
    return 1;
  }
  return 0;

}

void Creature_Knowledge::updateLOS()
{
  //for now just clear everything
  clear();
}
void Creature_Knowledge::clear()
{
  //if ( vVisibleTiles!=0)
  { vVisibleTiles.clear();
  }
}

char Creature_Knowledge::nextStep()
{
  if ( p.vPath.isSafe(pathIndex) )
  {
    return p.vPath(pathIndex++);
  }
  pathIndex=0;
  p.vPath.clear();
  return 0;
} 

#endif