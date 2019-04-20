#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_CPP
#define WORLDSIM_WORLD_LOCAL_CPP

/* WorldSim: World_Local
	#include "World_Local.cpp"
  
  Implementation of World_Local.hpp
*/

#include "World_Local.hpp"

#include "WorldObject.hpp"
  #include "WorldObject_Tree.hpp"
  #include "WorldObject_Flora.hpp"
  #include "WorldObject_Rock.hpp"
  #include "WorldObject_Sign.hpp"


#include "Creature.hpp"
  #include "Creature_Deer.hpp"
  #include "Creature_All.hpp"



World_Local::World_Local()
{
	globalX=0;
	globalY=0;
  
  initialized = false;
	active = false;
  
	nX=LOCAL_MAP_SIZE;
	nY=LOCAL_MAP_SIZE;
  
  hasRiver=-1;
  hasCave=false;
  hasRuin=false;
  
	seed = 0;
	baseBiome = NOTHING;
	//baseMoveCost = 0;
	//canHaveSettlement = false;
	baseFertility = 0;
	canMove = 0;
	//baseLogisticsCost = 0;
	//defensiveBonus = 0;
  baseMetal=0;
  
  //vAllTiles.reserve(0);
  vCreature.reserve(0);
  vCharacter.reserve(0);
  vItem.reserve(0);
  vObjectGeneric.reserve(0);
  
  data=0;
}

World_Local::~World_Local()
{
  
  initialized = false;
	active = false;
  
  vCreature.deleteAll();
  vItem.deleteAll();
  vObjectGeneric.deleteAll();
  
  if (data!=0)
  { delete data; }
  data=0;
}

void World_Local::unload()
{
	active = false;
  
  vCreature.deleteAll();
  vItem.deleteAll();
  vObjectGeneric.deleteAll();
  
  if (data!=0)
  { delete data; }
  data=0;
} 


