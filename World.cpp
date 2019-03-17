#pragma once
#ifndef WORLDSIM_WORLD_CPP
#define WORLDSIM_WORLD_CPP

/* World.cpp
	#include "World.cpp"
	Implementation of "World.hpp".
*/

//#include<set> /* For raytraceLOS */

#include <NameGen/NameGen.hpp>
#include <WorldGenerator/WorldGenerator2.hpp>

#include <File/FileLog.hpp>

#include <Time/Timer.hpp> // To manage work chunks and benchmark worldgen time.

#include <thread>
#include <mutex>
#include <atomic> // Variables which work with threads

#include "Civ.hpp"
  #include "Civ_Dwarven.hpp"

#include "Tribe.hpp"
  #include "Tribe_Human.hpp"
  #include "Tribe_Dwarven.hpp"
  #include "Tribe_Elf.hpp"

#include "Character.hpp"

#include "Settlement.hpp"
  #include "Settlement_Dwarven.hpp"

#include "WorldObjectGlobal.hpp"
#include <WorldGenerator/Biome.hpp>

#include "WorldObject.hpp"
  #include "WorldObject_Tree.hpp"
  #include "WorldObject_Rock.hpp"
  #include "WorldObject_Sign.hpp"

#include "Item.hpp"

class Item;

World::World(): SaveFileInterface(), seaLevel(0), mountainLevel(0)
{
	random.seed();
	
	nX=-1;
	nY=-1;

  maximumX = 0;
  maximumY = 0;
  
	active=false;
	generated=false;
	
	ticksBacklog=0;
	relinquishTimer.init();
	
	name="";
  
	dailyCounter=0;
	monthlyCounter=0;
  
  //localX = -1;
  //localY = -1;
  
  queryWorldX = -1;
  queryWorldY = -1;
  
  worldFilePath = "";
  
  isRaining=false;
}

World_Local* World::operator() (const int _x, const int _y)
{
  for (int i=0;i<vWorldLocal.size();++i)
  {
    if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
    {
      return vWorldLocal(i);
    }
  }
  
  // The local map isn't in memory, therefore we need to load it up.
  // For now we just generate it from scratch.
  generateLocal(_x,_y);

  for (int i=0;i<vWorldLocal.size();++i)
  {
    if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
    {
      return vWorldLocal(i);
    }
  }
  
  return 0;
}

inline LocalTile* World::operator() (unsigned long int _x, unsigned long int _y)
{
  //std::cout<<"WORLD ABSOLUTE QUERY\n";
  
  // CONVERT COORDINATES TO RELATIVE.
  int gX = 0;
  int gY = 0;
  int lX = 0;
  int lY = 0;
  
  if ( absoluteToRelative(_x,_y,&gX,&gY,&lX,&lY) == false )
  {
    return 0;
  }
  
  // Check if local map is already loaded.
  for (int i=0;i<vWorldLocal.size();++i)
  {
    if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY )
    {
      return &vWorldLocal(i)->aLocalTile(lX,lY);
    }
  }
  // The local map isn't in memory, therefore we need to load it up.
  // For now we just generate it from scratch.
  generateLocal(gX,gY);

  for (int i=0;i<vWorldLocal.size();++i)
  {
    if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY )
    {
      return &vWorldLocal(i)->aLocalTile(lX,lY);
    }
  }
  
  return 0;
}

  
  // COORDINATE CONVERSIONS
  
    // Pass absolute coordinates and recieve world and local relative coordinates.
    // This currently doesn't work if tileX or tileY are negative.
bool World::absoluteToRelative (const unsigned long int _absoluteX, const unsigned long int _absoluteY, int * _globalX, int * _globalY, int * _localX, int * _localY)
{
  if ( _globalX == 0 || _globalY == 0 || _localX == 0 || _localY == 0 )
  { return false; }
  
  // overflow check
  
  if (_absoluteX > maximumX || _absoluteY > maximumY )
  {
    *_globalX = 0;
    *_globalY = 0;
    *_localX = 0;
    *_localY = 0;
    return false;
  }
  
  *_globalX = _absoluteX / LOCAL_MAP_SIZE;
  *_globalY = _absoluteY / LOCAL_MAP_SIZE;
  
  if ( *_globalX == 0 )
  { *_localX = _absoluteX; }
  else
  { *_localX = _absoluteX % LOCAL_MAP_SIZE; }
  
  if ( *_globalY == 0 )
  { *_localY = _absoluteY; }
  else
  { *_localY = _absoluteY % LOCAL_MAP_SIZE; } 
  return true;

}

  // Return true if the map for this tile has been generated
bool World::isGenerated(unsigned long int _absoluteX, unsigned long int _absoluteY)
{
  int gX,gY,lX,lY;
  if (absoluteToRelative(_absoluteX,_absoluteY,&gX,&gY,&lX,&lY))
  {
    for (int i=0;i<vWorldLocal.size();++i)
    {
      if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY )
      {
        return true;
      }
    }
  }
  return false;
}

void World::nameRegions()
{
}

bool World::putObject(WorldObjectGlobal* _object, int x /* =-1 */, int y /* =-1 */)
{
	if ( x != -1 && y != -1 )
	{
		_object->worldX=x;
		_object->worldY=y;
	}
	
	if ( isSafe(_object->worldX, _object->worldY) )
	{
		//return false;
		
		if(vWorldObjectGlobal.contains(_object)==false)
		{
			vWorldObjectGlobal.push(_object);
			return true;
		}
		else
		{
			std::cout<<"Debug: WorldObjectGlobal was already placed.\n";
		}
	}
	
	std::cout<<"ERROR: Error placing worldobject.\n";

	return false;
}

bool World::removeObject(WorldObjectGlobal* _object)
{
  vWorldObjectGlobal.remove(_object);
  return false;
}

#include <Time\RyanTime.hpp>


Vector <WorldObjectGlobal*>* World::getNeighboringObjects(WorldObjectGlobal* _obj)
{

	Vector <WorldObjectGlobal*>* vNearbyObjects = new Vector <WorldObjectGlobal*>;
	
	for (int i=0;i<vWorldObjectGlobal.size();++i)
	{
		WorldObjectGlobal* _o = vWorldObjectGlobal(i);
		
		if (_o != _obj)
		{
			if ( _obj->distanceTo(_o) <= 1 )
			{
				//std::cout<<"Nearby object detected.\n";
				vNearbyObjects->push(_o);
			}
		}
		
	}

	return vNearbyObjects;
}

// void World::generateCitizens(const int nCitizens)
// {
	// for (int i=0;i<nCitizens;++i)
	// {
		// Citizen citizen;
		// citizen.init();
		// std::cout<<"Citizen: "<<citizen.getFullName()<<".\n";
	// }
// }

bool World::isSafe(int _x, int _y)
{
	return ( _x >= 0 && _x < nX && _y >= 0 && _y < nY );
}

bool World::isSafe(unsigned long int _x, unsigned long int _y)
{
	return ( _x <= maximumX && _y <= maximumY );
}


Character* World::getRandomCharacter()
{
  //Get total world population.
  
  Character* chosenChar = 0;
  
  int totalPop = 0;
  for ( int i=0; i<vTribe.size(); ++i )
  {
    totalPop += vTribe(i)->vCharacter.size();
  }
  
  int randomChar = Random::randomInt(totalPop-1);
  
  for (int i=0;i<vTribe.size();++i)
  {
    if (randomChar < vTribe(i)->vCharacter.size() )
    {
      return vTribe(i)->vCharacter(randomChar);
    }
    else
    {
      randomChar -= vTribe(i)->vCharacter.size();
    }
  }
  
  return 0;
}

    //Return a vector of coordinates visible from the given location.
