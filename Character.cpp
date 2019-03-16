#pragma once
#ifndef WORLDSIM_CHARACTER_CPP
#define WORLDSIM_CHARACTER_CPP

/* WorldSim: Character
  #include "Character.cpp"
  
  Implementation of Character.hpp
*/

#include "World.hpp"

#include "Character.hpp"
#include "Character_Knowledge.hpp"
#include "Tribe.hpp"

const int MAX_CHILDREN = 5;

//#include <Graphics/Texture/Texture.hpp>
class Texture;

Character::Character()
{
	firstName="";
	lastName="";
	epithet="";
	
		/* Give every character a unique number */
	static long unsigned int STATIC_ID = 0;
	id = STATIC_ID++;

	isMale = true;
	age = -1;
	daysCounter=0;
	secondsCounter=0;
	
	isAlive=true;
	isMarried=false;
	isPregnant=false;
	pregnantCounter=0;
	
	actionPoints = 0;
	money=0;
	
	father=0;
	mother=0;
	spouse = 0;
	
	strength=0;
	agility=0;
	charisma=0;
	intelligence=0;
	perception=0;
	endurance=0;
	courage=0;
	
	health=0;
	hunger=0;
	
	birthLocation.setXY(0,0);
	deathLocation.setXY(0,0);
	
	tribe = 0;
  
  causeOfDeath = UNKNOWN;
  
  isFavourite=false;
  
  knowledge=0;
  
  worldX = -1;
  worldY = -1;
  
  idleCounter=0;
  
  isSneaking=false;
  
  skillFishing=0;
  skillMarksmanship=0;
	
  nPelt = 1;
  nMeat = 1;
	//enum enumCauseOfDeath { UNKNOWN=0, STARVATION=1, OLD_AGE=2 };
  
  map = 0;
	
}

  //_sex: 0 - Roll, 1 - Male, 2 - Female.
void Character::init(const int _sex /* =0 */)
{
  
  if (_sex == 1) { isMale = true; }
  else if (_sex == 2) { isMale = false; }
  else { isMale = Random::flip(); }
  
	firstName = globalNameGen.generateName();
	lastName = globalNameGen.generateName();
	age=0;
	
  // There will be some differences in stats between genders, but
  // both genders will be able to potentially obtain 100 on every stat.
  // This is done to affect high-level gameplay, for example gender
  // distribution in battles.
  // TODO: Stats should be affected by age. However that will be calculated
  // by using age as a modifier. Not by modifying the base stat.
  
  if ( isMale )
  {
    strength=Random::multiRoll(8,10,true)+20;
    agility=Random::multiRoll(10,10,true);
    charisma=Random::multiRoll(10,10,true);
    intelligence=Random::multiRoll(10,10,true);
    perception=Random::multiRoll(10,10,true);
    endurance=Random::multiRoll(10,10,true);
    courage=Random::multiRoll(10,10,true);
  }
  else
  {
    strength=Random::multiRoll(10,10,true);
    agility=Random::multiRoll(8,10,true)+20;
    charisma=Random::multiRoll(10,10,true);
    intelligence=Random::multiRoll(10,10,true);
    perception=Random::multiRoll(10,10,true);
    endurance=Random::multiRoll(10,10,true);
    courage=Random::multiRoll(10,10,true);
  }

	
	health=10;
	hunger=0;
	
	tribe=0;
  
  isSneaking=false;
	
	dateOfBirth.set(world.calendar);
  
  //knowledge = new Character_Knowledge;
  //knowledge->init();
}

  //ITEM FUNCTIONS
  
void Character::giveItem(Item* _item)
{
  if (_item==0) { return; }
  world(worldX,worldY)->put(_item,x,y);
  //world(worldX,worldY)->vItem.push(_item);
}

void Character::consume(Item* _item)
{
  Console("NUM NUM NUM");
  hunger-=_item->hungerRestore;
  if (hunger<0) { hunger=0; }
  world(worldX,worldY)->erase(_item);
  
    // We should probably consider ways to avoid this check, because it is rarely true.
    // However the only possibility I can think of is making the player their own special class.
  if (this == playerCharacter)
  {
    removeFromInventoryGrid(_item);
  }
}

