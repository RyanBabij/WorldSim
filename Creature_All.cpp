#pragma once
#ifndef WORLDSIM_CREATURE_ALL_CPP
#define WORLDSIM_CREATURE_ALL_CPP

/* WorldSim: Creature_All.cpp
	#include"Creature_All.cpp"

  Implementation of Creature_All.hpp.
*/

#include "Creature_All.hpp"


Creature_Bat::Creature_Bat()
{
  nPelt=1;
  nMeat=2;
}
Creature_Bat::~Creature_Bat()
{
}

  //_sex: 0 - Roll, 1 - Male, 2 - Female.
void Creature_Bat::init(const int _sex  /*=0*/ )
{
  
  if (_sex == 1) { isMale = true; }
  else if (_sex == 2) { isMale = false; }
  else { isMale = Random::flip(); }
  
	age=0;
  map=0;
  knowledge = new Creature_Knowledge;
  knowledge->init();


  // Attack list
  vAttack.push(&atkBatClaw);
  vAttack.push(&atkScreech);
}

void Creature_Bat::incrementTicks(int nTicks)
{
  if ( map == 0 ) { return; }
  
  if (distanceTo(playerCharacter) > MAX_VIEW_RANGE && Random::oneIn(10)==false)
  {
    return;
  }
  
  int closestDistance = -1;
  Character* closestCharacter = 0;
  
  if (map!=0)
  {
    for (int i=0;i<map->vCharacter.size();++i)
    {
      Character* target = map->vCharacter(i);
      
      if ( isUnderground == target->isUnderground && ( closestCharacter == 0 || distanceTo(target) < closestDistance ))
      {
        closestCharacter = target;
        closestDistance = distanceTo(target);
      }
    }
  }
  
  
  if ( closestDistance != -1 && closestDistance < 2 )
  {
    //std::cout<<"Closest character is "<<closestDistance<<" tiles away\n";
    if ( closestCharacter == playerCharacter )
    {
      Console("The bat "+vAttack(0)->actionName+" you.");
      attack(closestCharacter, vAttack(0));
      
      //Creature_Attack* atk = vAttack(0);
    }
  }
  // If near an enemy, move toward it.
  else if (closestDistance != -1 && closestDistance < 8 && knowledge)
  {
    Console("BAT MOVING");
    
    int newX = x;
    int newY = y;
    char moveDirection = '?';
    
          //Pathing_Local p;
    knowledge->p.init(map);
    knowledge->p.pathLocal(x, y, closestCharacter->x, closestCharacter->y, 5, false);
    
    if (knowledge->p.vPath.size() > 0)
    {
      moveDirection=knowledge->p.vPath.back();
      
      int direction = Random::randomInt(3);
  
      if ( moveDirection == 'E' )
      { direction = 0; }
      else if (moveDirection == 'N')
      { direction = 2; }
      else if (moveDirection == 'S')
      { direction = 3; }
      else if (moveDirection == 'W')
      { direction = 1; }
      
      if ( direction==0 ) { ++newX; }
      else if ( direction==1 ) { --newX; }
      else if ( direction==2 ) { ++newY; }
      else { --newY; }
      
      if ( map->isSafe(newX,newY) && map->data->aLocalTile(newX,newY).hasMovementBlocker() == false )
      {
        map->remove(this);
        if (map->put(this,newX,newY,isUnderground) == false)
        {
          map->put(this,x,y,isUnderground);
        }
        
        // if (Random::oneIn(10))
        // {
          // delete map->data->aLocalTile(x,y).footprint;
          // map->data->aLocalTile(x,y).footprint = new Creature_Footprint;
        // }
      }
      //updateKnowledge();
      
      
    }
  }
  else
  {
    wander();
  }
  
  
  //Bat AI will be simple. Fly around and attack any Characters nearby.
  
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

void Creature_Bat::attack (Creature* _target, Creature_Attack* _attack)
{
  if (_target==0) { return; }
  std::cout<<"BAT ATACK CRTR\n";
}

void Creature_Bat::attack (Character* _target, Creature_Attack* _attack)
{
  if (_target==0) { return; }
  std::cout<<"BAT ATACK CHR\n";
  _target->getAttacked(this,_attack);
}

void Creature_Bat::wander()
{
  if ( map==0 ) { return; }
  
  int newX = x;
  int newY = y;
  char moveDirection = '?';
  
  Character* closestThreat = 0;
  int threatDistance = 0;
  

  if (knowledge)
  {
      // PICK A DESTINATION IF NECESSARY
    if (map->isSafe(&(knowledge->currentGoal))==false ||
    (knowledge->currentGoal.x == x && knowledge->currentGoal.y ==y))
    {
      HasXY* randomDestination = map->getRandomTile();
      knowledge->currentGoal.set(randomDestination);
      knowledge->pathIndex=0;
      knowledge->p.vPath.clear();
      
      //Pathing_Local p;
      knowledge->p.init(map);
      knowledge->p.pathLocal(x, y, randomDestination->x, randomDestination->y, 20, false);

      moveDirection=knowledge->nextStep();

      delete randomDestination;
    }
    else // Continue on path
    {
      moveDirection = knowledge->nextStep();
      
      if (moveDirection==0) // There's no steps left, calculate more.
      {
        knowledge->p.init(map);

        bool pathingSuccess = knowledge->p.pathLocal(x, y, knowledge->currentGoal.x, knowledge->currentGoal.y, 10, false);
        
        if (pathingSuccess == false && knowledge->p.vPath.size() < 9 )
        { knowledge->currentGoal.set(-1,-1);
        }
        moveDirection = knowledge->nextStep();
      }
      
    }
  }

  int direction = Random::randomInt(3);
  
  if ( moveDirection == 'E' )
  { direction = 0; }
  else if (moveDirection == 'N')
  { direction = 2; }
  else if (moveDirection == 'S')
  { direction = 3; }
  else if (moveDirection == 'W')
  { direction = 1; }
  
  if ( direction==0 ) { ++newX; }
  else if ( direction==1 ) { --newX; }
  else if ( direction==2 ) { ++newY; }
  else { --newY; }
  
  if ( map->isSafe(newX,newY) && map->data->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    map->remove(this);
    if (map->put(this,newX,newY,isUnderground) == false)
    {
      map->put(this,x,y,isUnderground);
    }
    
    if (Random::oneIn(10))
    {
      //delete map->data->aLocalTile(x,y).footprint;
      //map->data->aLocalTile(x,y).footprint = new Creature_Footprint;
    }
  }
  updateKnowledge();

}

Texture* Creature_Bat::currentTexture()
{
  if (isAlive)
  {
    return &TEX_CREATURE_BAT;
  }
  return &TEX_CREATURE_BAT_DED;
}
std::string Creature_Bat::getName()
{
  return "Bat";
}


#endif