Vector <HasXY2 <unsigned long int> *> * World::rayTraceLOS (unsigned long int _x, unsigned long int _y, const int RANGE, const bool isSneaking = false)
{
  if (RANGE <= 0) { return 0; }
  
  if (_x > maximumX || _y > maximumY )
  { return 0; }
  
  //Step 1: Get all raytrace coordinates.
  Vector <HasXY2 <unsigned long int> *> rayTraceCoordinates;
  
  unsigned long int rayX = _x-RANGE;
  
  //overflow check
  if ( rayX > maximumX )
  { rayX = 0; }
  
  unsigned long int rayY = _y-RANGE;
  
  //overflow check
  if ( rayY > maximumY )
  { rayY = 0; }
  
  unsigned long int rayMaxX = _x+RANGE;
  
  //overflow check
  if ( rayMaxX > maximumX )
  { rayMaxX = maximumX; }
  
  unsigned long int rayMaxY = _y+RANGE;
  
  //overflow check
  if ( rayMaxY > maximumY )
  { rayMaxY = maximumY; }

  unsigned long int tempX = rayX;
  unsigned long int tempY = rayY;
    
  auto hXY = new HasXY2 <unsigned long int>;
  
  //std::set <HasXY2 <unsigned long int> * > sInt;
  //setOfNumbers.insert("first");
    
  rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,tempY) );
  
  while (tempX <= rayMaxX)
  {
    rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,rayY) );
    rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,rayMaxY) );
    ++tempX;
  }
  
  // On the Y pass we prevent doing the corners again.
  ++tempY;
  while (tempY < rayMaxY)
  {
    rayTraceCoordinates.push( new HasXY2 <unsigned long int> (rayX,tempY) );
    rayTraceCoordinates.push( new HasXY2 <unsigned long int> (rayMaxX,tempY) );
    ++tempY;
  }
  
  // We now have a list of coordinates to raytrace.
  // //std::cout<<"RayTrace Coordinats size: "<<rayTraceCoordinates.size()<<".\n";
  
  auto vVisibleTiles = new Vector <HasXY2 <unsigned long int> *>;
  
  
  for (int i=0;i<rayTraceCoordinates.size();++i)
  {
    rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
    
    // Very bad implementation of peeking. Should be optimised in future.
    if (isSneaking)
    {
      rayTrace (_x+1,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
      rayTrace (_x-1,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
      rayTrace (_x,_y+1,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
      rayTrace (_x,_y-1,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
    }

    
  }
  
  return vVisibleTiles;
}

void World::rayTrace (unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2, Vector <HasXY2 <unsigned long int> *> * vVisibleTiles)
{
  // Old code from ECHO
  
  int xDiff = 0;
  if ( _x1 > _x2 )
  { xDiff = _x2 - _x1; }
  else if ( _x2 > _x1 )
  { xDiff = _x1 - _x2; }

  int yDiff = 0;
  if ( _y1 > _y2 )
  { yDiff = _y2 - _y1; }
  else if ( _y2 > _y1 )
  { yDiff = _y1 - _y2; }


  double slope = BasicMath::getSlopeULI(_x1,_y1,_x2,_y2);

  // IF SLOPE IS INFINITY, CHANGE VALUE TO 0.
  if ( slope == std::numeric_limits<double>::infinity() )
  { slope=0; }

    // SPECIAL CASE: 1 TILE.
    // REVEAL TILE STANDING ON.
  if ( (_x1==_x2) && (_y1==_y2) )
  {
    if ( isSafe(_x1,_y2) )
    {
      auto temp = new HasXY2 <unsigned long int> (_x1,_y1);
      
      // Proper optimisation would use a set, however for now we'll just do a pre-check
      // to prevent storing duplicates.
      // Update: I removed the dupe detection as it significantly hurts performance.
      // The renderer must iterate over the dupes, however it seems to be able to handle it better for now.
      //if ( vVisibleTiles->contains(temp) == false )
      if (true)
      {
        vVisibleTiles->push( temp );
      }
      else
      {
        //std::cout<<"DUPE DETECTED\n";
        delete temp;
      }
      
      
    }
  }

    // SPECIAL CASE: UP/DOWN
  else if (_x1 == _x2)
  {
    do
    {
      if ( isSafe(_x1,_y1) )
      {
        auto temp = new HasXY2 <unsigned long int> (_x1,_y1);
        
        //if ( vVisibleTiles->containsPtr(temp) == false )
        if (true)
        {
          vVisibleTiles->push( temp );
        }
        else
        {
          //std::cout<<"DUPE DETECTED\n";
          delete temp;
        }
        
        LocalTile * lt = (*this)(_x1, _y1);
        if (lt != 0 && lt->hasViewBlocker())
        { break; }
      }
      if ( _y1 < _y2 )
      { ++_y1; }
      else
      { --_y1; }
    } 
    while( _y1 != _y2 );
    
    // Final case
    if (_y1 == _y2)
    { vVisibleTiles->push(new HasXY2 <unsigned long int> (_x1,_y1) );
    }
    
  }
    //SHALLOW SLOPE
  else if ( xDiff <= yDiff )
  {
    if (_x1>_x2)
    { slope*=-1; }

    double currentY = _y1;

    //while (_x1 != _x2 )
    do
    {

      unsigned long int roundedY = 0;

      if ( _y1 > _y2 )
      {
        roundedY = floor(currentY);
      }
      else if ( _y1 < _y2 )
      {
        roundedY = ceil(currentY);
      }
      else
      {
        roundedY = round(currentY);
      }

      if ( isSafe(_x1,roundedY) == true )
      {
        auto temp = new HasXY2 <unsigned long int> (_x1,roundedY);
        
        //if ( vVisibleTiles->containsPtr(temp) == false )
        if (true)
        {
          vVisibleTiles->push( temp );
        }
        else
        {
          //std::cout<<"DUPE DETECTED\n";
          delete temp;
        }
        
        LocalTile * lt = (*this)(_x1, roundedY);
        if (lt != 0 && lt->hasViewBlocker())
        { break; }

        currentY+=slope;

      }
      if (_x1<_x2)
      { ++_x1; }
      else
      { --_x1; }
    }
    while (_x1 != _x2 );
    
    // Final case
    if (_x1 == _x2)
    { vVisibleTiles->push(new HasXY2 <unsigned long int> (_x1,_y2) );
    }
    //Final case
    //vVisibleTiles->push(new HasXY (_x1,_y1) );
  }
    //STEEP SLOPE.
  else
  {
    slope = BasicMath::getSlopeULI(_y1,_x1,_y2,_x2);

    if (_y1>_y2)
    { slope*=-1; }

    double currentX = _x1;

    //while (_y1 != _y2 )
    do
    {
      unsigned long int roundedX=0;

      if ( _x1 > _x2 )
      { roundedX = floor(currentX); }
      else if ( _x1 < _x2 )
      { roundedX = ceil(currentX); }
      else
      { roundedX = round(currentX); }

      if ( isSafe(roundedX,_y1) == true )
      {
        auto temp = new HasXY2 <unsigned long int> (roundedX,_y1);
        
        //if ( vVisibleTiles->containsPtr(temp) == false )
        if (true)
        {
          vVisibleTiles->push( temp );
        }
        else
        {
          //std::cout<<"DUPE DETECTED\n";
          delete temp;
        }
        
        LocalTile * lt = (*this)(roundedX, _y1);
        if (lt != 0 && lt->hasViewBlocker())
        { break; }

        currentX+=slope;

      }
      if (_y1<_y2)
      { ++_y1; }
      else
      { --_y1; }
    }
    while (_y1 != _y2 );
    
    // Final case
    if (_y1 == _y2)
    { vVisibleTiles->push(new HasXY2 <unsigned long int> (_x2,_y1) );
    } 
    //Final case
    //vVisibleTiles->push(new HasXY (_x1,_y1) );
  }

}



void World::generateTribes( int nTribesHuman = DEFAULT_NUMBER_TRIBES_HUMAN, int nTribesDwarven = DEFAULT_NUMBER_TRIBES_DWARVEN, int nTribesElven = DEFAULT_NUMBER_TRIBES_ELVEN)
{	
	std::cout<<"Generate tribes\n";

  if (nTribesHuman < 0 ) { nTribesHuman = 0; }
  if (nTribesDwarven < 0 ) { nTribesDwarven = 0; }
  if (nTribesElven < 0 ) { nTribesElven = 0; }
  
	if (nTribesHuman < 0 || nTribesHuman > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }
	if (nTribesDwarven < 0 || nTribesDwarven > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }
  if (nTribesElven < 0 || nTribesElven > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }


  std::cout<<"Generate humans: ";
	for (int i=0;i<nTribesHuman;++i)
	{
    std::cout<<".";
		Tribe_Human * t = new Tribe_Human;
		t->init(this);
		if (t->spawn() == false)
		{
      std::cout<<" FAIL ";
			delete t;
    }
    else
    {
      t->generateCouples(7);
    }
	}
  std::cout<<"\n";
  
  std::cout<<"Generate dwarves: ";
	for (int i=0;i<nTribesDwarven;++i)
	{
    std::cout<<".";
		Tribe_Dwarven * t = new Tribe_Dwarven;
		t->init(this);
		if (t->spawn() == false)
		{
      std::cout<<" FAIL ";
			delete t;
    }
    else
    {
      t->generateCouples(7);
    }
	}
  std::cout<<"\n";
  
  std::cout<<"Generate elves: ";
	for (int i=0;i<nTribesElven;++i)
	{
    std::cout<<".";
		Tribe_Elf * t = new Tribe_Elf;
		t->init(this);
		if (t->spawn() == false)
		{
      std::cout<<" FAIL ";
			delete t;
    }
    else
    {
      t->generateCouples(7);
    }
	}
  std::cout<<"\n";
 
	
	std::cout<<"END Generate tribes\n";
	
	
}


  // Remove tribe object and replace it with a Civ and Settlement (whereever the Tribe is standing.
void World::evolveToCiv( Tribe * _tribe )
{
  if ( _tribe == 0 ) { return; }
  
  Console (Stream() << "Tribe: " << _tribe->name << " became a Civ.");
  
  if ( _tribe->race == DWARVEN)
  {
    //std::cout<<"AYYY DWARVEN\n";
    Civ_Dwarven * civ = new Civ_Dwarven;
    
    
    civ->name = _tribe->name;
    civ->world = this;
    
    //std::cout<<"Civ name is: "<<civ->name<<".\n";
    
    civ->setColour(_tribe->colourRed,_tribe->colourGreen,_tribe->colourBlue);
    
    civ->vCharacter.copy(&_tribe->vCharacter);
    
    //std::cout<<"Copied: "<<civ->vCharacter.size() <<" characters.\n";
    
    Settlement_Dwarven * s = new Settlement_Dwarven;
    s->world = this;
    s->vCharacter.copy(&_tribe->vCharacter);
    putObject(s, _tribe->worldX, _tribe->worldY);
    removeObject(_tribe);
    vTribe.remove(_tribe);  

      // Give the Civ a Settlement
    civ->addSettlement(s);
    
    vCiv.push(civ);
    
      // Move all characters into the first Settlement
    s->vCharacter.copy(&_tribe->vCharacter);
  }
  
  //Civ * civ = new Civ;
}

	/* DEPRECATED */
// void World::incrementDay()
// {
	// //date.advanceUnits(10000*30);
	// //std::cout<<"*CIV1\n";
// //	civ1.aiDay();
	// //std::cout<<"*CIV2\n";
	// //civ2.aiDay();
	// //civ3.aiDay();
	// //civ4.aiDay();
	
// }
	/* DEPRECATED */
// void World::incrementSecond()
// {
// //	civ1.aiSecond();
// }

void World::incrementTicksBacklog(long long unsigned int nTicks)
{
	ticksBacklog+=nTicks;
}

	// Increments the world by nTicks ticks. Higher values may lead to abstraction.
void World::incrementTicks(int nTicks)
{

	//ticksBacklog+=nTicks;
	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;
  
	calendar.advanceSecond(nTicks);
  
  
	for ( int i=0;i<vTribe.size();++i)
	{
    if ( vTribe(i)->isAlive == false )
    {
      destroyInfluence(vTribe(i));
      //vTribe(i) = 0;
    }
    else if (vTribe(i) != 0)
    {
      vTribe(i)->incrementTicks(nTicks);
    }

	}
  
  vTribe.removeNulls();
	
	// for ( int i=0;i<vTribe.size();++i)
	// {
		// vTribe(i)->incrementTicks(nTicks);
	// }
  
	for ( int i=0;i<vCiv.size();++i)
	{
		//vCiv(i)->incrementTicks(nTicks);
	}
  

	while (monthlyCounter >= 2592000)
	{
    //std::cout<<"Degrade influence.\n";
    degradeInfluence();
		monthlyCounter-=2592000;
    nTicks-=2592000;
	}
  
	while ( dailyCounter >= 86400 )
  {

		dailyCounter-=86400;
    nTicks-=86400;
	}
  
  // FOR NOW INCREMENT INDIVIDUAL TICKS THROW AWAY SOME IF THERE'S TOO MANY
  if (nTicks > 10) { nTicks = 10; }
  
  while (nTicks-- > 0 )
  {
      // CHECK ALL LOADED MAPS
      for (int i=0;i<vWorldLocal.size();++i)
      {
        vWorldLocal(i)->incrementTicks(1);
      }
      
      if ( isRaining )
      {
        if (random.oneIn(150))
        {
          isRaining = !isRaining;
        }
      }
      else
      {
        if (random.oneIn(400))
        {
          isRaining = !isRaining;
        }
      }
  }
  
  
  // if (calendar.minute%5==0)
  // { isRaining=true; }
  // else
  // { isRaining = false; }

	
	//for ( int i=0;i<vCiv.size();++i)
	//{
		//vCiv(i)->incrementTicks(nTicks);
	//}
  

	
	//updateCivContacts();


	// if (nTicks == 1)
	// {
		// /* MINIMAL ABSTRACTION. */
	// }
	// else if (nTicks == 2)
	// {
	// }
	// else
	// {
		// /* MAXIMUM ABSTRACTION. */
	// }

}

void World::logicTick()
{
	//std::cout<<"logic tick\n";
	//handleTickBacklog();
}

void World::updateMaps()
{
  
  if ( playerCharacter != 0 && playerCharacter->tribe != 0 && playerCharacter->knowledge != 0 )
  {
    if ( playerCharacter->x < 20 )
    {
      generateLocal(playerCharacter->worldX-1,playerCharacter->worldY);
    }
    if ( playerCharacter->y < 20 )
    {
      generateLocal(playerCharacter->worldX,playerCharacter->worldY-1);
    }
    if (playerCharacter->x < 20 && playerCharacter->y < 20)
    {
      generateLocal(playerCharacter->worldX-1,playerCharacter->worldY-1);
    }
    
    if ( playerCharacter->x > LOCAL_MAP_SIZE-21 )
    {
      generateLocal(playerCharacter->worldX+1,playerCharacter->worldY);
    }
    if ( playerCharacter->y > LOCAL_MAP_SIZE-21 )
    {
      generateLocal(playerCharacter->worldX,playerCharacter->worldY+1);
    }
    if (playerCharacter->x > LOCAL_MAP_SIZE-21 && playerCharacter->y > LOCAL_MAP_SIZE-21)
    {
      generateLocal(playerCharacter->worldX+1,playerCharacter->worldY+1);
    }
    
  }

}

void World::handleTickBacklog()
{
	if (ticksBacklog<=0) { return; }
	
	unsigned long long int MAXIMUM_TICKS_AT_ONCE = 2592000;
	
	relinquishTimer.start();
	
	while (ticksBacklog > MAXIMUM_TICKS_AT_ONCE)
	{
		incrementTicks(MAXIMUM_TICKS_AT_ONCE);
		ticksBacklog-=MAXIMUM_TICKS_AT_ONCE;
		relinquishTimer.update();
		
		if (relinquishTimer.uSeconds > 100 )
		{
			return;
		}
	}
	
	// We have less than maximum ticks remaining, so finish them.
	incrementTicks(ticksBacklog);
	ticksBacklog=0;
}

void World::idleTick()
{
	handleTickBacklog();
  if ( playerCharacter != 0 )
  {
    playerCharacter->updateKnowledgeIdle();
  }
}


// void World::loadHeightMap(std::string filePath)
// {
	// std::cout<<"Loading heightmap png.\n";
	
	// int fileSize;
	// unsigned char* data = FileManager::getFile(filePath,&fileSize);
	// Png png;
	// png.load(data,fileSize);
	// aHeightMap.init(png.nX,png.nY,0);
	// //aWorldObject.init(png.nX,png.nY,0);
	// //aTopoMap.init(png.nX,png.nY,3,0);

	// for(int x=0;x<png.nX;++x)
	// {
		// for(int y=0;y<png.nY;++y)
		// { /* for some reason, the map needs to loaded upside-down */
			// aHeightMap(x,png.nY-y-1)=png.getPixel3D(x,y,0);
			
			// //aTopoMap(x,png.nY-y-1,0)=png.getPixel3D(x,y,0);
			// //aTopoMap(x,png.nY-y-1,1)=png.getPixel3D(x,y,1);
			// //aTopoMap(x,png.nY-y-1,2)=png.getPixel3D(x,y,2);
		// }
	// }
	// delete [] data;
	// delete [] png.data;
// }

bool World::isLand(int _x, int _y)
{
	if ( aTerrain.isSafe(_x,_y) == true )
	{
		int tile = aTerrain(_x,_y);
		//if ( tile == GRASSLAND || tile == FOREST || tile==DESERT || tile==MOUNTAIN || tile==SNOW )
		if ( tile == OCEAN )
		{
			return false;
		}
	}
	return true;
}
inline bool World::isLand(HasXY* _xy)
{
	return isLand(_xy->x, _xy->y);
}

// bool World::loadWorldData(std::string filePath)
// {
	// std::cout<<"Getting world data.\n";
	// int fileSize;
	// unsigned char* data = FileManager::getFile(filePath,&fileSize);
	// seaLevel = data[0];
	// mountainLevel = data[1];
	// std::cout<<"Data read: " << (int)seaLevel << ","<<(int)mountainLevel<<"\n";
	// return true;
// }

int mX;


	// This should be done externally.
void World::buildArrays()
{
	//aTopoMap.initNoFill(nX,nY,3,0);
	
// Create a lightening darkening texture fractal.
	ArrayS2 <int> aLightModifier2 (nX,nY,0);
	//aLightModifier2.init(mapSize,mapSize,0);

		// GENERATE HEIGHTMAP
	DiamondSquareAlgorithmCustomRange dsa2;
	dsa2.maxValue=12;
	//HEIGHTMAP TABLE FREESTEPS SMOOTHING
	dsa2.generate(&aLightModifier2,0,2,0.75,250);

	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
			const int lightModifier = aLightModifier2(_x,_y);
			//const int lightModifier = 0;
			
			aSeed(_x,_y) = random.randInt(PORTABLE_INT_MAX-1);

        //Initialise the WorldTile with biome enum.
      aWorldTile(_x,_y).init(aTerrain(_x,_y), aSeed(_x,_y),aRiverID(_x,_y));
        //Initialise the subterranean tiles
			
			int _red;
			int _green;
			int _blue;
			
			if ( aTerrain(_x,_y) == OCEAN )
			{
				_red=50;
				_green=50;
				_blue=240;
				
				
				aIsLand(_x,_y) = false;
			}
			else if ( aTerrain(_x,_y) == RIVER )
			{
				 _red=100;
				 _green=100;
				 _blue=240;
			}
			else if ( aTerrain(_x,_y) == MOUNTAIN )
			{
				_red=80;
				_green=80;
				_blue=80;
			}
			else if ( aTerrain(_x,_y) == HILLY )
			{
				_red=80;
				_green=140;
				_blue=80;
			}
			else if ( aTerrain(_x,_y) == FOREST )
			{
				_red=0;
				_green=120;
				_blue=0;
			}
			else if ( aTerrain(_x,_y) == DESERT )
			{
				_red=200;
				_green=200;
				_blue=20;
			}
			else if ( aTerrain(_x,_y) == SNOW )
			{
				_red=220;
				_green=220;
				_blue=240;
			}
			else if ( aTerrain(_x,_y) == JUNGLE )
			{
				_red=0;
				_green=70;
				_blue=0;
			}
			else if ( aTerrain(_x,_y) == WETLAND )
			{
				_red=20;
				_green=150;
				_blue=200;
			}
			else if ( aTerrain(_x,_y) == STEPPES )
			{
				_red=180;
				_green=120;
				_blue=40;
			}
			else if ( aTerrain(_x,_y) == ICE )
			{
				_red=255;
				_green=255;
				_blue=255;
			}
			else if ( aTerrain(_x,_y) == CAVE )
			{
				_red=0;
				_green=0;
				_blue=0;
			}
			else if ( aTerrain(_x,_y) == RUIN )
			{
				_red=255;
				_green=0;
				_blue=255;
			}
			else
			{
				_red=66;
				_green=180;
				_blue=66;
			}
			
			//mof light values.
			// _red += lightModifier;
			// if ( _red > 255 ) { _red = 255; }

			// _green += lightModifier;
			// if ( _green > 255 ) { _green = 255; }
			
			// _blue += lightModifier;
			//if ( _blue > 255 ) { _blue = 255; }
			
			_red -= lightModifier;
			if ( _red < 0 ) { _red = 0; }

			_green -= lightModifier;
			if ( _green < 0 ) { _green = 0; }
			
			_blue -= lightModifier;
			if ( _blue < 0 ) { _blue = 0; }
			
			
			aTopoMap(_x,_y,0) = _red;
			aTopoMap(_x,_y,1) = _green;
			aTopoMap(_x,_y,2) = _blue;
			

			
		}
	}
	Png png;
	png.encodeS3(strSavePath+"/worldmap.png",&aTopoMap);
}

void World::generateWorld(const std::string _worldName, const int x=127, const int y=127, const int seed=0, const int fragmentation=2, const bool islandMode = true, const bool wrapX=true, const bool wrapY=false, const double landPercent = 0.66)
{
	if ( x <= 0 || y <= 0 )
	{
		std::cout<<"Error: Invalid world size.\n";
		return;
	}
	if (_worldName == "")
	{
		std::cout<<"Error: World must have a name.\n";
		return;
	}
	if (DataTools::isAlphaNumeric(_worldName)==false)
	{
		std::cout<<"Error: World name must be alphanumeric.\n";
		return;
	}
	name=_worldName;
	
	FileManager::createDirectory("savedata");
	
	if ( FileManager::directoryExists("savedata") == false )
	{
		std::cout<<"Error: Unable to create save directory.\n";
		return;
	}

	nX = x;
	nY = y;
  
    //maximum possible tile X/Y coordinate.
  maximumX = LOCAL_MAP_SIZE*nX-1;
  maximumY = LOCAL_MAP_SIZE*nY-1;

	mX=x;
  landmassSeed = seed;
	
	strSavePath = "savedata/"+name;
  
		// For now, we will just delete any worlds with the same name.
	//std::string systemCommmand = "exec rm -r "+strSavePath;
	//system(systemCommmand.c_str());
	FileManager::DeleteDirectory(strSavePath,true);
	

	
	FileManager::createDirectory(strSavePath);
	
	if ( FileManager::directoryExists(strSavePath) == false )
	{
		std::cout<<"Error: Unable to create save directory.\n";
		return;
	}
	
	Timer worldGenTimer;
	worldGenTimer.init();
	worldGenTimer.start();
	

	// Multithreading this segment runs 23% faster. Saves about 0.1 seconds or something.
  #define THREAD_WORLD_INIT
  #if defined THREAD_ALL || defined THREAD_WORLD_INIT 

      // Lambdas look much nicer with threads.
    std::thread t1( [this,x,y] { aWorldObject.init(x,y,0); aTopoMap.init(x,y,3,0); });
    std::thread t2( [this,x,y] { aTerrain.init(x,y,NOTHING); aSeed.init(x,y,0); });
    std::thread t3( [this,x,y] { aLandmassID.init(x,y,-1); aIsLand.init(x,y,true); });
    std::thread t4( [this,x,y] { aBiomeID.init(x,y,-1); aWorldTile.initClass(x,y); });
    std::thread t5( [this] { vWorldObjectGlobal.deleteAll(); vLandmass.deleteAll(); vBiome.deleteAll(); vTribe.clear(); });

  #else

		//aHeightMap.init(x,y,0);
		aWorldObject.init(x,y,0);
		aTopoMap.init(x,y,3,0);
		aTerrain.init(x,y,NOTHING);
		//aInfluence.init(x,y,0);
		aSeed.init(x,y,0);
		
		aLandmassID.init(x,y,-1);
		aBiomeID.init(x,y,-1);
		aIsLand.init(x,y,true);
    aWorldTile.initClass(x,y);
    
	// vTribe does not need to be deleted, because all Tribe objects are in vWorldObjectGlobal
  // However, the vector must be cleared.
    vWorldObjectGlobal.deleteAll();
    vLandmass.deleteAll();
    vBiome.deleteAll();
    vTribe.clear();

	#endif

  
	//
//	aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
//	aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  
	dailyCounter=0;
	monthlyCounter=0;


	WorldGenerator2 wg;
	wg.wrapX=wrapX;
	wg.wrapY=wrapY;
	wg.seed = seed;
	wg.mapSize=x;
	
	wg.oceanPercent=landPercent;
	
	wg.mountainPercent=0.025;
	wg.forestPercent=0.25;
	wg.desertPercent=0.33;
	wg.snowPercent=0.04;

	wg.goodPercent=0.05;
	wg.evilPercent=0.05;
	
	wg.freeSteps=fragmentation;
	wg.islandMode=islandMode;
	wg.variance=400;
	wg.landSmoothing=0.88;
	


	// Create master file.
  worldFilePath = strSavePath+"/main.dat";
  

  
	FileManager::createFile(worldFilePath);
  FileManager::writeTag("LANDSEED",DataTools::toString(landmassSeed),worldFilePath);
  FileManager::writeTag("WORLDNAME",name,worldFilePath);
  FileManager::writeTag("SIZEX",DataTools::toString(nX),worldFilePath);
  FileManager::writeTag("SIZEY",DataTools::toString(nY),worldFilePath);
  //FileManager::writeString(DataTools::toString(landmassSeed)+"\n"+name+"\n"+DataTools::toString(nX)+"\n"+DataTools::toString(nY)+"\n",worldFilePath);
	
  
  #if defined THREAD_ALL || defined THREAD_WORLD_INIT 
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
	#endif


	wg.generate(); 
	
		// We could remove this later by using a pointer.
	aTerrain = wg.aTerrainType;
  aRiverID = wg.aRiverMap;
	
	buildArrays(); 

	// BUILD LANDMASS ID ARRAY
  
  std::thread tLandmass ( [this]
  {
    int currentLandmassID = 0;
    int currentOceanID = 0;

    std::cout<<"Building landmass info.\n";
      
    for ( int _y=0;_y<nY;++_y)
    {
      for ( int _x=0;_x<nX;++_x)
      {
        if (aLandmassID(_x,_y) == -1 && aIsLand(_x,_y) == true )
        {
          Vector <HasXY*>* vFill = aIsLand.floodFillVector(_x,_y,false);
          
          for (int i=0;i<vFill->size();++i)
          {
            HasXY* v = (*vFill)(i);
            aLandmassID(v->x,v->y) = currentLandmassID;

          }
          
          World_Landmass* landmass = new World_Landmass;
          landmass->name = globalNameGen.generateName();
          landmass->size = vFill->size();
          landmass->getAverageCoordinates(vFill);
          vLandmass.push(landmass);

          vFill->deleteAll();
          delete vFill;
          
          ++currentLandmassID;
        }
        else if (aBiomeID(_x,_y) == -1 && aIsLand(_x,_y) == false)// fill ocean
        {
          
        }
        
      }
    }
    std::cout<<"Filled "<<currentLandmassID<<" landmasses.\n";
    
  });
  tLandmass.join();
	
	// Trying another algorithm
	
	// std::cout<<"Building landmass info with second algorithm.\n";
	
	// int nLandmass2 = 0;
	
	// ArrayS2 <int>* aLandmassID2 = aIsLand.floodFillUniqueID(false, &nLandmass2);
	
	// std::cout<<"Filled "<<nLandmass2<<" landmasses.\n";
	
	std::cout<<"Building biome info.\n";
  
  Timer timerBiomeFill;
  timerBiomeFill.init();
  timerBiomeFill.start();
	
	int currentID3=0;
  
  //Vector <int> vBiomesFilling;
  
  #define THREADED_BIOME_FILL
  #if defined THREAD_ALL || defined THREADED_BIOME_FILL 
  
  // Here I am spawning 1 thread for every possible biome. Each thread then processes biomes that have that type.
  // Performance is significantly improved and I'm getting solid 100% CPU on my quad core. It's important to spawn
  // a small number of large threads, rather than a large number of small threads. I tried the latter and I only gained
  // a few seconds on large maps, and small maps generated slower. This new approach is more than 3x faster.
    // threaded - cb - 2049 - seed 12345: 6.61 7.15 6.98        Biome time: 2.48, 2.46, 2.44
    // unthreaded - cb - 2049 - seed 12345: 12.47 13.02 13.06   Biome time: 8.25, 8.17, 8.16
    
  std::thread * aThread [N_BIOMES] = { 0 };
  
  std::mutex m;
  int currentBiomeID=0; /* Every unique biome gets a unique ID, which will make later processing much easier. */
  std::atomic<int> biomeID=0; /* Atomic is generally more efficient than using lock because of OS-level support. */
            
		// BUILD BIOME VECTOR
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			if (aBiomeID(_x,_y) == -1 && aThread[aTerrain(_x,_y)] == 0)
			{
        const int biomeSearch = aTerrain(_x,_y);
          // Spawn a thread to process this biome type
        aThread [aTerrain(_x,_y)] = new std::thread ( [ this, _x, _y, biomeSearch, &m, &biomeID ]
        {
          int thisBiomeID = biomeID++;

          for ( int _y2=_y;_y2<nY;++_y2)
          {
            for ( int _x2=_x;_x2<nX;++_x2)
            {
              if (aTerrain(_x2,_y2) == biomeSearch && aBiomeID(_x2,_y2) == -1)
              {
                Vector <HasXY*>* vFill = aTerrain.floodFillVector(_x2,_y2,true);
                for (int i=0;i<vFill->size();++i)
                {
                  HasXY* v = (*vFill)(i);
                  aBiomeID(v->x,v->y) = thisBiomeID;
                }
                World_Biome* biome = new World_Biome;
                biome->type = biomeSearch;
                biome->size = vFill->size();

                m.lock();
                  vBiome.push(biome);
                m.unlock();
                vFill->deleteAll();
                delete vFill;
              }
            }
          }
        });
      }
    }
  }

  // Let all threads finish.
  for (int i=0; i<N_BIOMES; ++i)
  {
    if ( aThread[i]!= 0)
    {
      aThread[i]->join();
      delete aThread[i];
      aThread[i]=0;
    }
  }
  
  // Name the biomes
  for (int i=0;i<vBiome.size();++i)
  {
    int biomeType = vBiome(i)->type;
    if ( biomeType == OCEAN)
    {
      if ( vBiome(i)->size < 20 )
      {
        vBiome(i)->name = "Lake " + globalNameGen.generateName();
      }
      else
      {
        vBiome(i)->name = "Ocean of "+globalNameGen.generateName();
      }
    }
    else
    {
      vBiome(i)->name = globalNameGen.generateName();
    }
    
    if ( biomeType == MOUNTAIN)
    {
      if ( vBiome(i)->size == 1 )
      {
        vBiome(i)->name = "Mount " + globalNameGen.generateName();
      }
      else
      {
        vBiome(i)->name = globalNameGen.generateName() + " Mountains";
      }
    }
  }
  
  #else
    
		// BUILD BIOME VECTOR
	for ( int _y=0;_y<nY;++_y)
	{
		//break;
		for ( int _x=0;_x<nX;++_x)
		{
			if (aBiomeID(_x,_y) == -1 )
			{
				int biomeType = aTerrain(_x,_y);
        
        
				Vector <HasXY*>* vFill = aTerrain.floodFillVector(_x,_y,true);


				
				for (int i=0;i<vFill->size();++i)
				{
					HasXY* v = (*vFill)(i);
					aBiomeID(v->x,v->y) = currentID;
				}
				
				World_Biome* biome = new World_Biome;
				biome->type = biomeType;
				biome->size = vFill->size();
				
				//  NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13
				
				if ( biomeType == OCEAN)
				{
					biome->name = "Ocean of "+globalNameGen.generateName();
				}
				else
				{
					biome->name = globalNameGen.generateName();
				}
				
				if ( biomeType == MOUNTAIN)
				{
					if ( biome->size == 1 )
					{
						biome->name = "Mount " + globalNameGen.generateName();
					}
					else
					{
						biome->name = globalNameGen.generateName() + " Mountains";
					}
				}
				
				vBiome.push(biome);

        vFill->deleteAll();
				delete vFill;
				
				++currentID;
			}
			
		}
	}
  
  
  #endif

  timerBiomeFill.update();
	std::cout<<"Filled "<<currentID<<" biomes in "<<timerBiomeFill.fullSeconds<<" seconds.\n";

	
	generated = true;
	
  isRaining=false;
	
	// Build and shuffle the tile vector
	//Vector < Vector <HasXY> > vAllTiles;
  
  std::thread tvAllTiles( [this]
  {
    for (int i=0;i<vAllTiles.size();++i)
    {
      delete vAllTiles(i);
    }
    vAllTiles.clear();
    for (int i=0;i<nY;++i)
    {
      Vector <int>* vCoord = new Vector <int>;
      
      for (int _x=0;_x<nX;++_x)
      {
        vCoord->push(_x);
      }
      vCoord->shuffle();
      vAllTiles.push(vCoord);
    }
  });
  
  std::thread tvAllTiles2( [this]
  {
    for (int i=0;i<vAllTiles2.size();++i)
    {
      delete vAllTiles2(i);
    }
    vAllTiles2.clear();
    for (int _y=0;_y<nY;++_y)
    {
      for (int _x=0;_x<nX;++_x)
      {
        vAllTiles2.push( new HasXY (_x,_y) );
      }
    }
    vAllTiles2.shuffle();
  });
	
  tvAllTiles.join();
  tvAllTiles2.join();
	

  
  


  


	
	
	//Vector < Vector <HasXY* >* > vAllTiles;
  
  
  // SAVE WORLD DATA HERE (LATER PUT INTO FUNCTION)
    // std::string tileData = "";
    // for (int _y=0;_y<nY;++_y)
    // {
      // for (int _x=0;_x<nX;++_x)
      // {
        // tileData+=",";
        // tileData+=DataTools::toString(aWorldTile(_x,_y).biome);
      // }
    // }
    // FileManager::writeTag("BIOME",tileData,worldFilePath);
	
	
	worldGenTimer.update();
	std::cout<<"world generated in: "<<worldGenTimer.fullSeconds<<" seconds.\n";
  
  std::cout<<"The world's uid is: "<<getUID()<<".\n";

  
  //std::cout<<"Doing test save.\n";
  
  // worldFilePath = strSavePath+"/main2.dat";
  // saveFileManager.vSaveObjects.clear();
  // saveFileManager.vSaveObjects.push(this);
  // saveFileManager.saveFile(worldFilePath);
    //FileManager::writeString(NYA,worldFilePath);
}


