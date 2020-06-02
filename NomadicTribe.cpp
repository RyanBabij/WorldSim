#pragma once
#ifndef WORLDSIM_NOMADIC_TRIBE_CPP
#define WORLDSIM_NOMADIC_TRIBE_CPP

/* WorldSim: NomadicTribe.cpp
   #include "NomadicTribe.cpp"

*/

#include "NomadicTribe.hpp"


NomadicTribe::NomadicTribe()
{
   name="?";
   
   worldX=0;
   worldY=0;
   
   inCombat=false;
   
   type=NOMADIC_TRIBE;
   
}


void NomadicTribe::addNomads(int nNomads)
{
   for ( int i =0 ;i<nSol;++i)
   {
      Soldier * s = new Soldier;
      
      vSoldier.push(s);
   }
}

bool NomadicTribe::canAccess(int _x, int _y)
{
   // if ( owner->aVisible.isSafe(_x,_y) )
   // {
      // // TRY TO PATHFIND TO THIS TILE.
   // }
   
   return false;

}

void NomadicTribe::wander()
{
   int destinationX = worldX + Random::randomIntRange(-1,1);
   int destinationY = worldY + Random::randomIntRange(-1,1);
   
   if (world->isSafe(destinationX,destinationY) && world->isLand(destinationX,destinationY))
   {
      // MOVE THE ARMY TO THE LOCATION.
      worldX=destinationX;
      worldY=destinationY;
      // REVEAL FOG AROUND THE ARMY.
      owner->aVisible.setNeighbors(worldX,worldY,true);
   }
}

void NomadicTribe::incrementTicks(int _nTicks)
{
   wander();
   
   if ( owner==0 || world == 0 )
   {
      std::cout<<"Error: Army has no connection to world.\n";
      return;
   }
   
   // IF THE ARMY IS NEXT TO A HOSTILE ARMY, THEN FIGHT.
      // LOOK FOR NEARBY OBJECTS.
      //std::cout<<"Checking neigbors...\n";
      Vector <WorldObjectGlobal*> * vNearbyObjects = world->getNeighboringObjects(this);
      if ( vNearbyObjects->size() > 0 )
      {
         //std::cout<<"NEARBY FOUND.\n";
         
         //if ( owner->atWarWith() )
         //{
            
         //}
         
      }
}

Texture* NomadicTribe::currentTexture()
{
   return &TEX_WORLD_ARMY_01;
}

#endif
