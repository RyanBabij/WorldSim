#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_CPP
#define WORLDSIM_WORLD_LOCAL_CPP

/* World_Local.hpp
	#include "World_Local.cpp"

	Project: WorldSim
	Created: 10/12/2017, 0272632857.
	Updated: 10/12/2017, 0272632857.

	Description:
	The World stores local-level data, such as the position of local units.

	Notes:

	0272632857 - Added basic documentation.

*/



#include "World_Local.hpp"

World_Local::World_Local()
{
	globalX=0;
	globalY=0;
	
	nX=LOCAL_MAP_SIZE;
	nY=LOCAL_MAP_SIZE;
	active=false;
	generated=false;
}

void World_Local::init(int _x, int _y)
{
  globalX = _x;
  globalY = _y;
  
  aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
}

bool World_Local::generate()
{
  
  if ( world.isSafe(globalX,globalY) == false )
  { return false; }
  
  baseBiome = world.aWorldTile(globalX,globalY).biome;
  
  seed = world.aSeed(globalX,globalY);
  
  
		// GENERATE HEIGHTMAP
  DiamondSquareAlgorithmCustomRange dsa2;
	dsa2.maxValue=5;
  
  ArrayS2 <int> aLocalHeight;
  aLocalHeight.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
  
  // If we are generating a mountain, set a summit in the middle which is 3x3 tiles.
  if ( baseBiome == MOUNTAIN )
  {
    aLocalHeight(LOCAL_MAP_SIZE/2,LOCAL_MAP_SIZE/2) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,LOCAL_MAP_SIZE/2) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,LOCAL_MAP_SIZE/2) = 100;
    
    aLocalHeight(LOCAL_MAP_SIZE/2,(LOCAL_MAP_SIZE/2)-1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,(LOCAL_MAP_SIZE/2)-1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,(LOCAL_MAP_SIZE/2)-1) = 100;
    
    aLocalHeight(LOCAL_MAP_SIZE/2,(LOCAL_MAP_SIZE/2)+1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)+1,(LOCAL_MAP_SIZE/2)+1) = 100;
    aLocalHeight((LOCAL_MAP_SIZE/2)-1,(LOCAL_MAP_SIZE/2)+1) = 100;
    
    aLocalHeight.fillBorder(1);
    dsa2.maxValue=100;
    dsa2.generate(&aLocalHeight,0,0,0.9,10);
    
  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      aLocalHeight(_x,_y) =  aLocalHeight(_x,_y)/20;
    }
  }
    //aLocalHeight.setBorder(1);
  }
  else
  {
	////HEIGHTMAP TABLE FREESTEPS SMOOTHING
    dsa2.generate(&aLocalHeight,0,0,0.75,100);
  }
	
  
  // Take the seed for this world tile and expand it into a subseed for every local tile */
  random.seed(seed);
  
  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      aLocalTile(_x,_y).baseTerrain = baseBiome;
      
      int baseTreeChance = 30;
      
      // Temporary hack to make forests look less bad.
      if ( baseBiome == FOREST || baseBiome == JUNGLE )
      {
        aLocalTile(_x,_y).baseTerrain = GRASSLAND;
        baseTreeChance/=8;
      }
      
      

      aLocalTile(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aLocalTile(_x,_y).clearObjects();
      aLocalTile(_x,_y).height = aLocalHeight(_x,_y);
      
      
      aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
      aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aSubterranean(_x,_y).clearObjects();
      aSubterranean(_x,_y).height = -1;
      
      if (random.oneIn(baseTreeChance))
      {
        //put tree
        auto tree = new WorldObject_Tree;
        tree->growth = 1;
        aLocalTile(_x,_y).addObject(tree);
      }
      else if (random.oneIn(1000))
      {
        auto tree = new WorldObject_Tree;
        tree->growth = 0;
        aLocalTile(_x,_y).addObject(tree);
      }
      
      else if ( baseBiome == MOUNTAIN )
      {
        if (random.oneIn(10))
        {
          auto * rockyBoi = new WorldObject_Rock;
          if ( random.oneIn(10) )
          {
            rockyBoi->nGold = 100;
          }
          
          aLocalTile(_x,_y).addObject(rockyBoi);
        }
        
      }
    }
  }
  
  
  for ( int x = 10; x<20; ++x)
  {
    for ( int y=10;y<20; ++y)
    {
      aLocalTile(x,y).hasFloor = true;
    }
  }
  
  auto sign = new WorldObject_Sign;
  aLocalTile(21,21).addObject(sign);

  //Generate global objects
  Vector <Tribe * > * vTribesHere = world.getTribesOn(globalX,globalY);
  
  for (int i=0;i<vTribesHere->size();++i)
  {
    Tribe * currentTribe = (*vTribesHere)(i);
    
    for ( int i2=0; i2<currentTribe->vCharacter.size(); ++i2)
    {
      //RANDOMLY PLACE THE TRIBE CHARACTERS HERE
      //Character * c = new Character;
      
      int randX = Random::randInt(nX-1);
      int randY = Random::randInt(nY-1);

      currentTribe->vCharacter(i2)->x=randX;
      currentTribe->vCharacter(i2)->y=randY;
      aLocalTile(randX,randY).addObject(currentTribe->vCharacter(i2));
      //aLocalTile(Random::randInt(nX-1),Random::randInt(nY-1)).addObject(c);
    
    }
    

  }
  delete vTribesHere;
  return false;
  
	std::cout<<"Checking map data in path: "<<world.strSavePath<<"\n";
  
	// strSavePath = "savedata/"+name;
	
		// // For now, we will just delete any worlds with the same name.
	// //std::string systemCommmand = "exec rm -r "+strSavePath;
	// //system(systemCommmand.c_str());
	// FileManager::DeleteDirectory(strSavePath,true);
	


  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	
  std::cout<<"Savefile for this map is: "<<localMapPath<<"\n";
	// FileManager::createDirectory(strSavePath);
	
	if ( FileManager::directoryExists(world.strSavePath) )
  {
    if ( FileManager::fileExists(localMapPath) )
    {
    }
    else
    {
      //WRITE A FILE
      FileManager::createFile(localMapPath);
    }
	}
  else
  {
    std::cout<<"Error: Unable to access directory.\n";
  }
  
	//aIsLand.init(3000,3000,false);
	return false;
}