void Character::removeFromInventoryGrid(Item* _item) /* Player-only function */
{
    for (int _y=0;_y<10;++_y)
    {
      for (int _x=0;_x<10;++_x)
      {
        if (inventoryGrid[_x][_y]==_item)
        {
          inventoryGrid[_x][_y]=0;
        }
      }
    }
}

std::string Character::getFullName() const
{
	return firstName + " " + lastName;
}

std::string Character::getBiography()
{
	
	std::string biography;
	
	biography = getFullName();
	
	if ( father == 0 || mother == 0 )
	{
		biography += " was divinely concieved.\n\n";
	}
	else
	{
		biography += " was the RELATION of " + father->getFullName() + " and " + mother->getFullName() + ".\n\n";
	}
	
	
	// The character parents
	// 
	
	
	return getFullName();
}

void Character::incrementTicks(int nTicks)
{
	secondsCounter+=nTicks;
	
  hunger+=nTicks;
  
  if (hunger > MAX_HUNGER) { hunger = MAX_HUNGER; /* isAlive=false; */ }
  
	while(secondsCounter>=86400)
	{ 
		++daysCounter;
		secondsCounter-=86400;
	}
	//std::cout<<"Dayscounter: "<<daysCounter<<".\n";
	while(daysCounter >=360)
	{
		age++;
		daysCounter-=360;
		
		// if ( hunger > 0 )
		// {
			// // Chance of dying at 100 hunger = 1 in 100.
			// // Chance of dying at 1 hunger = 1 in 5000
			// int starvationChance = (101-hunger)*10;
			
			// if ( Random::oneIn(starvationChance) )
			// {
				// //die();
			// }
		// }
	}
}

void Character::wander()
{
  if ( map==0 ) { return; }
  
  int newX = x;
  int newY = y;
  char moveDirection = '?';
  
  
  
  if (knowledge)
  {
      // PICK A DESTINATION IF NECESSARY
    if (map->isSafe(&(knowledge->currentGoal))==false ||
    (knowledge->currentGoal.x == x && knowledge->currentGoal.y ==y))
    {
      HasXY* randomDestination = map->getRandomTile();
      
      knowledge->currentGoal.set(randomDestination);
      
      Pathing_Local p;
      p.init(map);
      p.pathLocal(x, y, randomDestination->x, randomDestination->y, 10, false);
          
      if (p.vPath.size() > 0)
      {
        moveDirection=p.vPath(0);
      }
      else // Go somewhere else.
      { knowledge->currentGoal.set(-1,-1);
      }
          
      delete randomDestination;
    }
    else
    {
      Pathing_Local p;
      p.init(map);
      bool pathingSuccess = p.pathLocal(x, y, knowledge->currentGoal.x, knowledge->currentGoal.y, 10, false);
      
      if (pathingSuccess == false && p.vPath.size() < 9 )
      { knowledge->currentGoal.set(-1,-1);
      }
      
      
      if (p.vPath.size() > 0)
      {
        moveDirection=p.vPath(0);
      }
      else { knowledge->currentGoal.set(-1,-1); }
    }
  }
  else { std::cout<<"noknow\n"; }

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
  
  if ( map->isSafe(newX,newY) && map->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    map->remove(this);
    if (map->put(this,newX,newY) == false)
    {
      map->put(this,x,y);
    }
    
    if (Random::oneIn(10))
    {
      //delete map->aLocalTile(x,y).footprint;
      //map->aLocalTile(x,y).footprint = new Creature_Footprint;
    }
  }
  //updateKnowledge();

}

void Character::die(enumCauseOfDeath _causeOfDeath /* =UNKNOWN */)
{
	isAlive = false;
	dateOfDeath.set(world.calendar);
  causeOfDeath = _causeOfDeath;
	
	if ( tribe != 0 )
	{
		deathLocation.setXY(tribe->worldX,tribe->worldY);
	}
	else
	{
		std::cout<<"no tribe...\n";
	}
}