void World_Local::init(const int _globalX, const int _globalY, const enumBiome _biomeID, const int _seed = 0, const int _hasRiver=-1)
{
  baseBiome = _biomeID;
  seed = _seed;
  hasRiver = _hasRiver;
  
  globalX = _globalX;
  globalY = _globalY;
  
  // This needs to be changed to initialise only when generated.
  //aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  //aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
    
  switch(_biomeID) {
    
   //baseMoveCost=0;
    //canHaveSettlement=false;
    case NOTHING:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 0;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case OCEAN:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case GRASSLAND:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 4;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case FOREST:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case DESERT:
    {
     //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case MOUNTAIN:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case SNOW:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case HILLY:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case JUNGLE:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case WETLAND:
    {
     // baseMoveCost = 0;
     // canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case STEPPES:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case RIVER:
    {
      //baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    default:
    {
      ///baseMoveCost = 0;
      //canHaveSettlement = false;
      baseFertility = 0;
      canMove = 0;
      //baseLogisticsCost = 0;
      //defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    break;
}
    
    if ( baseBiome == MOUNTAIN )
    {
      //std::cout<<"Adding resources to mountain.\n";
      baseMetal=1000;
    }
    else
    {
      baseMetal=0;
    }

}

inline LocalTile* World_Local::operator() (int _x, int _y)
{
  if (data && isSafe(_x,_y))
  {
    return &data->aLocalTile(_x,_y);
  }
  return 0;
}

    // Returns true if the coordinate is inside this map
bool World_Local::isSafe(const int _x, const int _y)
{
  if (data==0) { return false; }
  return data->aLocalTile.isSafe(_x,_y);
}
bool World_Local::isSafe(HasXY* xy)
{
  if (data==0) { return false; }
  return data->aLocalTile.isSafe(xy);
}
bool World_Local::isSafe(WorldObject* _object)
{
  if (data==0) { return false; }
  return data->aLocalTile.isSafe(_object->x,_object->y);
}

bool World_Local::generate()
{
  // World is already loaded and running
  if ( active ) { return true; }
  
  // World has invalid coordinates so we can't generate it.
  if ( world.isSafe(globalX,globalY) == false )
  { return false; }

  // Create data struct if necessary.
  if (data==0) { data = new Data; }
  
  if (initialized)
  {
    //std::cout<<"World was already previously generated. We should load it from cache.\n";
    load();
    active = true;
    return true;
  }
  
  localDate.set(0,0,0,CALENDAR_INITIAL_HOUR,CALENDAR_INITIAL_MINUTE,0);

  initialized = true;
  active = true;
  

  data->aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  data->aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);

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
        aLocalHeight(_x,_y) = aLocalHeight(_x,_y)/20;
      }
    }
  }
  else
  {
    //HEIGHTMAP TABLE FREESTEPS SMOOTHING
    dsa2.generate(&aLocalHeight,0,0,0.75,100);
  }
  
  // Take the seed for this world tile and expand it into a subseed for every local tile */
  random.seed(seed);
  
  int nGemSeams = random.randomInt(3);
  while (nGemSeams-- > 0)
  {
    Vector <HasXY*> * vGemSeam = getRandomWalk(random.randomInt(40)+10);
    
    for (int i=0;i<vGemSeam->size();++i)
    {
      data->aSubterranean((*vGemSeam)(i)).nGems=random.multiRoll(3,3);
    }
    vGemSeam->deleteAll();
    delete vGemSeam;
  }
  
  int nMetalSeams = random.randomInt(6);
  while (nMetalSeams-- > 0)
  {
    Vector <HasXY*> * vMetalSeam = getRandomWalk(random.randomInt(40)+10);
    
    for (int i=0;i<vMetalSeam->size();++i)
    {
      data->aSubterranean((*vMetalSeam)(i)).nMetal=random.multiRoll(3,3);
    }
    vMetalSeam->deleteAll();
    delete vMetalSeam;
  }
  
  int midX = LOCAL_MAP_SIZE/2;

  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      data->aLocalTile(_x,_y).seed = random.randInt(USHRT_MAX-1);
      //data->aLocalTile(_x,_y).clearObjects();
      data->aLocalTile(_x,_y).height = aLocalHeight(_x,_y);

      data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
      data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
      //data->aSubterranean(_x,_y).clearObjects();
      data->aSubterranean(_x,_y).height = 0;
      
        
      //data->aSubterranean(_x,_y).hasGems=Random::oneIn(100);
      if (Random::oneIn(1000))
      {
        //data->aSubterranean(_x,_y).hasGems=true;
      }
        
      // GENERATE RIVERS
      // FOR NOW THEY JUST RUN ALONG THE EDGE OF THE MAP
      if ( ((riverConnections & 0b01000010) == 0b01000010
       || (riverConnections & 0b00001010) == 0b00001010
       || (riverConnections & 0b11111111) == 0b00000010
       || (riverConnections & 0b11111111) == 0b01000000
      )
      && _x > LOCAL_MAP_SIZE-3 )
      {
        data->aLocalTile(_x,_y).baseTerrain = OCEAN;  
        continue;
      }
      if ( ((riverConnections & 0b00011000) == 0b00011000
        || (riverConnections & 0b01010000) == 0b01010000
       || (riverConnections & 0b11111111) == 0b00001000
       || (riverConnections & 0b11111111) == 0b00010000
        )
            && _y > LOCAL_MAP_SIZE-3 )
      {
        data->aLocalTile(_x,_y).baseTerrain = OCEAN;
        continue;
      }
      if ( (riverConnections & 0b01001000) == 0b01001000
            && _y > LOCAL_MAP_SIZE-3 && _x > LOCAL_MAP_SIZE-3 )
      {
        data->aLocalTile(_x,_y).baseTerrain = OCEAN;
        continue;
      }
      // Left and down (runs along right and top)
      if ( (riverConnections & 0b00010010) == 0b00010010
            && (_y > LOCAL_MAP_SIZE-3 || _x > LOCAL_MAP_SIZE-3) )
      {
        data->aLocalTile(_x,_y).baseTerrain = OCEAN;
        continue;
      }

      data->aLocalTile(_x,_y).baseTerrain = baseBiome;
      
      if ( baseBiome != OCEAN )
      {
        int baseTreeChance = 40;
        int basePlantChance = 20;
        
        if ( baseBiome == FOREST )
        {
          data->aLocalTile(_x,_y).baseTerrain = GRASSLAND;
          baseTreeChance=9;
        }
        else if ( baseBiome == JUNGLE )
        {
          data->aLocalTile(_x,_y).baseTerrain = GRASSLAND;
          baseTreeChance=5;
        }
        else if ( baseBiome == DESERT )
        {
          data->aLocalTile(_x,_y).baseTerrain = DESERT;
          baseTreeChance=200;
        }
        else if (baseBiome == STEPPES )
        {
          data->aLocalTile(_x,_y).baseTerrain = STEPPES;
          baseTreeChance=100;
        }
        else if (baseBiome == SNOW )
        {
          data->aLocalTile(_x,_y).baseTerrain = SNOW;
          baseTreeChance=100;
        }
        else if (baseBiome == WETLAND )
        {
          data->aLocalTile(_x,_y).baseTerrain = WETLAND;
          baseTreeChance=9;
        }
        
        //Put down some water for drinking
        if (Random::oneIn(500))
        {
          data->aLocalTile(_x,_y).baseTerrain = OCEAN;
          continue;
        }
        else if (random.oneIn(400)) /* Put down some testing objects */
        {
          put(new Item_Floor, _x, _y);
          put(new Item_Sword, _x, _y);
          put(new Item_Longbow, _x, _y);
          put(new Item_Fishrod, _x, _y);
          put(new Item_Shovel, _x, _y);
          put(new Item_Axe, _x, _y);
          put(new Item_Fish, _x, _y);
          put(new Item_Fish, _x, _y);
          put(new Item_Campfire, _x, _y);
          put(new Item_Log, _x, _y);
          put(new Item_Plank, _x, _y);
          put(new Item_Plank, _x, _y);
          put(new Item_Wall, _x, _y);
          put(new Item_Wall, _x, _y);
          put(new Item_Wall, _x, _y);
          put(new Item_PlantFibre, _x, _y);
          put(new Item_Knife, _x, _y);
          put(new Item_Pickaxe, _x, _y);
        }
        else if (random.oneIn(baseTreeChance))
        {
          if (random.oneIn(10))
          {
            put(new WorldObject_Tree(0), _x, _y);
          }
          else
          {
            put(new WorldObject_Tree(1), _x, _y);
          }

        }
        else if (random.oneIn(basePlantChance))
        {
          if (random.oneIn(500))
          {
            put(new Flora_Blueweed(),_x,_y);
          }
          else
          {
            put(new WorldObject_Flora(),_x,_y);
          }
          
        }
        else if (random.oneIn(1500) && (baseBiome == FOREST || baseBiome == GRASSLAND) )
        {
          auto deer = new Creature_Deer;
          deer->init();
          put(deer,_x,_y);
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
            put (rockyBoi,_x,_y);
          }
          
        }
      }
    }
  }
  
  if ( baseBiome == GRASSLAND && random.oneIn(100) )
  {
    for ( int x = 10; x<20; ++x)
    {
      for ( int y=10;y<20; ++y)
      {
        data->aLocalTile(x,y).hasFloor = true;
      }
    }
    
    auto sign = new WorldObject_Sign;
    data->aLocalTile(21,21).add(sign);
  }
  
  
  //BUILD CAVE
  //if (Random::oneIn(2))
  if (true)
  {
    hasCave=true;
    //Basically random walk with extras, and then occasionally breach the surface in the form of a cave tile.
    
    //int caveSize = Random::randomInt(LOCAL_MAP_SIZE*5)+3;
    int caveSize = Random::multiRoll(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE)*2;
    
    Vector <HasXY*> * vCaveMap = getRandomWalk(caveSize);
      
    int nEntrances = Random::randomInt((vCaveMap->size()/500));
    if (nEntrances > 10) { nEntrances=10; }
    nEntrances = 15;
    
    for (int i2=0;i2<vCaveMap->size();++i2)
    {
      data->aSubterranean((*vCaveMap)(i2)).baseTerrain = GRASSLAND;
      data->aSubterranean((*vCaveMap)(i2)).nGems=0;
      data->aSubterranean((*vCaveMap)(i2)).nMetal=0;
      
      if (Random::oneIn(100))
      {
        auto creature = new Creature_Bat;
        creature->init();
        put(creature,(*vCaveMap)(i2),true);
      }
      
      if (Random::oneIn(25))
      {
        put(new Flora_Blackweed(),(*vCaveMap)(i2),true);
      }
      
      if (i2<nEntrances)
      {
        data->aLocalTile((*vCaveMap)(i2)).isCave=true;
        data->aSubterranean((*vCaveMap)(i2)).isCave=true;
      }
    }
    vCaveMap->deleteAll();
    delete vCaveMap;
  }

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

      //currentTribe->vCharacter(i2)->x=randX;
      //currentTribe->vCharacter(i2)->y=randY;
      currentTribe->vCharacter(i2)->map=this;
      put(currentTribe->vCharacter(i2),randX,randY);
      currentTribe->vCharacter(i2)->initialiseKnowledge();
      //vCharacter.push(currentTribe->vCharacter(i2));
      //std::cout<<"vChar size: "<<vCharacter.size()<<".\n";
      
      //aLocalTile(randX,randY).addObject(currentTribe->vCharacter(i2));
      //aLocalTile(Random::randInt(nX-1),Random::randInt(nY-1)).addObject(c);
    
    }
    

  }
  delete vTribesHere;
  
  save();

	return false;
}