bool World_Local::generateTestMap()
{
  
  if ( world.isSafe(globalX,globalY) == false )
  { return false; }
  
  baseBiome = world.aWorldTile(globalX,globalY).biome;
  
  seed = world.aSeed(globalX,globalY);
  
  
		// GENERATE HEIGHTMAP
  DiamondSquareAlgorithmCustomRange dsa2;
	dsa2.maxValue=5;
  
  ArrayS2 <int> aLocalHeight;
  aLocalHeight.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);

  dsa2.generate(&aLocalHeight,0,0,0.75,100);
  
  // Take the seed for this world tile and expand it into a subseed for every local tile */
  random.seed(666);
  
  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      vAllTiles.push( new HasXY (_x,_y) );

      aLocalTile(_x,_y).baseTerrain = baseBiome;
      aLocalTile(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aLocalTile(_x,_y).clearObjects();
      aLocalTile(_x,_y).height = aLocalHeight(_x,_y);
      
      
      aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
      aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aSubterranean(_x,_y).clearObjects();
      aSubterranean(_x,_y).height = -1;
      
      if (random.oneIn(20))
      {
        //put tree
        auto tree = new WorldObject_Tree;
        tree->growth = 1;
        aLocalTile(_x,_y).addObject(tree);
      }
      else if (random.oneIn(1000))
      {
        auto tree = new WorldObject_Tree;
        tree->growth = 0;
        aLocalTile(_x,_y).addObject(tree);
      }
      

    }
  }
  vAllTiles.shuffle();
  
  for ( int i=0; i<100; ++i)
  {
    //spawn some people in.
  }
  
  //Generate global objects
  Vector <Tribe * > * vTribesHere = world.getTribesOn(globalX,globalY);
  
  for (int i=0;i<vTribesHere->size();++i)
  {
    Tribe * currentTribe = (*vTribesHere)(i);
    //RANDOMLY PLACE THE TRIBE CHARACTERS HERE
    Character * c = new Character;

    aLocalTile(0,0).addObject(c);
    
  }
  delete vTribesHere;
  return false;
  
	std::cout<<"Checking map data in path: "<<world.strSavePath<<"\n";
  
	// strSavePath = "savedata/"+name;
	
		// // For now, we will just delete any worlds with the same name.
	// //std::string systemCommmand = "exec rm -r "+strSavePath;
	// //system(systemCommmand.c_str());
	// FileManager::DeleteDirectory(strSavePath,true);
	


  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	
  std::cout<<"Savefile for this map is: "<<localMapPath<<"\n";
	// FileManager::createDirectory(strSavePath);
	
	if ( FileManager::directoryExists(world.strSavePath) )
  {
    if ( FileManager::fileExists(localMapPath) )
    {
    }
    else
    {
      //WRITE A FILE
      FileManager::createFile(localMapPath);
    }
	}
  else
  {
    std::cout<<"Error: Unable to access directory.\n";
  }
  
	//aIsLand.init(3000,3000,false);
	return false;
}