bool Character::marry(Character* c)
{
	if (c==0) {return false;}
	
	isMarried = true;
	c->isMarried=true;
	spouse = c;
	c->spouse = this;
	
	if (isMale == true && c->isMale == false)
	{
		c->lastName = lastName;
	}
	else if (isMale == false && c->isMale == true)
	{
		lastName = c->lastName;
	}

	dateOfMarriage.set(world.calendar);
	c->dateOfMarriage.set(world.calendar);
  
  // If the spouse is from a different tribe, move them to the new one.
  if ( tribe != c->tribe )
  {
    c->tribe->removeCharacter(c);
    tribe->addCharacter(c);
  }
	
	return true;
}

Vector <Character*> * Character::getDescendants(Vector <Character*> * vDescendants)
{
	if (vDescendants==0) {return 0;}
	
	/* GET ALL CHILDREN, THEN GET ALL CHILDREN'S CHILDREN. RETURN 0 IF NO CHILDREN. */
	if (vChildren.size() == 0)
	{
		return vDescendants;
	}
	
	//Vector <Character*> * vChildren = new Vector <Character*>;
	//auto vDescendants = new Vector <Character*>;
	
	// for (auto i=0;i<vChildren.size();++i)
	// {
	// }
	
	//for(auto const& value: vChildren.data) { }
	//for(auto const& v: vChildren) { std::cout<<"Person: "<<v->firstName<<".\n"; }
	
	
	// Push all character's children onto vector.
	for(auto const& v: vChildren) { vDescendants->push(v); v->getDescendants(vDescendants); }
	
	
	//Push all children's descendants onto vector.
	//for(auto const& v: vChildren) { v->getDescendants(vDescendants); }
	
	vDescendants->removeDuplicates();
	
	return vDescendants;
}