bool World_Local::save()
{
  // Can't save world if it's not fully generated.
  if ( data==0 ) { return false; }
  
  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	
  //std::cout<<"Savefile for this map is: "<<localMapPath<<"\n";
	// FileManager::createDirectory(strSavePath);
	
	if ( FileManager::directoryExists(world.strSavePath)  == false )
  {
    std::cout<<"Error: Unable to access directory.\n";
    return false;
	}
  
  // Make the file or clear it.
  FileManager::makeNewFile(localMapPath);
  SaveFileManager sfm;
  
  std::string saveData="";
  
  SaveChunk chonk ("TILE ARRAY");
  
  // Only unload the local map if it is loaded.
  for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      chonk.add(data->aLocalTile(_x,_y).getSaveData());
    }
  }
  
  sfm.addChunk(chonk);
  
  SaveChunk chonkSub ("SUBTERRANEAN ARRAY");
  SaveChunk chonkGem ("GEM ARRAY");
  SaveChunk chonkMetal ("METAL ARRAY");
  
  for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      chonkSub.add(data->aSubterranean(_x,_y).getSaveData());
      chonkGem.add(DataTools::toString(data->aSubterranean(_x,_y).nGems));
      chonkMetal.add(DataTools::toString(data->aSubterranean(_x,_y).nMetal));
    }
  }
  sfm.addChunk(chonkSub);
  sfm.addChunk(chonkGem);
  sfm.addChunk(chonkMetal);
  
  SaveChunk chonkObjects ("OBJECT VECTOR");
  
  for (int i=0;i<vObjectGeneric.size();++i)
  {
    chonkObjects.add(vObjectGeneric(i)->getBaseData());
  }
  
  SaveChunk chonkItems ("ITEM VECTOR");
  for (int i=0;i<vItem.size();++i)
  {
    chonkItems.add(vItem(i)->getBaseData());
  }
  
  sfm.addChunk(chonkObjects);
  sfm.addChunk(chonkItems);
  
  // Only unload the local map if it is loaded.
  // for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  // {
    // for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    // {
      // ++i;
      // chonk.add(data->aLocalTile(_x,_y).getSaveData());
    // }
  // }
  // Vector <Creature*> vCreature;
  // //Vector of all Characters on this map
  // Vector <Character*> vCharacter;
  // //Vector of all Items on this map
  // Vector <Item*> vItem;
  // // Vector of all non-categorised objects on this map.
  // Vector <WorldObject*> vObjectGeneric;
  
  
  
  sfm.saveToFile(localMapPath);

  return true;
}

bool World_Local::load()
{
  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
  
  //std::cout<<"Attempting to load cached map: "<<localMapPath<<".\n";

	if ( FileManager::directoryExists(world.strSavePath)  == false )
  {
    std::cout<<"Error: Unable to access directory.\n";
    return false;
	}
  
  initialized = true;
  active = true;
  
  if ( data == 0 )
  { data=new Data;
  }
  data->aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  data->aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  
  
  // Open the cache file for loading into memory.
  SaveFileManager sfm;
  
  std::string saveData="";
  sfm.loadFile(localMapPath);
  
  //SaveChunk chonk ("TILEARRAY");
  SaveChunk* chonk = sfm.getChunk("TILE ARRAY");
  
  if ( chonk != 0 )
  {
    //std::cout<<"Chonks: "<<chonk->vData.size()<<".\n";

    int i=0;
    // Only unload the local map if it is loaded.
    for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
    {
      for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
      {
        //data->aLocalTile(_x,_y).clearObjects();
        data->aLocalTile(_x,_y).loadData((*chonk)(i));
        
        data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
        data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
        //data->aSubterranean(_x,_y).clearObjects();
        data->aSubterranean(_x,_y).height = 0;

        //data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
        //data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
        //data->aSubterranean(_x,_y).clearObjects();
        //data->aSubterranean(_x,_y).height = 0;
        //chonk.add(data->aLocalTile(_x,_y).getSaveData());
        ++i;
      }
    }
    delete chonk;
    
    SaveChunk* chonkSub = sfm.getChunk("SUBTERRANEAN ARRAY");
    SaveChunk* chonkGem = sfm.getChunk("GEM ARRAY");
    SaveChunk* chonkMetal = sfm.getChunk("METAL ARRAY");
    
    if ( chonkSub != 0 )
    {
      //std::cout<<"Chonks: "<<chonk->vData.size()<<".\n";

      i=0;
      // Only unload the local map if it is loaded.
      for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
      {
        for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
        {
          data->aSubterranean(_x,_y).loadData((*chonkSub)(i));
          data->aSubterranean(_x,_y).nGems=DataTools::toUnsignedShort((*chonkGem)(i));
          data->aSubterranean(_x,_y).nMetal=DataTools::toUnsignedShort((*chonkMetal)(i));
          
          //data->aLocalTile(_x,_y).clearObjects();
          //data->aLocalTile(_x,_y).loadData((*chonk)(i));
          
          // data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
          // data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
          // //data->aSubterranean(_x,_y).clearObjects();
          // data->aSubterranean(_x,_y).height = 0;

          //data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
          //data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
          //data->aSubterranean(_x,_y).clearObjects();
          //data->aSubterranean(_x,_y).height = 0;
          //chonk.add(data->aLocalTile(_x,_y).getSaveData());
          ++i;
        }
      }
      delete chonkSub;
    }
    
  

    chonk = sfm.getChunk("OBJECT VECTOR");
    
    //std::cout<<"Chonks2: "<<chonk->vData.size()<<".\n";
    
    for (i=0;i<chonk->vData.size();++i)
    {
      //std::cout<<"Current object chonk: "<<chonk->vData(i)<<".\n";
      
      //std::cout<<"Tokenising...\n";
      
      Vector <std::string> * vToke = DataTools::tokenize(chonk->vData(i), " \t\n\r");
      
      if ( vToke && vToke->size() >= 3)
      {
        //std::cout<<"Object type: "<<(*vToke)(0)<<".\n";
        //std::cout<<"X: "<<(*vToke)(1)<<".\n";
        //std::cout<<"Y: "<<(*vToke)(2)<<".\n";
        
        std::string objectType = (*vToke)(0);
        short int objectX = DataTools::toShort((*vToke)(1));
        short int objectY = DataTools::toShort((*vToke)(2));
        
        //std::cout<<"Tree coords: "<<objectX<<", "<<objectY<<".\n";
        
        
        if (objectType == "Tree" )
        {
         //std::cout<<"Loading a tree.\n";
          
          if (isSafe( objectX, objectY ) )
          {
            put (new WorldObject_Tree(1), objectX, objectY);
          }
          
        }
        
        delete vToke;
      }

    }
  }
  else
  {
    std::cout<<"Chonk fail\n";
    return false;
  }

  return true;
}