//This makes the local map visible.
void World::generateLocal(const int _localX, const int _localY)
{
  if ( isSafe(_localX,_localY) == false )
  { return; }

  // Only generate this local map if it isn't already in memory.
  for ( int i=0;i<vWorldLocal.size();++i)
  {
    if ( vWorldLocal(i)->globalX == _localX && vWorldLocal(i)->globalY == _localY )
    {
      //std::cout<<"ALREADY GENERATED\n";
      return;
    }
  }

  auto worldLocal = new World_Local;
  worldLocal->init(_localX,_localY);
  worldLocal->generate();
  
  vWorldLocal.push(worldLocal);
  
  //There needs to be a minimum of 3 maps active at any time. (1 map the player is currently in,
    // and potentially three neighboring maps. Additional maps will likely need to be loaded in
    // the background, therefore I'll set it to 5 for now.
    // I'll need to make an algorithm to decide which maps to purge.
  
  if ( vWorldLocal.size() > MAX_LOCAL_MAPS_IN_MEMORY )
  {
      // UNLOAD LOCAL MAP HERE
    //delete vWorldLocal(0);
    //vWorldLocal.eraseSlot(0);
  }
  
  return;

}

void World::unloadLocal(const int _localX, const int _localY)
{
  if ( isSafe(_localX,_localY) == false )
  { return; }

  // Only unload the local map if it is loaded.
  for ( int i=0;i<vWorldLocal.size();++i)
  {
    if ( vWorldLocal(i)->globalX == _localX && vWorldLocal(i)->globalY == _localY )
    {
      //std::cout<<"ALREADY GENERATED\n";
      return;
    }
  }

  // auto worldLocal = new World_Local;
  // worldLocal->init(_localX,_localY);
  // worldLocal->generate();
  
  // vWorldLocal.push(worldLocal);
  
  return;

}


