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
  #include "WorldObject_Plant.hpp"
  #include "WorldObject_Rock.hpp"
  #include "WorldObject_Sign.hpp"

#include "Creature.hpp"
  #include "Creature_Deer.hpp"



World_Local::World_Local()
{
	globalX=0;
	globalY=0;
  
  initialized = false;
	active = false;
  
	nX=LOCAL_MAP_SIZE;
	nY=LOCAL_MAP_SIZE;


  hasRiver=-1;
  
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

bool World_Local::generate()
{
  if ( active ) { return true; }
  
  if ( world.isSafe(globalX,globalY) == false )
  { return false; }
  if (data==0) { data = new Data; }
  
  if (initialized)
  {
    std::cout<<"World was already previously generated. We should load it from cache.\n";
    load();
  }

  initialized = true;
  active = true;
  

  data->aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  data->aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  //std::cout<<"arrays inited\n";
  
  // if ( initialized )
  // {
    
    // for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
    // {
      // for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
      // { data->aLocalTile(_x,_y).baseTerrain = OCEAN;  
      // }
    // }
    // std::cout<<"Done\n";
    // return true;
  // }
  // std::cout<<"done 2\n";
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
    //aLocalHeight.setBorder(1);
  }
  else
  {
	////HEIGHTMAP TABLE FREESTEPS SMOOTHING
    dsa2.generate(&aLocalHeight,0,0,0.75,100);
  }
	
  
  // Take the seed for this world tile and expand it into a subseed for every local tile */
  random.seed(seed);
  
  int midX = LOCAL_MAP_SIZE/2;



  for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
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
        int baseTreeChance = 30;
        int basePlantChance = 20;
        
        // Temporary hack to make forests look less bad.
        if ( baseBiome == FOREST || baseBiome == JUNGLE )
        {
          data->aLocalTile(_x,_y).baseTerrain = GRASSLAND;
          baseTreeChance/=8;
          basePlantChance/=6;
        }
        
        data->aLocalTile(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
        data->aLocalTile(_x,_y).clearObjects();
        data->aLocalTile(_x,_y).height = aLocalHeight(_x,_y);

        data->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
        data->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
        data->aSubterranean(_x,_y).clearObjects();
        data->aSubterranean(_x,_y).height = -1;
        
        
        if ( hasRiver != -1 && ( (_x ==LOCAL_MAP_SIZE/2) || (_y == LOCAL_MAP_SIZE/2)) )
        {
          //data->aLocalTile(_x,_y).baseTerrain = OCEAN;
        }
        
        //Put down some water for drinking
        else if (Random::oneIn(400))
        {
          data->aLocalTile(_x,_y).baseTerrain = OCEAN;
          continue;
        }
        
        else if (random.oneIn(200)) /* Put down some testing objects */
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
        }
        else if (random.oneIn(baseTreeChance))
        {
          data->aLocalTile(_x,_y).add(new WorldObject_Tree(1));
        }
        else if (random.oneIn(basePlantChance))
        {
          data->aLocalTile(_x,_y).add(new WorldObject_Plant());
        }
        else if (random.oneIn(1000))
        {
          data->aLocalTile(_x,_y).add(new WorldObject_Tree);
        }
        else if (random.oneIn(1500) && (baseBiome == FOREST || baseBiome == GRASSLAND) )
        //else if (random.oneIn(200))
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
            
            data->aLocalTile(_x,_y).add(rockyBoi);
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
  
  SaveChunk chonk ("TILEARRAY");

  // Only unload the local map if it is loaded.
  for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
  {
    for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
    {
      chonk.add(data->aLocalTile(_x,_y).getSaveData());
    }
  }

  sfm.addChunk(chonk);
  sfm.saveToFile(localMapPath);

  return true;
}

bool World_Local::load()
{
  std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
  
  std::cout<<"Attempting to load cached map: "<<localMapPath<<".\n";

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
  

  return true;
}


bool World_Local::put (WorldObject* _object, int _x, int _y)
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
  data->aLocalTile(_x,_y).add(_object);
  vObjectGeneric.push(_object);
  return true;
}

bool World_Local::put (Item* _item, int _x, int _y)
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
  data->aLocalTile(_x,_y).add(_item);
  vItem.push(_item);
  return true;
}
bool World_Local::put (Character* _character, int _x, int _y)
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
  data->aLocalTile(_x,_y).add(_character);
  vCharacter.push(_character);
  
  return true;
}
bool World_Local::put (Creature* _creature, int _x, int _y)
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
  data->aLocalTile(_x,_y).add(_creature);
  vCreature.push(_creature);
  _creature->map=this;
  
  return true;
}
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
  
  if ( data->aLocalTile.isSafe(newX,newY) == false )
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX-nMaps;
          _object->worldY = globalY;
          
          world(globalX-nMaps,globalY)->put(_object,newX,newY);
          
          _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
          _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;

          world(_object->fullX,_object->fullY);

          int gX = 0;
          int gY = 0;
          int lX = 0;
          int lY = 0;
          world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);

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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX+nMaps;
          _object->worldY = globalY;
          world(globalX+nMaps,globalY)->put(_object,newX,newY);
          
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY-nMaps;
          world(globalX,globalY-nMaps)->put(_object,newX,newY);
          
          
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY+nMaps;
          world(globalX,globalY+nMaps)->put(_object,newX,newY);
          
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
  
  //Moving inside of map
  
  if ( data->aLocalTile(newX,newY).hasMovementBlocker() )
  {
    return false;
  }
  
  data->aLocalTile(_object->x,_object->y).remove(_object);
  




  
  _object->x=newX;
  _object->y=newY;
  _object->worldX = globalX;
  _object->worldY = globalY;
  
  data->aLocalTile(newX,newY).add(_object);
  
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
  //Yeah this is a mess.