bool World_Local::put (WorldObject* _object, int _x, int _y, bool subterranean)
{
  if ( !data ) { return false; }
  
  if ( data->aLocalTile.isSafe(_x,_y) == false )
  { return false; }

  _object->worldX = globalX;
  _object->worldY = globalY;
  _object->x = _x;
  _object->y = _y;  
  _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;
  
  _object->isUnderground = subterranean;
  
  if ( subterranean )
  {
    data->aSubterranean(_x,_y).add(_object);
  }
  else
  {
    data->aLocalTile(_x,_y).add(_object);
  }

  vObjectGeneric.push(_object);
  return true;
}
bool World_Local::put (WorldObject* _object, HasXY* _xy, bool subterranean)
{ return put(_object, _xy->x, _xy->y, subterranean); }
bool World_Local::put (WorldObject* _object, HasXY _xy, bool subterranean)
{ return put(_object, _xy.x, _xy.y, subterranean); }

bool World_Local::put (Item* _item, int _x, int _y, bool subterranean)
{
  if ( !data ) { return false; }
  
  if ( data->aLocalTile.isSafe(_x,_y) == false )
  { return false; }

  _item->worldX = globalX;
  _item->worldY = globalY;
  _item->x = _x;
  _item->y = _y;
  _item->fullX = _item->worldX * LOCAL_MAP_SIZE + _item->x;
  _item->fullY = _item->worldY * LOCAL_MAP_SIZE + _item->y;
  
  _item->isUnderground = subterranean;

  
  if ( subterranean )
  {
    data->aSubterranean(_x,_y).add(_item);
  }
  else
  {
    data->aLocalTile(_x,_y).add(_item);
  }

  
  vItem.push(_item);
  return true;
}
bool World_Local::put (Item* _object, HasXY* _xy, bool subterranean)
{ return put(_object, _xy->x, _xy->y, subterranean); }
bool World_Local::put (Item* _object, HasXY _xy, bool subterranean)
{ return put(_object, _xy.x, _xy.y, subterranean); }

bool World_Local::put (Character* _character, int _x, int _y, bool subterranean)
{
  if ( !data ) { return false; }
  if ( data->aLocalTile.isSafe(_x,_y) == false )
  { return false; }

  _character->worldX = globalX;
  _character->worldY = globalY;
  _character->x = _x;
  _character->y = _y;
  _character->fullX = _character->worldX * LOCAL_MAP_SIZE + _character->x;
  _character->fullY = _character->worldY * LOCAL_MAP_SIZE + _character->y;
  
  _character->isUnderground = subterranean;

  if ( subterranean )
  {
    data->aSubterranean(_x,_y).add(_character);
  }
  else
  {
    data->aLocalTile(_x,_y).add(_character);
  }
  
  vCharacter.push(_character);
  
  return true;
}
bool World_Local::put (Character* _object, HasXY* _xy, bool subterranean)
{ return put(_object, _xy->x, _xy->y, subterranean); }
bool World_Local::put (Character* _object, HasXY _xy, bool subterranean)
{ return put(_object, _xy.x, _xy.y, subterranean); }

bool World_Local::put (Creature* _creature, int _x, int _y, bool subterranean)
{
  if ( !data ) { return false; }
  if ( data->aLocalTile.isSafe(_x,_y) == false )
  { return false; }

  _creature->worldX = globalX;
  _creature->worldY = globalY;
  _creature->x = _x;
  _creature->y = _y;
  _creature->fullX = _creature->worldX * LOCAL_MAP_SIZE + _creature->x;
  _creature->fullY = _creature->worldY * LOCAL_MAP_SIZE + _creature->y;
  
  _creature->isUnderground = subterranean;

  if ( subterranean )
  {
    data->aSubterranean(_x,_y).add(_creature);
  }
  else
  {
    data->aLocalTile(_x,_y).add(_creature);
  }
  
  
  vCreature.push(_creature);
  _creature->map=this;
  
  return true;
}
bool World_Local::put (Creature* _object, HasXY* _xy, bool subterranean)
{ return put(_object, _xy->x, _xy->y, subterranean); }
bool World_Local::put (Creature* _object, HasXY _xy, bool subterranean)
{ return put(_object, _xy.x, _xy.y, subterranean); }