Vector <Tribe*>* World::getTribesOn(const int _x, const int _y)
{
  Vector <Tribe*>* vTribeOn = new Vector <Tribe*>;
  if ( isSafe(_x,_y) )
  {
    for ( int i=0;i<vTribe.size();++i)
    {
      if ( vTribe(i)->worldX == _x && vTribe(i)->worldY == _y )
      {
        vTribeOn->push(vTribe(i));
      }
    }
  }
  return vTribeOn;
}

int World::getTileFertility(const int _x, const int _y)
{
	if ( aTerrain.isSafe(_x,_y) )
	{
    return aWorldTile(_x,_y).baseFertility;
	}

	return 0;
}
	inline int World::getTileFertility(const HasXY* xy)
	{ return getTileFertility(xy->x, xy->y); }

int World::getSurroundingFertility(const int _x, const int _y)
{
	int totalFertility = 0;

	Vector < HasXY* >* vTerrain = aTerrain.getNeighbors(_x,_y,true);
	
	//aTerrain.getNeighborVector(_x,_y,&vTerrain,true /* INCLUDE SELF */);
	
	for ( int i=0;i<vTerrain->size();++i)
	{
		totalFertility+= getTileFertility((*vTerrain)(i));
		// int terrainType = vTerrain(i);
		
		// if ( terrainType == OCEAN )
		// {
			// totalFertility += 1;
		// }
		// else if (terrainType == GRASSLAND )
		// {
			// totalFertility += 1;
		// }
		// else if (terrainType == FOREST )
		// {
			// totalFertility += 1;
		// }
	}
	vTerrain->deleteAll();
	delete vTerrain;
	return totalFertility;
	
}

