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
      aLocalTile(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aLocalTile(_x,_y).clearObjects();
      aLocalTile(_x,_y).height = aLocalHeight(_x,_y);
      
      
      aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
      aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      aSubterranean(_x,_y).clearObjects();
      aSubterranean(_x,_y).height = -1;
      
      if (random.oneIn(100))
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
      
      if (random.oneIn(100))
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

bool World_Local::moveObject (WorldObject* _object, int newX, int newY )
{
  if ( aLocalTile.isSafe(newX,newY) == false )
  {
    return false;
  }
  
  aLocalTile(_object->x,_object->y).removeObject(_object);
  
  _object->x=newX;
  _object->y=newY;
  aLocalTile(newX,newY).addObject(_object);
  return true;
}

// void World_Local::getRandomTile (int* x, int* y)
// {
	// *x = Random::randomInt(nX);
	// *y = Random::randomInt(nY);
// }


#endif