// bool World_Local::put (Creature_Deer* _creature, int _x, int _y)
// {
  // if ( aLocalTile.isSafe(_x,_y) == false )
  // { return false; }

  // std::cout<<"ADDING DEER\n";

  
  // return true;
// }


  // We need to implement optional map-only restriction
bool World_Local::moveObject (WorldObject* _object, int newX, int newY )
{
  if ( !data ) { return false; }
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
    // Calculate new map if we're moving between maps.
  
  int destinationGlobalX = globalX;
  int destinationGlobalY = globalY;
  
  // Left/right
  while ( newX < 0 )
  {
    newX += LOCAL_MAP_SIZE;
    --destinationGlobalX;
  }

  while ( newX > LOCAL_MAP_SIZE-1 )
  {
    newX -= LOCAL_MAP_SIZE;
    ++destinationGlobalX;
  }

  // Up/down
  while ( newY < 0 )
  {
    newY += LOCAL_MAP_SIZE;
    --destinationGlobalY;
  }

  while ( newY > LOCAL_MAP_SIZE-1 )
  {
    newY -= LOCAL_MAP_SIZE;
    ++destinationGlobalY;
  }

  
  if ( world.isSafe(destinationGlobalX, destinationGlobalY) == false )
  { return false; }
  
  World_Local * destination = world(destinationGlobalX, destinationGlobalY);
  
  if ( destination == 0 || destination->data == 0 || destination->isSafe (newX,newY) == false ) { return false; }
  
  
  if (_object->isUnderground)
  {
    if ( data->aSubterranean(newX,newY).hasMovementBlocker() )
    { return false; }
  
    data->aSubterranean(_object->x,_object->y).remove(_object);
    
    _object->x=newX;
    _object->y=newY;
    _object->worldX = destinationGlobalX;
    _object->worldY = destinationGlobalY;
    
    destination->put(_object,newX,newY,true);
  }
  else
  {
    if ( data->aLocalTile(newX,newY).hasMovementBlocker() )
    {
      return false;
    }
    data->aLocalTile(_object->x,_object->y).remove(_object);
    
    
    _object->x=newX;
    _object->y=newY;
    _object->worldX = globalX;
    _object->worldY = globalY;
    
    destination->put(_object,newX,newY);
  }

  _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;

  //world(_object->fullX,_object->fullY);
  
   int gX = 0;
   int gY = 0;
  int lX = 0;
  int lY = 0;
  world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);

  return true;
}

  //Yeah this is a mess.
bool World_Local::moveObject (Character* _object, int newX, int newY )
{
  if ( !data ) { return false; }
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
    // Calculate new map if we're moving between maps.
  
  int destinationGlobalX = globalX;
  int destinationGlobalY = globalY;
  
  // Left/right
  while ( newX < 0 )
  {
    newX += LOCAL_MAP_SIZE;
    --destinationGlobalX;
  }

  while ( newX > LOCAL_MAP_SIZE-1 )
  {
    newX -= LOCAL_MAP_SIZE;
    ++destinationGlobalX;
  }

  // Up/down
  while ( newY < 0 )
  {
    newY += LOCAL_MAP_SIZE;
    --destinationGlobalY;
  }

  while ( newY > LOCAL_MAP_SIZE-1 )
  {
    newY -= LOCAL_MAP_SIZE;
    ++destinationGlobalY;
  }

  
  if ( world.isSafe(destinationGlobalX, destinationGlobalY) == false )
  { return false; }
  
  World_Local * destination = world(destinationGlobalX, destinationGlobalY);
  
  if ( destination == 0 || destination->data == 0 || destination->isSafe (newX,newY) == false ) { return false; }
  
  
  if (_object->isUnderground)
  {
    if ( destination->data->aSubterranean(newX,newY).hasMovementBlocker() )
    { return false; }
  
    data->aSubterranean(_object->x,_object->y).remove(_object);
    
    _object->x=newX;
    _object->y=newY;
    _object->worldX = destinationGlobalX;
    _object->worldY = destinationGlobalY;

    destination->put(_object,newX,newY,true);
  }
  else
  {
    if ( destination->data->aLocalTile(newX,newY).hasMovementBlocker() )
    { return false; }
  
    data->aLocalTile(_object->x,_object->y).remove(_object);
    
    _object->x=newX;
    _object->y=newY;
    _object->worldX = globalX;
    _object->worldY = globalY;
    
    destination->put(_object,newX,newY);
  }
  
  _object->map = destination;
  _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;

  //world(_object->fullX,_object->fullY);
  
   int gX = 0;
   int gY = 0;
  int lX = 0;
  int lY = 0;
  world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);

  return true;
  
  // LocalTile* destination;
  // LocalTile* source;
  // if ( _object->isUnderground )
  // {
    // destination = &data->aSubterranean(newX,newY);
    // source = &data->aSubterranean(_object->x,_object->y);
  // }
  // else
  // {
    // destination = &data->aLocalTile(newX,newY);
    // source = &data->aLocalTile(_object->x,_object->y);
  // }
  
  // if ( destination->hasMovementBlocker() )
  // {
    // return false;
  // }
  
  // if (data->aLocalTile.isSafe(_object->x,_object->y)==false)
  // {
    // return false;
  // }
  
  // //Moving inside of map
  // // I'm sure you can do some fancy bitwise stuff here but I can't think of it right now.
  // // It'll probably have to be done in future when pathfinding is optimised.
  // //char combinedCollision = source->bWall | destination->bWall;
  
  // // Check walls
  // if (source->bWall!=0 || destination->bWall!=0)
  // {
    // //return false;
    // // West
    // if ( newX < _object->x)
    // {
      // if ( (source->bWall & 0b00010000) == 0b00010000
      // ||   (destination->bWall & 0b00000100) == 0b00000100 )
      // {
        // return false;
      // }
    // }
    // //East
    // else if (newX > _object->x)
    // {
      // if ( (source->bWall & 0b01000000) == 0b01000000
      // ||   (destination->bWall & 0b00000001) == 0b00000001 )
      // {
        // return false;
      // }
    // }
    // //North
    // else if (newY > _object->y)
    // {
      // if ( (source->bWall & 0b10000000) == 0b10000000
      // ||   (destination->bWall & 0b00000010) == 0b00000010 )
      // {
        // return false;
      // }
    // }
    // //South
    // else if (newY < _object->y)
    // {
      // if ( (source->bWall & 0b00100000) == 0b00100000
      // ||   (destination->bWall & 0b00001000) == 0b00001000 )
      // {
        // return false;
      // }
    // }
  // }
  
  // if (_object->isUnderground)
  // {
    // if ( data->aSubterranean(newX,newY).hasMovementBlocker() )
    // {
      // return false;
    // }
    // data->aSubterranean(_object->x,_object->y).remove(_object);
    
  
    // _object->x=newX;
    // _object->y=newY;
    // _object->worldX = globalX;
    // _object->worldY = globalY;
    
    // data->aSubterranean(newX,newY).add(_object);
  // }
  // else
  // {
    // if ( data->aLocalTile(newX,newY).hasMovementBlocker() )
    // {
      // return false;
    // }
    // data->aLocalTile(_object->x,_object->y).remove(_object);
    
    
    // _object->x=newX;
    // _object->y=newY;
    // _object->worldX = globalX;
    // _object->worldY = globalY;
    
    // data->aLocalTile(newX,newY).add(_object);
  // }

  // _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  // _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;

  // world(_object->fullX,_object->fullY);
  
  // int gX = 0;
  // int gY = 0;
  // int lX = 0;
  // int lY = 0;
  // world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
  
  // return true;
}

