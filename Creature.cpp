#pragma once
#ifndef WORLDSIM_CREATURE_CPP
#define WORLDSIM_CREATURE_CPP

/* WorldSim: Creature
  #include "Creature.cpp"
  
  Implementation of Creature.hpp
*/

#include "World.hpp"
#include "Creature.hpp"

class Texture;

Creature::Creature()
{

	isMale = true;
	age = -1;
	daysCounter=0;
	secondsCounter=0;
	
	isAlive=true;

	actionPoints = 0;
	
	health=0;
	hunger=0;
  
  worldX = -1;
  worldY = -1;
	
  map=0;
}

  //_sex: 0 - Roll, 1 - Male, 2 - Female.
void Creature::init(const int _sex /* =0 */)
{
  
  if (_sex == 1) { isMale = true; }
  else if (_sex == 2) { isMale = false; }
  else { isMale = Random::flip(); }
  
	age=0;
  map=0;

}


void Creature::incrementTicks(int nTicks)
{
	secondsCounter+=nTicks;
	
	while(secondsCounter>=86400)
	{
		++daysCounter;
		secondsCounter-=86400;
	}
	while(daysCounter >=360)
	{
		age++;
		daysCounter-=360;
	}
}

void Creature::die()
{
	isAlive = false;
}


// void Creature::attack(Creature* target)
// {
  // if (target == 0 || target->isAlive == false ) { return; }

// }



void Creature::wander()
{
  
  int newX = x;
  int newY = y;
  
  int direction = Random::randomInt(3);
  
  if ( direction==0 ) { ++newX; }
  else if ( direction==1 ) { --newX; }
  else if ( direction==2 ) { ++newY; }
  else { --newY; }
  
  if ( map->isSafe(newX,newY) && map->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    map->remove(this);
    if (map->put(this,newX,newY) == false)
    {
      map->put(this,x,y);
    }
    
    if (Random::oneIn(10))
    {
      delete map->aLocalTile(x,y).footprint;
      map->aLocalTile(x,y).footprint = new Creature_Footprint;
    }
  }

}



  // LOCATION

  
  // KNOWLEDGE
  

    //Update knowledge with current instance.
  void Creature::updateKnowledge()
  {
    if ( knowledge == 0 || map==0 ) { return; }
    
    //idleCounter=0;


    knowledge->addTile(map,x,y);
  
    
    //Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(fullX,fullY,MAX_VIEW_RANGE/2);
    
    // if ( vVisibleTiles!=0 )
    // {
      // for (int i=0; i<vVisibleTiles->size(); ++i)
      // {
        // //std::cout<<"ADDING\n";
        // knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
        // delete (*vVisibleTiles)(i);
      // }
    // }
    

  }
  
    //Update knowledge with current instance.
  void Creature::updateKnowledgeIdle()
  {
    // Implement a basic delay to prevent idle flickering in.
    // ++idleCounter;
    // if (idleCounter > 10) { idleCounter=10; }
    
    // if ( vMovesToProcess.size() == 0 && vMovesToProcessSneak.size() == 0 ) { return; }
    // if ( knowledge == 0 ) { return; }
    // if ( tribe == 0 ) { return; }
    
    

    // //Update the LOS backlog during idle time.
    
      // //For now this simply wipes LOS from last turn.
    // knowledge->updateLOS();
    
    // if (vMovesToProcess.size() > 0)
    // {
      // vMovesToProcess.shuffle();
      
      // auto moveToProcess = vMovesToProcess(0);

      // knowledge->addTile(moveToProcess->x,moveToProcess->y);
      

      // Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE);
      
      // if ( vVisibleTiles!=0 )
      // {
        // for (int i=0; i<vVisibleTiles->size(); ++i)
        // {
          // //std::cout<<"ADDING\n";
          // knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
          // delete (*vVisibleTiles)(i);
        // }
      // }
      
      // delete moveToProcess;
      // vMovesToProcess.removeSlot(0);
    // }
    // else if ( vMovesToProcessSneak.size() > 0)
    // {
      // vMovesToProcessSneak.shuffle();
      
      // auto moveToProcess = vMovesToProcessSneak(0);

      // knowledge->addTile(moveToProcess->x,moveToProcess->y);
      

      // Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE,true);
      
      // if ( vVisibleTiles!=0 )
      // {
        // for (int i=0; i<vVisibleTiles->size(); ++i)
        // {
          // //std::cout<<"ADDING\n";
          // knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
          // delete (*vVisibleTiles)(i);
        // }
      // }
      
      // delete moveToProcess;
      // vMovesToProcessSneak.removeSlot(0);
    // }

    
  }
  

  
  bool Creature::hasSeen( World_Local* _map, int _x, int _y )
  {
    return false;
  }

// INHERITED FUNCTIONS


Texture* Creature::currentTexture ()
{	
	return &TEX_CREATURE_DEER;
}


// TABLE INTERFACE

std::string Creature::getColumn(std::string _column)
{
	return "?";
}
std::string Creature::getColumnType(std::string _column)
{
	return "string";
}

Texture* Creature_Footprint::currentTexture ()
{
  return &TEX_CREATURE_DEER_FOOTPRINT;
}

#endif