bool World_Local::moveObject (Character* _object, int newX, int newY )
{
  if ( !data ) { return false; }
  // Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right
  
  if ( data->aLocalTile.isSafe(newX,newY) == false )
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX-nMaps;
          _object->worldY = globalY;
          _object->map = world(globalX-nMaps,globalY);
          
          world(globalX-nMaps,globalY)->put(_object,newX,newY);
          
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX+nMaps;
          _object->worldY = globalY;
          _object->map = world(globalX+nMaps,globalY);
          world(globalX+nMaps,globalY)->put(_object,newX,newY);
          
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY-nMaps;
          _object->map = world(globalX,globalY-nMaps);
          world(globalX,globalY-nMaps)->put(_object,newX,newY);
          
          
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
          data->aLocalTile(_object->x,_object->y).remove(_object);
          _object->x=newX;
          _object->y=newY;
          _object->worldX = globalX;
          _object->worldY = globalY+nMaps;
          _object->map = world(globalX,globalY+nMaps);
          world(globalX,globalY+nMaps)->put(_object,newX,newY);
          
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
  LocalTile* destination = &data->aLocalTile(newX,newY);
  
  if ( destination->hasMovementBlocker() )
  {
    return false;
  }
  
  if (data->aLocalTile.isSafe(_object->x,_object->y)==false)
  {
    return false;
  }
  
  LocalTile* source = &data->aLocalTile(_object->x,_object->y);
  
  //Moving inside of map
  // I'm sure you can do some fancy bitwise stuff here but I can't think of it right now.
  // It'll probably have to be done in future when pathfinding is optimised.
  //char combinedCollision = source->bWall | destination->bWall;
  
  // Check walls
  if (source->bWall!=0 || destination->bWall!=0)
  {
    //return false;
    // West
    if ( newX < _object->x)
    {
      if ( (source->bWall & 0b00010000) == 0b00010000
      ||   (destination->bWall & 0b00000100) == 0b00000100 )
      {
        return false;
      }
    }
    //East
    else if (newX > _object->x)
    {
      if ( (source->bWall & 0b01000000) == 0b01000000
      ||   (destination->bWall & 0b00000001) == 0b00000001 )
      {
        return false;
      }
    }
    //North
    else if (newY > _object->y)
    {
      if ( (source->bWall & 0b10000000) == 0b10000000
      ||   (destination->bWall & 0b00000010) == 0b00000010 )
      {
        return false;
      }
    }
    //South
    else if (newY < _object->y)
    {
      if ( (source->bWall & 0b00100000) == 0b00100000
      ||   (destination->bWall & 0b00001000) == 0b00001000 )
      {
        return false;
      }
    }
  }
  
  data->aLocalTile(_object->x,_object->y).remove(_object);

  _object->x=newX;
  _object->y=newY;
  _object->worldX = globalX;
  _object->worldY = globalY;

  data->aLocalTile(newX,newY).add(_object);
  
  _object->fullX = _object->worldX * LOCAL_MAP_SIZE + _object->x;
  _object->fullY = _object->worldY * LOCAL_MAP_SIZE + _object->y;

  world(_object->fullX,_object->fullY);
  
  int gX = 0;
  int gY = 0;
  int lX = 0;
  int lY = 0;
  world.absoluteToRelative (_object->fullX,_object->fullY,&gX,&gY,&lX,&lY);
  
  return true;
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
  
  if (data->aLocalTile.isSafe(_object->x,_object->y))
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
  
  if (data->aLocalTile.isSafe(_item->x,_item->y))
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
  
  if (data->aLocalTile.isSafe(_character->x,_character->y))
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
  
  if (data->aLocalTile.isSafe(_creature->x,_creature->y))
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
Vector <HasXY*> * World_Local::rayTraceLOS (int _x, int _y, const int RANGE)
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
    rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
  }
  
  
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

void World_Local::rayTrace (int _x1, int _y1, int _x2, int _y2, Vector <HasXY*> * vVisibleTiles)
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
  
  

bool World_Local::isBlockingView(int _x, int _y)
{
  if ( !data ) { return true; }
  return data->aLocalTile(_x,_y).hasViewBlocker();

}

	// Increments the map by nTicks ticks. Higher values may lead to abstraction.
void World_Local::incrementTicks(int nTicks)
{
  if ( !data ) { return; }
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
    vToMove(i)->wander();
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
		if ( seed %4 == 0 )
		{
			//std::cout<<"0\n";
			return &TEX_WORLD_TERRAIN_GRASS_00;
		}
		if ( seed %4 == 1 )
		{
			//std::cout<<"1\n";
			return &TEX_WORLD_TERRAIN_GRASS_01;
		}
		if ( seed %4 == 2 )
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