bool World_Local::moveObject(WorldObject*, const enumDirection direction)
{
  
  if ( direction == NORTH )
  {
  }
  
  return false;
}

bool World_Local::moveDown(WorldObject* _object)
{
  if ( !data || !_object ) { return false; }

  if ( isSafe(_object) )
  {
    if ( data->aLocalTile(_object->x,_object->y).isCave )
    {
      data->aLocalTile(_object->x,_object->y).remove(_object);
      _object->isUnderground=true;
      data->aSubterranean(_object->x,_object->y).add(_object);
      return true;
    }
  }
  return false;

}
bool World_Local::moveDown(Character* _object)
{
  if ( !data || !_object ) { return false; }

  if ( isSafe(_object) )
  {
    if ( data->aLocalTile(_object->x,_object->y).isCave )
    {
      data->aLocalTile(_object->x,_object->y).remove(_object);
      _object->isUnderground=true;
      data->aSubterranean(_object->x,_object->y).add(_object);
      return true;
    }
  }
  return false;
}


bool World_Local::moveUp(WorldObject* _object)
{
  if ( !data || !_object ) { return false; }

  if ( isSafe(_object) )
  {
    if ( data->aLocalTile(_object->x,_object->y).isCave )
    {
      data->aSubterranean(_object->x,_object->y).remove(_object);
      _object->isUnderground=false;
      data->aLocalTile(_object->x,_object->y).add(_object);
      return true;
    }
  }
  return false;

}
bool World_Local::moveUp(Character* _object)
{
  if ( !data || !_object ) { return false; }

  if ( isSafe(_object) )
  {
    if ( data->aLocalTile(_object->x,_object->y).isCave )
    {
      data->aSubterranean(_object->x,_object->y).remove(_object);
      _object->isUnderground=false;
      data->aLocalTile(_object->x,_object->y).add(_object);
      return true;
    }
  }
  return false;
}

  // Move object in random direction
  // For now objects don't leave their map.
bool World_Local::wander (WorldObject* _object)
{
  if ( _object==0 ) { return false; }
  if ( !data ) { return false; }
  
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
  int newX = _object->x;
  int newY = _object->y;
  
  int direction = random.randomInt(3);
  
  if ( direction==0 ) { ++newX; }
  else if ( direction==1 ) { --newX; }
  else if ( direction==2 ) { ++newY; }
  else { --newY; }
  
  if ( data->aLocalTile.isSafe(newX,newY) && data->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    data->aLocalTile(_object->x,_object->y).remove(_object);
    put(_object,newX,newY);
    return true;
  }

  

  return false;
}

  // Move object in random direction
  // For now objects don't leave their map.
bool World_Local::wander (Character* _object)
{
  if ( !data ) { return false; }
  if ( _object==0 ) { return false; }
  
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
  int newX = _object->x;
  int newY = _object->y;
  
  int direction = random.randomInt(3);
  
  if ( direction==0 ) { ++newX; }
  else if ( direction==1 ) { --newX; }
  else if ( direction==2 ) { ++newY; }
  else { --newY; }
  
  if ( data->aLocalTile.isSafe(newX,newY) && data->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    data->aLocalTile(_object->x,_object->y).remove(_object);
    put(_object,newX,newY);
    return true;
  }

  

  return false;
}

  // Move object in random direction
  // For now objects don't leave their map.
bool World_Local::wander (Creature* _object)
{
  if ( !data ) { return false; }
  if ( _object==0 ) { return false; }

  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
  int newX = _object->x;
  int newY = _object->y;
  
  int direction = random.randomInt(3);
  
  if ( direction==0 ) { ++newX; }
  else if ( direction==1 ) { --newX; }
  else if ( direction==2 ) { ++newY; }
  else { --newY; }
  
  if ( data->aLocalTile.isSafe(newX,newY) && data->aLocalTile(newX,newY).hasMovementBlocker() == false )
  {
    //aLocalTile(_object->x,_object->y).remove(_object);
    remove(_object);
    put(_object,newX,newY);
    return true;
  }

  

  return false;
}


bool World_Local::remove (WorldObject* _object )
{
  if ( !data ) { return false; }
  if ( _object==0) {return false;}
  vObjectGeneric.remove(_object);
  
  
  if (_object->isUnderground)
  {
    if (data->aSubterranean.isSafe(_object->x,_object->y))
    {
      data->aSubterranean(_object->x,_object->y).remove(_object);
    }
  }
  else if (data->aLocalTile.isSafe(_object->x,_object->y))
  {
    data->aLocalTile(_object->x,_object->y).remove(_object);
  }
  
  return false;
}