Vector <Character*> * Character::getRelatives()
{
	Vector <Character*> vCloseAncestors;
	
	//auto vCloseAncestors = new Vector <Character*>;
	auto vAncestors = new Vector <Character*>;
	
		// The character cannot marry any descendants of their grandparents. So first we get all 4 grandparents. If there are no grandparents, we work from parents. If there are no parents, we work from this.
	if ( father != 0)
	{
		if (father->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(father->mother != 0)
		{
			vCloseAncestors.push(father->mother);
		}
		if ( father->father == 0 && father->mother == 0)
		{
			vCloseAncestors.push(father);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}
	if ( mother != 0)
	{
		if (mother->father != 0)
		{
			vCloseAncestors.push(father->father);
		}
		if(mother->mother != 0)
		{
			vCloseAncestors.push(mother->mother);
		}
		if ( mother->father == 0 && mother->mother == 0)
		{
			vCloseAncestors.push(mother);
		}
	}
	else
	{
		vCloseAncestors.push(this);
	}
	
		// Get all descendants of all these relatives.
	for(auto const& v: vCloseAncestors) { vAncestors->push(v); v->getDescendants(vAncestors); }
	
	vAncestors->removeDuplicates();
	
	return vAncestors;
}

void Character::starve()
{
	//--health;	
	++hunger;
	
	if (hunger > 100)
	{
		hunger = 100;
	}
	
	//if ( health <= 0)
	//{
		//std::cout<<"Starved to death.\n";
	//	isAlive = false;
	//}
}

bool Character::canMarry(Character* c)
{
	//std::cout<<"Checking if can marry.\n";
	
	
		//Cannot marry null pointer or yourself or underage or same sex.
	if(c==0 || c==this || isAlive==false || c->isAlive==false || age<16 || c->age<16 || isMale==c->isMale || spouse !=0 || c->spouse!=0 ) { return false; }
	
	//auto vRelatives = getRelatives();
	//if ( vRelatives->contains(c) )
	//{
	//	delete vRelatives;
	//	return false;
	//}
	
	return true;
}

Character* Character::giveBirth()
{
	
	Character * babby = new Character;
	babby->init();
	babby->lastName = lastName;
		// Inherit tribe membership from mother I guess.
	babby->tribe = tribe;
	
	vChildren.push(babby);
	spouse->vChildren.push(babby);
	
	if(isMale)
	{
		babby->father=this;
		if ( spouse!=0 )
		{
			babby->mother=this->spouse;
		}
	}
	else
	{
		babby->mother=this;
		if ( spouse!=0 )
		{
			babby->father=this->spouse;
		}
	}
	
	return babby;
}

void Character::attack(Character* target)
{
  if (target == 0 || target->isAlive == false ) { return; }
  //For now we will just use strength to calculate outcome.
  
  if ( strength >= target->strength && Random::flip() )
  {
    
    
    //std::cout<<firstName<<" killed "<<target->firstName<<".\n";
    target->die(COMBAT);
    vKills.add(target);
    return;
  }
  else if ( strength < target->strength )
  {
    if ( Random::flip() )
    {
      //std::cout<<target->firstName<<" killed "<<firstName<<" in self-defence.\n";
      target->vKills.add(this);
      die(COMBAT);
    }
    else
    {
    }
    
  }
  //std::cout<<"Attack missed.\n";
  
  
}







  // LOCATION
  
  bool Character::isOnMap(int _mapX , int _mapY)
  {
    //For now we are assuming the Character is on the same map as his tribe.
    
    return (worldX == _mapX && worldY == _mapY);
    
    // if ( tribe != 0 )
    // {
      // return tribe->isOnMap(_mapX,_mapY);
    // }
    
    // return false;
  }

  
  // KNOWLEDGE
  
void Character::initialiseKnowledge()
{
  if ( knowledge == 0 )
  {
    std::cout<<"INIT\n";
    knowledge = new Character_Knowledge;
    knowledge->init();
    
    if ( tribe != 0 )
    {
      worldX = tribe->worldX;
      worldY = tribe->worldY;
      
      fullX = tribe->worldX * LOCAL_MAP_SIZE;
      fullY = tribe->worldY * LOCAL_MAP_SIZE;
    }
  }
}
  
  char Character::hasSeen( World_Local* _map, int _x, int _y )
  {
    if (knowledge==0) { return false; }
    
    
    return ( knowledge->hasSeen(_map,_x,_y) );
  }

    //Update knowledge with current instance.
  void Character::updateKnowledge()
  {
    if ( knowledge == 0 ) { return; }
    if ( tribe == 0 ) { return; }
    
    idleCounter=0;

    //learn about all surrounding tiles.
    
    // The basic 3x3 box of guaranteed sight.
    
    
      //For now this simply wipes LOS from last turn.
    //knowledge->updateLOS();
    
    //knowledge->addTile(world(worldX,worldY), x,y);
    
    //std::cout<<"Adding tile: "<<fullX<<", "<<fullY<<"\n";
    knowledge->addTile(fullX,fullY);
    
    // knowledge->addTile(tribe->getCurrentMap(), x-1,y);
    // knowledge->addTile(tribe->getCurrentMap(), x+1,y);
    // knowledge->addTile(tribe->getCurrentMap(), x,y-1);
    // knowledge->addTile(tribe->getCurrentMap(), x-1,y-1);
    // knowledge->addTile(tribe->getCurrentMap(), x+1,y-1);
    // knowledge->addTile(tribe->getCurrentMap(), x,y+1);
    // knowledge->addTile(tribe->getCurrentMap(), x-1,y+1);
    // knowledge->addTile(tribe->getCurrentMap(), x+1,y+1);
    
    // Now we need to run a proper line of sight algorithm. We can start with raytrace.
    //MAX_VIEW_RANGE
    
    // for (int i=0;i<MAX_VIEW_RANGE;++i)
    // {
      // int actualTileX = x+i;
      
      // if (tribe->getCurrentMap()->isBlockingView(x+i,y))
      // {
        // break;
      // }
      // //knowledge->addTile(tribe->getCurrentMap(), x+i,y);
      
    // }
    
    
    //Vector <HasXY*> * vVisibleTiles = world(worldX,worldY)->rayTraceLOS(x,y,MAX_VIEW_RANGE);
    //Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world(worldX,worldY)->rayTraceLOS(fullX,fullY,MAX_VIEW_RANGE);
    
    //std::cout<<"coordinates to raytrace from: "<<fullX<<", "<<fullY<<".\n";
    
    Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(fullX,fullY,MAX_VIEW_RANGE/2);
    
    if ( vVisibleTiles!=0 )
    {
      for (int i=0; i<vVisibleTiles->size(); ++i)
      {
        //std::cout<<"ADDING\n";
        knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
        delete (*vVisibleTiles)(i);
      }
    }
    
    // Add this tile for later processing when idle.
    if ( isSneaking )
    { vMovesToProcessSneak.push(new HasXY2 <unsigned long int> (fullX,fullY)); }
    else
    { vMovesToProcess.push(new HasXY2 <unsigned long int> (fullX,fullY)); }
  }
  
    //Update knowledge with current instance.
  void Character::updateKnowledgeIdle()
  {
    // Implement a basic delay to prevent idle flickering in.
    ++idleCounter;
    if (idleCounter > 10) { idleCounter=10; }
    
    if ( vMovesToProcess.size() == 0 && vMovesToProcessSneak.size() == 0 ) { return; }
    if ( knowledge == 0 ) { return; }
    if ( tribe == 0 ) { return; }
    
    

    //Update the LOS backlog during idle time.
    
      //For now this simply wipes LOS from last turn.
    knowledge->updateLOS();
    
    if (vMovesToProcess.size() > 0)
    {
      vMovesToProcess.shuffle();
      
      auto moveToProcess = vMovesToProcess(0);

      knowledge->addTile(moveToProcess->x,moveToProcess->y);
      

      Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE);
      
      if ( vVisibleTiles!=0 )
      {
        for (int i=0; i<vVisibleTiles->size(); ++i)
        {
          //std::cout<<"ADDING\n";
          knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
          delete (*vVisibleTiles)(i);
        }
      }
      
      delete moveToProcess;
      vMovesToProcess.removeSlot(0);
    }
    else if ( vMovesToProcessSneak.size() > 0)
    {
      vMovesToProcessSneak.shuffle();
      
      auto moveToProcess = vMovesToProcessSneak(0);

      knowledge->addTile(moveToProcess->x,moveToProcess->y);
      

      Vector <HasXY2 <unsigned long int> *> * vVisibleTiles = world.rayTraceLOS(moveToProcess->x,moveToProcess->y,MAX_VIEW_RANGE,true);
      
      if ( vVisibleTiles!=0 )
      {
        for (int i=0; i<vVisibleTiles->size(); ++i)
        {
          //std::cout<<"ADDING\n";
          knowledge->addTile((*vVisibleTiles)(i)->x,  (*vVisibleTiles)(i)->y);
          delete (*vVisibleTiles)(i);
        }
      }
      
      delete moveToProcess;
      vMovesToProcessSneak.removeSlot(0);
    }

    
  }
  
// INHERITED FUNCTIONS


Texture* Character::currentTexture ()
{
  if ( isAlive == false ) { return &TEX_NPC_DED; }
	return &TEX_NPC;
}

std::string Character::getName()
{
  return "Character";
}

// COUT OPERATOR

std::ostream& operator<<(std::ostream& stream, const Character& c)
{
	return stream<<c.getFullName();
}

// TABLE INTERFACE

/* TABLE INTERFACE */

std::string Character::getColumn(std::string _column)
{
	if ( _column=="firstname" )
	{
		return firstName;
	}
	else if ( _column=="lastname" )
	{
		return lastName;
	}
	if ( _column=="age" )
	{
		return DataTools::toString(age);
	}
	if ( _column=="sex" )
	{
		if (isMale) { return "Male"; }
    return "Female";
	}
  
	if ( _column=="strength" )
	{
		return DataTools::toString(strength);
	}
  
	if ( _column=="kills" )
	{
		return DataTools::toString(vKills.size());
	}
  
  if ( _column=="favourite" )
  {
    if ( isFavourite ) { return "T"; }
    return "";
  }
	// if ( _column=="coordinates" )
	// {
		// return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	// }
	// if ( _column=="territory" )
	// {
		// return DataTools::toString(vTerritory.size());
	// }
	// // else if ( _column=="id" )
	// // {
		// // return DataTools::toString(id);
	// // }
	
	return "?";
}
std::string Character::getColumnType(std::string _column)
{
	if ( _column == "age" || _column == "territory" || _column == "food" || _column == "strength" || _column == "kills" )
	{
		return "number";
	}
	return "string";
}

#endif