int World::getHuntingYield(const int _x, const int _y)
{
	int totalYield = 0;

	Vector <enumBiome> vTerrain;
	aTerrain.getNeighborVector(_x,_y,&vTerrain,true /* INCLUDE SELF */);
	
	for ( int i=0;i<vTerrain.size();++i)
	{
		int terrainType = vTerrain(i);
		
		if ( terrainType == OCEAN )
		{
			totalYield += 2;
		}
		else if (terrainType == GRASSLAND )
		{
			totalYield += 1;
		}
		else if (terrainType == FOREST )
		{
			totalYield += 2;
		}
		else if (terrainType == JUNGLE)
		{
			totalYield += 2;
		}
	}
	
	return totalYield;
}

//NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};

std::string World::getTileType(const int _x, const int _y)
{
	if (aTerrain.isSafe(_x,_y))
	{
		int terrainType = aTerrain(_x,_y);
		
		return biomeName[terrainType];
	}
	return "?";
}

void World::getRandomTile (int* x, int* y)
{
	*x = Random::randomInt(nX);
	*y = Random::randomInt(nY);
}

int World::getPopulation()
{
	int population = 0;
	
	for (int i=0;i<vTribe.size();++i)
	{
		population+=vTribe(i)->getPopulation();
	}
	
	
	return population;
}