bool World_Local::remove (Item* _item )
{
  if ( !data ) { return false; }
  if ( _item==0) {return false;}
  vItem.remove(_item);
  
  if (_item->isUnderground)
  {
    if (data->aSubterranean.isSafe(_item->x,_item->y))
    {
      data->aSubterranean(_item->x,_item->y).remove(_item);
    }
  }
  else if (data->aLocalTile.isSafe(_item->x,_item->y))
  {
    data->aLocalTile(_item->x,_item->y).remove(_item);
  }
  
  return false;
}
bool World_Local::remove (Character* _character )
{
  if ( !data ) { return false; }
  if ( _character == 0 ) {return false;}
  vCharacter.remove(_character);
  
  if (_character->isUnderground)
  {
    if (data->aSubterranean.isSafe(_character->x,_character->y))
    {
      data->aSubterranean(_character->x,_character->y).remove(_character);
    }
  }
  else if (data->aLocalTile.isSafe(_character->x,_character->y))
  {
    data->aLocalTile(_character->x,_character->y).remove(_character);
  }
  
  return false;
}
bool World_Local::remove (Creature* _creature )
{
  if ( !data ) { return false; }
  if ( _creature == 0 ) {return false;}
  vCreature.remove(_creature);
  
  if (_creature->isUnderground)
  {
    if (data->aSubterranean.isSafe(_creature->x,_creature->y))
    {
      data->aSubterranean(_creature->x,_creature->y).remove(_creature);
    }
  }
  else if (data->aLocalTile.isSafe(_creature->x,_creature->y))
  {
    data->aLocalTile(_creature->x,_creature->y).remove(_creature);
  }
  
  return false;
}

bool World_Local::erase (WorldObject* _object )
{
  if ( !data ) { return false; }
  if ( _object==0) {return false;}
  vObjectGeneric.remove(_object);
  
  if (data->aLocalTile.isSafe(_object->x,_object->y))
  {
    data->aLocalTile(_object->x,_object->y).remove(_object);
  }
  delete _object;
  return false;
}

bool World_Local::erase (Item* _item )
{
  if ( !data ) { return false; }
  if ( _item==0) {return false;}
  vItem.remove(_item);
  
  if (data->aLocalTile.isSafe(_item->x,_item->y))
  {
    data->aLocalTile(_item->x,_item->y).remove(_item);
  }
  delete _item;
    
  return false;
}
bool World_Local::erase (Character* _character )
{
  if ( !data ) { return false; }
  if ( _character == 0 ) {return false;}
  vCharacter.remove(_character);
  
  if (data->aLocalTile.isSafe(_character->x,_character->y))
  {
    data->aLocalTile(_character->x,_character->y).remove(_character);
  }
  delete _character;
  return false;
}
bool World_Local::erase (Creature* _creature )
{
  if ( !data ) { return false; }
  if ( _creature == 0 ) {return false;}
  vCreature.remove(_creature);
  
  if (data->aLocalTile.isSafe(_creature->x,_creature->y))
  {
    data->aLocalTile(_creature->x,_creature->y).remove(_creature);
  }
  delete _creature;
  return false;
}

bool World_Local::contains(WorldObject* _target)
{
  if ( !data ) { return false; }
  if (vObjectGeneric.contains(_target))
  {
    return true;
  } return false;
}
bool World_Local::contains(Character* _target)
{
  if ( !data ) { return false; }
  if (vCharacter.contains(_target))
  {
    return true;
  } return false;
}

    //Return a vector of coordinates visible from the given location.
    // This raytrace stays within map boundaries.
Vector <HasXY*> * World_Local::rayTraceLOS (int _x, int _y, const int RANGE, bool subterranean)
{
  if ( !data ) { return 0; }
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
    rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles, subterranean);
  }
  
  rayTraceCoordinates.clearData();
  return vVisibleTiles;
}

    //Return a vector of coordinates visible from the given location.
    // New version using global coordinates
    // Yes, this is a confusing overload which should be fixed in the future.
Vector <HasXY2 <unsigned long int> *> * World_Local::rayTraceLOS (long unsigned int _x, long unsigned int _y, const int RANGE)
{
  if ( !data ) { return 0; }
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

  auto vVisibleTiles = new Vector <HasXY2 <unsigned long int> *>;
  
  return vVisibleTiles;
}