bool World_Local::saveToFile(std::string _path)
{
	return false;
}


bool World_Local::putObject (WorldObject* _object, int _x, int _y)
{
  if ( aLocalTile.isSafe(_x,_y) == false )
  { return false; }

  _object->worldX = globalX;
  _object->worldY = globalY;
  aLocalTile(_x,_y).addObject(_object);
  
  _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
  
  return true;
}

bool World_Local::moveObject (WorldObject* _object, int newX, int newY )
{
  
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
  if ( aLocalTile.isSafe(newX,newY) == false )
  {
    // Moving between maps.
    
      //LEFT
    if ( newX < 0 )
    {
      int nMaps = 0;
      while ( newX < 0 )
      {
        newX += LOCAL_MAP_SIZE-1;
        ++nMaps;
      }
      ++newX;
      
      //PUT OBJECT
      if (nMaps > 0 )
      {
        if (world.isSafe(globalX-nMaps,globalY))
        {
          aLocalTile(_object->x,_object->y).removeObject(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX-nMaps;
          _object->worldY = globalY;
          
          world(globalX-nMaps,globalY)->putObject(_object,newX,newY);
          
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
          //std::cout<<"New gps coords: "<<_object->fullX<<", "<<_object->fullY<<".\n";
          
          //std::cout<<"World conversion:";
          world(_object->fullX,_object->fullY);
          
          
          
          
          //std::cout<<"Conversion test:\n";
          
          
          //void absoluteToRelative (const int _absoluteX, const int _absoluteY, int * _globalX, int * _globalY, int * _localX, int * _localY)
          
          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
          //std::cout<<"Abs to rel: "<<gX<<", "<<gY<<", "<<lX<<", "<<lY<<".\n";
          
          return false;
          
        }
      }
    }
      //RIGHT
    else if (newX > LOCAL_MAP_SIZE-1)
    {
      int nMaps = 0;
      while ( newX > LOCAL_MAP_SIZE-1 )
      {
        newX -= LOCAL_MAP_SIZE-1;
        ++nMaps;
      }
      --newX;
      //std::cout<<"NEWX: "<<newX<<".\n";
      
      //PUT OBJECT
      if (nMaps > 0 )
      {
        if (world.isSafe(globalX+nMaps,globalY))
        {
          aLocalTile(_object->x,_object->y).removeObject(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX+nMaps;
          _object->worldY = globalY;
          world(globalX+nMaps,globalY)->putObject(_object,newX,newY);
          
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
          //std::cout<<"New gps coords: "<<_object->fullX<<", "<<_object->fullY<<".\n";
          
          //std::cout<<"World conversion:";
          world(_object->fullX,_object->fullY);
          
          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
          //std::cout<<"Abs to rel: "<<gX<<", "<<gY<<", "<<lX<<", "<<lY<<".\n";
          
          return false;
          
        }
      }
    }
      //DOWN
    if ( newY < 0 )
    {
      int nMaps = 0;
      while ( newY < 0 )
      {
        newY += LOCAL_MAP_SIZE-1;
        ++nMaps;
      }
      ++newY;
      
      //PUT OBJECT
      if (nMaps > 0 )
      {
        if (world.isSafe(globalX,globalY-nMaps))
        {
          aLocalTile(_object->x,_object->y).removeObject(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY-nMaps;
          world(globalX,globalY-nMaps)->putObject(_object,newX,newY);
          
          
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
          //std::cout<<"New gps coords: "<<_object->fullX<<", "<<_object->fullY<<".\n";
          
          //std::cout<<"World conversion:";
          world(_object->fullX,_object->fullY);
          
          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
          //std::cout<<"Abs to rel: "<<gX<<", "<<gY<<", "<<lX<<", "<<lY<<".\n";
          
          return false;
          
        }
      }
    }
      //UP
    else if (newY > LOCAL_MAP_SIZE-1)
    {
      int nMaps = 0;
      while ( newY > LOCAL_MAP_SIZE-1 )
      {
        newY -= LOCAL_MAP_SIZE-1;
        ++nMaps;
      }
      --newY;

      //PUT OBJECT
      if (nMaps > 0 )
      {
        if (world.isSafe(globalX,globalY+nMaps))
        {
          aLocalTile(_object->x,_object->y).removeObject(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY+nMaps;
          world(globalX,globalY+nMaps)->putObject(_object,newX,newY);
          
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
          //std::cout<<"New gps coords: "<<_object->fullX<<", "<<_object->fullY<<".\n";
          
          
          //std::cout<<"World conversion:";
          world(_object->fullX,_object->fullY);
          
          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
          //std::cout<<"Abs to rel: "<<gX<<", "<<gY<<", "<<lX<<", "<<lY<<".\n";
          
          return false;
          
        }
      }
    }
      
    return false;

  }
  
  aLocalTile(_object->x,_object->y).removeObject(_object);
  




  
  _object->x=newX;
  _object->y=newY;
  _object->worldX = globalX;
  _object->worldY = globalY;
  
  aLocalTile(newX,newY).addObject(_object);
  
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
          //std::cout<<"New gps coords: "<<_object->fullX<<", "<<_object->fullY<<".\n";
          
          //std::cout<<"World conversion:";
          
          world(_object->fullX,_object->fullY);
          
          
          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
          //std::cout<<"Abs to rel: "<<gX<<", "<<gY<<", "<<lX<<", "<<lY<<".\n";
  return true;
}


    //Return a vector of coordinates visible from the given location.
Vector <HasXY*> * World_Local::rayTraceLOS (int _x, int _y, const int RANGE)
{
  if (RANGE <= 0) { return 0; }
  
  if (_x < 0 || _y < 0 || _x >= LOCAL_MAP_SIZE || _y >= LOCAL_MAP_SIZE )
  { return 0; }
  
  
  //Step 1: Get all raytrace coordinates.
  
  Vector <HasXY*> rayTraceCoordinates;
  
  int rayX = _x-RANGE;
  if (rayX<0) { rayX=0; }
    
  int rayY = _y-RANGE;
  if (rayY<0) { rayY=0; }
  
  int rayMaxX = _x+RANGE;
  
  if ( rayMaxX >= LOCAL_MAP_SIZE)
  {
    rayMaxX = LOCAL_MAP_SIZE - 1;
  }
  
  int rayMaxY = _y+RANGE;
  
  if ( rayMaxY >= LOCAL_MAP_SIZE)
  {
    rayMaxY = LOCAL_MAP_SIZE - 1;
  }
  
  int tempX = rayX;
  int tempY = rayY;
    
  //rayTraceCoordinates.push( new HasXY(tempX,tempY) );
  
  while (tempX <= rayMaxX)
  {
    rayTraceCoordinates.push( new HasXY(tempX,rayY) );
    rayTraceCoordinates.push( new HasXY(tempX,rayMaxY) );
    ++tempX;
  }
  
  // On the Y pass we prevent doing the corners again.
  ++tempY;
  while (tempY < rayMaxY)
  {
    rayTraceCoordinates.push( new HasXY(rayX,tempY) );
    rayTraceCoordinates.push( new HasXY(rayMaxX,tempY) );
    ++tempY;
  }
  
  // We now have a list of coordinates to raytrace.
  //std::cout<<"RayTrace Coordinats size: "<<rayTraceCoordinates.size()<<".\n";
  
  auto vVisibleTiles = new Vector <HasXY*>;
  
  
  for (int i=0;i<rayTraceCoordinates.size();++i)
  {
    rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
  }
  
  
  return vVisibleTiles;
}

    //Return a vector of coordinates visible from the given location.
    // New version using global coordinates
    // Yes, this is a confusing overload which should be fixed in the future.
Vector <HasXY2 <unsigned long int> *> * World_Local::rayTraceLOS (long unsigned int _x, long unsigned int _y, const int RANGE)
{
  if (RANGE <= 0) { return 0; }
  
  if ( _x >= (long unsigned int) LOCAL_MAP_SIZE*world.nX || _y >= (long unsigned int) LOCAL_MAP_SIZE*world.nY )
  { return 0; }
  
  
  //Step 1: Get all raytrace coordinates.
  
  Vector <HasXY2 <unsigned long int> *> rayTraceCoordinates;
  
  unsigned long int rayX = _x-RANGE;
  
  //overflow check
  if ( rayX > world.maximumX )
  { rayX = 0; }
  
  unsigned long int rayY = _y-RANGE;
  
  //overflow check
  if ( rayY > world.maximumY )
  { rayY = 0; }
  
  unsigned long int rayMaxX = _x+RANGE;
  
  //overflow check
  if ( rayMaxX > world.maximumX )
  { rayMaxX = world.maximumX; }
  
  unsigned long int rayMaxY = _y+RANGE;
  
  //overflow check
  if ( rayMaxY > world.maximumY )
  { rayMaxY = world.maximumY; }
  
  //Easy check for overflow
  
  
  unsigned long int tempX = rayX;
  unsigned long int tempY = rayY;
    
  auto hXY = new HasXY2 <unsigned long int>;
    
  // //rayTraceCoordinates.push( new HasXY(tempX,tempY) );
  
  // while (tempX <= rayMaxX)
  // {
    // rayTraceCoordinates.push( new HasXY(tempX,rayY) );
    // rayTraceCoordinates.push( new HasXY(tempX,rayMaxY) );
    // ++tempX;
  // }
  
  // // On the Y pass we prevent doing the corners again.
  // ++tempY;
  // while (tempY < rayMaxY)
  // {
    // rayTraceCoordinates.push( new HasXY(rayX,tempY) );
    // rayTraceCoordinates.push( new HasXY(rayMaxX,tempY) );
    // ++tempY;
  // }
  
  // // We now have a list of coordinates to raytrace.
  // //std::cout<<"RayTrace Coordinats size: "<<rayTraceCoordinates.size()<<".\n";
  
  auto vVisibleTiles = new Vector <HasXY2 <unsigned long int> *>;
  
  
  for (int i=0;i<rayTraceCoordinates.size();++i)
  {
    //rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
  }
  
  
  return vVisibleTiles;
}

void World_Local::rayTrace (int _x1, int _y1, int _x2, int _y2, Vector <HasXY*> * vVisibleTiles)
{
  // Old code from ECHO
  
  //std::cout<<"Tracing ray: ("<<_x1<<", "<<_y1<<") ("<<_x2<<", "<<_y2<<").\n";
  
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


  double slope = BasicMath::getSlope(_x1,_y1,_x2,_y2);

  // IF SLOPE IS INFINITY, CHANGE VALUE TO 0.
  if ( slope == std::numeric_limits<double>::infinity() )
  { slope=0; }

			// SPECIAL CASE: 1 TILE.
			// REVEAL TILE STANDING ON.
		if ( (_x1==_x2) && (_y1==_y2) )
		{
			if ( aLocalTile.isSafe(_x1,_y2) == true )
			{ vVisibleTiles->push(new HasXY (_x1,_y1) ); }
		}
    
			// SPECIAL CASE: UP/DOWN
		else if (_x1 == _x2)
		{
      do
      {
				if ( aLocalTile.isSafe(_x1,_y1) == true )
				{
					vVisibleTiles->push(new HasXY (_x1,_y1) );
          
          if (isBlockingView(_x1,_y1))
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
      { vVisibleTiles->push(new HasXY (_x1,_y1) );
      }
      
			// while ( _y1 != _y2 )
			// {

			// }
      //Final case
      //vVisibleTiles->push(new HasXY (_x1,_y1) );
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

				int roundedY=0;

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

				if ( aLocalTile.isSafe(_x1,roundedY) == true )
				{
					vVisibleTiles->push(new HasXY (_x1,roundedY) );
          
          if (isBlockingView(_x1,roundedY))
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
      { vVisibleTiles->push(new HasXY (_x1,_y2) );
      }
      //Final case
      //vVisibleTiles->push(new HasXY (_x1,_y1) );
		}
			//STEEP SLOPE.
		else
		{
			slope = BasicMath::getSlope(_y1,_x1,_y2,_x2);

			if (_y1>_y2)
			{ slope*=-1; }

			double currentX = _x1;

			//while (_y1 != _y2 )
      do
			{
				int roundedX=0;

				if ( _x1 > _x2 )
				{ roundedX = floor(currentX); }
				else if ( _x1 < _x2 )
				{ roundedX = ceil(currentX); }
				else
				{ roundedX = round(currentX); }

				if ( aLocalTile.isSafe(roundedX,_y1) == true )
				{
					vVisibleTiles->push(new HasXY (roundedX,_y1) );
          
          if (isBlockingView(roundedX,_y1))
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
      { vVisibleTiles->push(new HasXY (_x2,_y1) );
      } 
      //Final case
      //vVisibleTiles->push(new HasXY (_x1,_y1) );
		}

}

bool World_Local::isBlockingView(int _x, int _y)
{
  return aLocalTile(_x,_y).hasViewBlocker();

}

// void World_Local::getRandomTile (int* x, int* y)
// {
	// *x = Random::randomInt(nX);
	// *y = Random::randomInt(nY);
// }


#endif