bool World::getRandomLandTile(int* x, int* y)
{
	//std::cout<<"Get random land tile\n";
	//*x=0;
	//*y=0;
	//return true;
	
	int randomRow = random.randomInt(nY-1);
	int nRowsSearched = 0;
	
	while ( nRowsSearched < nY )
	{
		for (int _x=0;_x<nX;++_x)
		{
			int randomX = (*vAllTiles(randomRow))(_x);
			
			if ( aTerrain(randomX,randomRow) != OCEAN )
			{
				*x=randomX;
				*y=randomRow;
				return true;
			}

		}
		
		vAllTiles(randomRow)->shuffle();
		
		++randomRow;
		if (randomRow >= nY) { randomRow=0; }
		++nRowsSearched;
	}
	
	
	// for ( int nAttempts=0;nAttempts<100;++nAttempts)
	// {
		// getRandomTile(x,y);
	
		// if ( aTerrain(*x,*y) != OCEAN && aTerrain(*x,*y) != NOTHING)
		// {
			// return true;
		// }
	
	// }

	*x=0;
	*y=0;
	return false;
}

// UPDATE: This now actually returns a random distribution.
HasXY* World::getRandomTileOfType(enumBiome _type)
{
  vAllTiles2.shuffle();
  for (auto xy : vAllTiles2)
  {
    if ( aTerrain(xy->x,xy->y) == _type )
    {
      return xy;
    }
  }
  
	// int randomRow = random.randomInt(nY-1);
	// int nRowsSearched = 0;
	
	// while ( nRowsSearched < nY )
	// {
		// for (int _x=0;_x<nX;++_x)
		// {
			// int randomX = (*vAllTiles(randomRow))(_x);
			
			// if ( aTerrain(randomX,randomRow) == _type )
			// {
				// return new HasXY (randomX,randomRow);
			// }

		// }
		
		// vAllTiles(randomRow)->shuffle();
		
		// ++randomRow;
		// if (randomRow >= nY) { randomRow=0; }
		// ++nRowsSearched;
	// }
	
	
	return 0;
}