void World_Local::rayTrace (int _x1, int _y1, int _x2, int _y2, Vector <HasXY*> * vVisibleTiles, bool subterranean)
{
  if ( !data ) { return; }
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
			if ( data->aLocalTile.isSafe(_x1,_y2) == true )
			{ vVisibleTiles->push(new HasXY (_x1,_y1) ); }
		}
    
			// SPECIAL CASE: UP/DOWN
		else if (_x1 == _x2)
		{
      do
      {
				if ( data->aLocalTile.isSafe(_x1,_y1) == true )
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
		}
			//SHALLOW SLOPE
		else if ( xDiff <= yDiff )
		{
			if (_x1>_x2)
			{ slope*=-1; }

			double currentY = _y1;

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

				if ( data->aLocalTile.isSafe(_x1,roundedY) == true )
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
		}
			//STEEP SLOPE.
		else
		{
			slope = BasicMath::getSlope(_y1,_x1,_y2,_x2);

			if (_y1>_y2)
			{ slope*=-1; }

			double currentX = _x1;

      do
			{
				int roundedX=0;

				if ( _x1 > _x2 )
				{ roundedX = floor(currentX); }
				else if ( _x1 < _x2 )
				{ roundedX = ceil(currentX); }
				else
				{ roundedX = round(currentX); }

				if ( data->aLocalTile.isSafe(roundedX,_y1) == true )
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
		}
}

//SEARCH FUNCTIONS

Vector <Character*> * World_Local::getAdjacentCharacters(int _x, int _y)
{
  if ( !data ) { return 0; }
  auto vNearbyCharacters = new Vector <Character*>;
  for (int i=0;i<vCharacter.size();++i)
  {
    if ( vCharacter(i) != 0 && vCharacter(i)->distanceTo(_x,_y) <= 1)
    {
      vNearbyCharacters->push(vCharacter(i));
    }
  }
  
  return vNearbyCharacters;
}

HasXY* World_Local::getRandomTile()
{
  if ( !data ) { return 0; }
  auto xy = new HasXY ( Random::randomInt(nX-1), Random::randomInt(nY-1) );
  return xy;
}

HasXY* World_Local::getRandomNeighbor(HasXY* _source)
{
  if ( !data ) { return 0; }
  if (isSafe(_source->x,_source->y) == false) { return 0; }
  
  Vector <HasXY*> * vN = data->aLocalTile.getNeighborsOrthogonal(_source->x,_source->y,false,true);
  

  if ( vN!=0 && vN->size()>0 )
  {
    HasXY * returnValue = new HasXY((*vN)(0));
    //returnValue->set((*vN)(0));
    vN->clearData();
    delete vN;
    return returnValue;
  }
  if ( vN !=0 ) { delete vN; }
  
  return 0;
}

Vector <HasXY*> * World_Local::getRandomWalk(int _nSteps)
{
  auto vTile = new Vector <HasXY*>;
  
  HasXY* xy = getRandomTile();
  if ( xy )
  {
    for (int i2=0;i2<_nSteps;++i2)
    {
      if ( xy )
      {
        xy = getRandomNeighbor(xy);
        vTile->pushUniquePtr(xy);
      }
    }
  }
  vTile->shuffle();

  return vTile;
}
  

bool World_Local::isBlockingView(int _x, int _y)
{
  if ( !data ) { return true; }
  return data->aLocalTile(_x,_y).hasViewBlocker();

}

	// Increments the map by nTicks ticks. Higher values may lead to abstraction.
void World_Local::incrementTicks(int nTicks)
{
  if ( !data ) { return; }
  
  localDate.advanceSecond(nTicks);
  
  
  
  Vector <Creature*> vToMove;
  for (int i=0;i<vCreature.size();++i)
  {
    if (vCreature(i)->isAlive)
    {
      vToMove.push(vCreature(i));
    }
  }
  for (int i=0;i<vToMove.size();++i)
  {
    //wander(vToMove(i));
    vToMove(i)->incrementTicks(1);
    //vToMove(i)->wander();
  }

  
  Vector <Character*> vToMove2;
  for (int i=0;i<vCharacter.size();++i)
  {
    if (vCharacter(i)->isAlive && vCharacter(i) != playerCharacter)
    {
      vToMove2.push(vCharacter(i));
    }
  }
  for (int i=0;i<vToMove2.size();++i)
  {
    vToMove2(i)->wander();
  }
}

void World_Local::updateTickBacklog(Calendar _currentDate)
{
  // we need to figure out how many ticks behind we are.
  
  
  //int 
}


//WORLDTILE ALGORITHMS

void World_Local::addInfluence (Tribe* tribe, int amount)
{
  
  if ( tribe == 0 ) { return; }
  
  // Note that std::map will automatically initialise new entries.
  mInfluence[tribe] += amount;
  
  
  //Prevent influence from being excessively high.
  if ( mInfluence[tribe] > 200 )
  {
    mInfluence[tribe] = 200;
  }
  
  // std::cout<<"Adding influence.\n";
  
  // for(auto elem : mInfluence)
  // {
     // std::cout << elem.first->name   << " " << elem.second <<".\n";
  // }
}

void World_Local::removeInfluence (Tribe* tribe, int amount)
{
  if ( tribe == 0 ) { return; }
  
  // Note that std::map will automatically initialise new entries.
  mInfluence[tribe] -= amount;
  
  if ( mInfluence[tribe] < 0 ) { mInfluence[tribe] = 0; }
  
  // std::cout<<"Removing influence.\n";
  
  // for(auto elem : mInfluence)
  // {
     // std::cout << elem.first->name   << " " << elem.second <<".\n";
  // }
}

void World_Local::degradeInfluence (int amount /* =1 */)
{
  //std::cout<<"Degrading influence.\n";
  
  for(auto &elem : mInfluence)
  {
    elem.second -= amount;
    if (elem.second < 0) { elem.second = 0; }
  }
}

void World_Local::destroyInfluence (Tribe* tribe)
{
  if ( tribe == 0 ) { return; }

  mInfluence.erase(tribe);
}

Tribe* World_Local::getDominantInfluence ()
{
  if ( mInfluence.empty() || (--mInfluence.end())->second<=0 ) { return 0; }
  // Maps are sorted from smallest to largest, so we simply need to return the last key.
  
  //std::cout<<"Val is :"<<(--mInfluence.end())->second<<".\n";
  //return 0;
  
  return (--mInfluence.end())->first;
}

int World_Local::getDominantInfluenceValue ()
{
  if ( mInfluence.empty() ) { return 0; }
  // Maps are sorted from smallest to largest, so we simply need to return the last key.
  return (--mInfluence.end())->second;
}


std::string World_Local::getTerrainName()
{
  return biomeName[baseBiome];
}

Texture* World_Local::currentTexture()
{
	//enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
	if ( baseBiome == NOTHING )
	{
		return &TEX_WORLD_TEST_00;
	}
	else if ( baseBiome == OCEAN )
	{
		return &TEX_WORLD_TERRAIN_OCEAN_00;
	}
	else if (baseBiome == FOREST)
	{
		return &TEX_WORLD_TERRAIN_FOREST_TREES;
	}
	else if (baseBiome == DESERT)
	{
		return &TEX_WORLD_TERRAIN_DESERT_00;
	}
	
	//else if ( biome == GRASSLAND )
	else
	{
		//std::cout<<"Returning grassyboi\n";
		if ( seed==0 || seed %4 == 0 )
		{
			//std::cout<<"0\n";
			return &TEX_WORLD_TERRAIN_GRASS_00;
		}
		else if ( seed %4 == 1 )
		{
			//std::cout<<"1\n";
			return &TEX_WORLD_TERRAIN_GRASS_01;
		}
		else if ( seed %4 == 2 )
		{
			//std::cout<<"2\n";
			return &TEX_WORLD_TERRAIN_GRASS_02;
		}
		//std::cout<<"3\n";
		return &TEX_WORLD_TERRAIN_GRASS_03;

	}

	return &TEX_WORLD_TEST_00;
}


#endif