void World::queryTile( int hoveredXTile, int hoveredYTile)
{
	if ( hoveredXTile < 0 || hoveredYTile < 0 || hoveredXTile >= nX || hoveredYTile >= nY )
	{ return; }
  
  queryWorldX = hoveredXTile;
  queryWorldY = hoveredYTile;


  //bool hasObject = false;
	for (int i=0;i<vWorldObjectGlobal.size();++i)
	{
		if ( vWorldObjectGlobal(i)->worldX == hoveredXTile && vWorldObjectGlobal(i)->worldY == hoveredYTile)
		{
			//std::cout<<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name<<".\n";
      //Console (Stream() <<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name);
      //hasObject = true;
		}
	}
  //if ( hasObject )
  //{ Console (Stream() <<"Objects:"); }

}

void World::addInfluence(Tribe* tribe, int amount)
{
	if ( tribe==0 || isSafe (tribe->worldX,tribe->worldY) == false )
	{ return; }
	
  aWorldTile(tribe->worldX,tribe->worldY).addInfluence(tribe,amount);
}

// THIS IS A BIT DODGY. THERE SHOULD BE A SINGLE PASS FOR EVERYONE AT ONCE.
void World::degradeInfluence(int value /* =1 */)
{
  for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
      aWorldTile(_x,_y).degradeInfluence(1);
    }
	}
}

void World::destroyInfluence (Tribe* _tribe)
{
	if ( _tribe==0 )
	{ return; }

  
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
      aWorldTile(_x,_y).destroyInfluence(_tribe);
    }
	}
	
	// for (std::map <Tribe*,int>* mInfluence : aInfluence )
	// {
		// if ( mInfluence != 0)
		// {
			// // Search for existing influence from this tribe.
			// auto search = mInfluence->find(_tribe);
			// if(search != mInfluence->end())
			// {
				// // Decrement influence if this tribe has been here.
				// if(search->second > 0)
				// { search->second=0;
				// }
        
        // //When influence is 0, it should be deleted.


			// }
		// }
	// }
}

	// Note that 0 influence doesn't count as influence. In this case it will return 0.
Tribe* World::getDominantInfluence (const int _x, const int _y)
{
  if ( isSafe(_x,_y) == false ) { return 0; }
  
  return aWorldTile(_x,_y).getDominantInfluence();
  
	// if ( aInfluence.isSafe(_x,_y) && aInfluence(_x,_y) != 0)
	// {
		// std::map<Tribe* , int>::iterator it;
		
		// int largestInfluence = 0;
		// Tribe * dominantTribe = 0;
		
		// for ( it = aInfluence(_x,_y)->begin(); it != aInfluence(_x,_y)->end(); it++ )
		// {
			// if ( it->second > largestInfluence )
			// {
				// dominantTribe = it->first;
				// largestInfluence = it->second;
			// }
		// }
		// return dominantTribe;
	// }
	// return 0;
}

Tribe* World::getDominantInfluence (HasXY* _xy)
{
	return getDominantInfluence(_xy->x,_xy->y);
}
	
	
int World::getHighestInfluence(const int _x, const int _y)
{
  if ( isSafe(_x,_y) == false ) { return 0; }
  
  return aWorldTile(_x,_y).getDominantInfluenceValue();
  
  
	// if ( aInfluence.isSafe(_x,_y) && aInfluence(_x,_y) != 0)
	// {
		
		// std::map<Tribe* , int>::iterator it;
		
		// int largestInfluence = 0;
		// Tribe * dominantTribe = 0;
		
		// for ( it = aInfluence(_x,_y)->begin(); it != aInfluence(_x,_y)->end(); it++ )
		// {
			// if ( dominantTribe == 0 || it->second > largestInfluence )
			// {
				// dominantTribe = it->first;
				// largestInfluence = it->second;
			// }
		// }
		// return largestInfluence;
	// }
	// return 0;
}

	int World::getHighestInfluence(HasXY* _xy)
	{ return getHighestInfluence(_xy->x,_xy->y); }

	
std::string World::getLandmassName(const int _x, const int _y)
{
  if ( aLandmassID.isSafe(_x,_y) )
  {
    const int id = aLandmassID(_x,_y);
    if (id != -1)
    {
      return vLandmass(id)->name;
    }
  }
  
  return "";
}
  std::string World::getLandmassName (HasXY* _xy)
  { return getLandmassName(_xy->x,_xy->y); }
  
std::string World::getBiomeName(const int _x, const int _y)
{
  if ( isSafe(_x,_y) )
  {
    return vBiome(aBiomeID(_x,_y))->name;
  }
  
  return "";
}
    

  bool World::hasFreeTerritory(int landmassID)
  {
    Vector <HasXY*>* vLandmassTiles = 0;
    
    for ( int _y=0;_y<nY;++_y)
    {
      for ( int _x=0;_x<nX;++_x)
      {
        if (aLandmassID(_x,_y) == landmassID && aIsLand(_x,_y) == true )
        {
          // GET ALL COORDINATES OF THIS 
          vLandmassTiles = aIsLand.floodFillVector(_x,_y,false);
          
          for (int i=0;i<vLandmassTiles->size();++i)
          {
            if ( getHighestInfluence( (*vLandmassTiles)(i) ) == 0)
            {
              delete vLandmassTiles;
              return true;
            }
          }
          delete vLandmassTiles;
          return false;

        }
        
      }
    }
    delete vLandmassTiles;
    return false;
  }
  
  int World::nFreeTerritory(int landmassID)
  {
    Vector <HasXY*>* vLandmassTiles = 0;
    
    int nUnclaimed = 0;
    
    for ( int _y=0;_y<nY;++_y)
    {
      for ( int _x=0;_x<nX;++_x)
      {
        if (aLandmassID(_x,_y) == landmassID && aIsLand(_x,_y) == true )
        {
          // GET ALL COORDINATES OF THIS LANDMASS
          vLandmassTiles = aIsLand.floodFillVector(_x,_y,false);
          
          for (int i=0;i<vLandmassTiles->size();++i)
          {
            if ( getHighestInfluence( (*vLandmassTiles)(i) ) == 0)
            {
              ++nUnclaimed;
              //delete vLandmassTiles;
              //return true;
            }
          }
          delete vLandmassTiles;
          return nUnclaimed;

        }
      }
    }
    delete vLandmassTiles;
    return 0;
  }
  
  // Generate local map if necessary.
  // Update worldviewer.
void World::controlCharacter(Character* _character)
{
  if (_character==0) { return; }
  
  playerCharacter = _character;
  
  /* Get character coordinates */
  int genX = playerCharacter->worldX;
  int genY = playerCharacter->worldY;
  
  //worldViewer.localX = genX;
  //worldViewer.localY = genY;
  
  //worldViewer.centerTileX = genX;
  //worldViewer.centerTileY = genY;
  
  /* Generate local map */
  generateLocal(genX,genY);
  
  worldViewer.tileSize = 2000;
  worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
  
  playerCharacter->initialiseKnowledge();
  //playerCharacter->updateKnowledge();
  
  // Jiggle character around to make sure they're not inside an object.
  
  World_Local* wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
  if ( wl != 0 )
  {
    wl->moveObject(playerCharacter,playerCharacter->x+1,playerCharacter->y);
    playerCharacter->updateKnowledge();
  }
  wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
  if ( wl != 0 )
  {
    wl->moveObject(playerCharacter,playerCharacter->x-1,playerCharacter->y);
    playerCharacter->updateKnowledge();
  }
  wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
  if ( wl != 0 )
  {
    wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y+1);
    playerCharacter->updateKnowledge();
  }
  wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
  if ( wl != 0 )
  {
    wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y-1);
    playerCharacter->updateKnowledge();
  }
  
  worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
  updateMaps();
  playerCharacter->updateKnowledge();
  
  
}
  
  
Tribe * World::getNearestConnectedTribe (Tribe * _tribe, bool sameRace /* =true */ )
{
  // STEP 1: FIND TRIBE ON SAME LANDMASS
  int distance = -1;
  Tribe* closestTribe = 0;
  int landmass = aLandmassID(_tribe->worldX, _tribe->worldY);
  
  for (int i=0;i<vTribe.size();++i)
  {
    if ( vTribe(i) != _tribe && vTribe(i)->isAlive && (sameRace==false || _tribe->race == vTribe(i)->race ) )
    {
      if ( aLandmassID(vTribe(i)->worldX,vTribe(i)->worldY) == landmass )
      {
        int distance2 = _tribe->distanceTo(vTribe(i));
        if ( closestTribe == 0 || distance2 < distance )
        {
          closestTribe = vTribe(i);
          distance = distance2;
        }
      }
    }
  }
  
  
  return closestTribe;
}
  
Tribe* World::combatCheck (Tribe* _tribe)
{
  for ( int i=0;i<vTribe.size();++i)
  {
    if ( vTribe(i) != _tribe && vTribe(i)->race == _tribe->race && vTribe(i)->isAlive && _tribe->isAlive) /* Temp limit combat between same race */
    {
      const int distX = abs(_tribe->worldX - vTribe(i)->worldX);
      const int distY = abs(_tribe->worldY - vTribe(i)->worldY);

      if ( distX < 2 && distY < 2 )
      {
        return vTribe(i);
      }
    }
  }
  return 0;
}
	
WorldTile * World::getTile (const int x, const int y )
{
  if (isSafe(x,y) )
  { return &aWorldTile(x,y); }
  return 0;
}


// This should be merged with control character.
bool World::prepareAdventureMode( Character * _character )
{
  //std::cout<<"World is preparing for Adventure Mode.\n";
  
  if ( _character == 0 )
  {
    std::cout<<"Error: Bad pointer.\n";
    return false;
  }
  
  playerCharacter = _character;
  
  if ( playerCharacter->tribe == 0 )
  {
    std::cout<<"Error: Character doesn't have Tribe.\n";
    return false;
  }
  
  //std::cout<<"Generating local map: ("<<playerCharacter->tribe->worldX<<", "<<playerCharacter->tribe->worldY<<").\n";
  //generateLocal(playerCharacter->tribe->worldX,playerCharacter->tribe->worldY);
  
  controlCharacter(_character);
  
  return true;
}


bool World::loadWorld(std::string _name)
{
  SaveFileManager sfm;
  
	strSavePath = "savedata/"+_name;
	
  std::cout<<"Attempting to load data from: "<<strSavePath<<".\n";
	
	if ( FileManager::directoryExists(strSavePath) == false )
	{
		std::cout<<"Error: This world doesn't appear to exist.\n";
		return false;
	}
  
  std::cout<<"Loading data from: "<<strSavePath<<".\n";
  
	// Load master file.
  worldFilePath = strSavePath+"/main.dat";
  
  if ( FileManager::fileExists(worldFilePath) )
  {
    std::cout<<"Loading master.dat\n";
    
    std::cout<<FileManager::getFileAsString(worldFilePath)<<".\n";
    
    sfm.loadFile(worldFilePath);
  }
  else
  {
    std::cout<<"Master file doesn't appear to exist.\n";
    return false;
  }

  
  std::string wname = sfm.loadVariableString("WORLDNAME");
  std::string wseed = sfm.loadVariableString("LANDSEED");
  std::string wX = sfm.loadVariableString("SIZEX");
  std::string wY = sfm.loadVariableString("SIZEY");
  
  std::cout<<"Loaded worldname is: "<<wname<<".\n";
  std::cout<<"Loaded seed is: "<<wseed<<".\n";
  std::cout<<"Loaded x is: "<<wX<<".\n";
  std::cout<<"Loaded y is: "<<wY<<".\n";

	int fileSize;
	unsigned char* data = FileManager::getFile(strSavePath+"/biome.png" ,&fileSize);
  
  if (data == 0)
  {
    std::cout<<"ERROR: biome.png couldn't be loaded.\n";
    return false;
  }
  
	Png png;
  
  if (png.load(data,fileSize) == false )
  {
    std::cout<<"ERROR: PNG didn't load successfully.\n";
    return false;
  }
  
  if ( png.nX < 1 || png.nY < 1 )
  {
    std::cout<<"ERROR: PNG has bad dimensions.\n";
    return false;
  }
  
  nX = png.nX;
  nY = png.nY;
  
  
	aWorldObject.init(png.nX,png.nY,0);
	aTopoMap.init(png.nX,png.nY,3,0);
  aTerrain.init(png.nX,png.nY,NOTHING);
  aSeed.init(png.nX,png.nY,1);
  aLandmassID.init(png.nX,png.nY,-1);
  aBiomeID.init(png.nX,png.nY,-1);
  aIsLand.init(png.nX,png.nY,true);

	for(int x=0;x<png.nX;++x)
	{
		for(int y=0;y<png.nY;++y)
		{
      // Note that PNGs use a different coordinate system, therefore must be loaded upside-down.
      // Maybe change this in future.
			
			aTopoMap(x,png.nY-y-1,0)=png.getPixel3D(x,y,0);
			aTopoMap(x,png.nY-y-1,1)=png.getPixel3D(x,y,1);
			aTopoMap(x,png.nY-y-1,2)=png.getPixel3D(x,y,2);
      
      
			// aTopoMap(x,y,0)=png.getPixel3D(x,y,0);
			// aTopoMap(x,y,1)=png.getPixel3D(x,y,1);
			// aTopoMap(x,y,2)=png.getPixel3D(x,y,2);
      
      
      if (aTopoMap(x,png.nY-y-1,0) == biomeRed[OCEAN] && aTopoMap(x,png.nY-y-1,1) == biomeGreen[OCEAN] && aTopoMap(x,png.nY-y-1,2) == biomeBlue[OCEAN] )
      //if (aTopoMap(x,y,0) == biomeRed[OCEAN] && aTopoMap(x,y,1) == biomeGreen[OCEAN] && aTopoMap(x,y,2) == biomeBlue[OCEAN] )
      {
        aTerrain(x,png.nY-y-1)=OCEAN;
      }
      else
      {
        aTerrain(x,png.nY-y-1)=GRASSLAND;
      }
      

		}
	}
  
  generated = true;
	return true;
}

// SaveFileInterface


void World::save()
{
  SaveFileManager sfm;
  
  // WORLD INFO
  
  sfm.addVariable("WORLDNAME",name);
  sfm.addVariable("LANDSEED",landmassSeed);
  sfm.addVariable("SIZEX",nX);
  sfm.addVariable("SIZEY",nY);
  
  sfm.saveToFile(worldFilePath);
  
  // SAVE BIOME INFO AS PNG.
    // BIOME INFO ALSO FUNCTIONS AS LANDMASS INFO.
    
  ArrayS3 <unsigned char> aBiomeData (nX,nY,3,0);
  
	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
      enumBiome _biome = aTerrain(_x,_y);
      
      aBiomeData(_x,_y,0) = biomeRed[_biome];
      aBiomeData(_x,_y,1) = biomeGreen[_biome];
      aBiomeData(_x,_y,2) = biomeBlue[_biome];
    }
  }
    
	Png png;
	png.encodeS3(strSavePath+"/biome.png",&aBiomeData);
  
  
  // SAVE RESOURCE INFO AS PNG.
}

#endif
