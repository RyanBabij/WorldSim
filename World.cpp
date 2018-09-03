#pragma once
#ifndef GUILD_WORLD_CPP
#define GUILD_WORLD_CPP

/* World.cpp
	#include "World.cpp"
	Implementation of "World.hpp".
*/

#include <NameGen/NameGen.hpp>
#include <WorldGenerator/WorldGenerator2.hpp>

#include <File/FileLog.hpp>

#include <Time/Timer.hpp> // To manage work chunks and benchmark worldgen time.

#include <thread>

#include "Civ.cpp"
#include "Civ_Dwarven.cpp"

#include "Tribe.cpp"
#include "Tribe_Human.cpp"
#include "Tribe_Dwarven.cpp"
#include "Tribe_Elf.cpp"

#include "Settlement.cpp"
#include "Settlement_Dwarven.cpp"

#include "WorldObjectGlobal.hpp"
#include <WorldGenerator/Biome.hpp>

World::World(): SaveFileInterface(), seaLevel(0), mountainLevel(0)
{
	random.seed();
	
	nX=-1;
	nY=-1;
 
	active=false;
	generated=false;
	
	ticksBacklog=0;
	relinquishTimer.init();
	
	name="";
  
	dailyCounter=0;
	monthlyCounter=0;
  
  localX = -1;
  localY = -1;
  
  queryWorldX = -1;
  queryWorldY = -1;
  
  worldFilePath = "";
  
  playerCharacter = 0;
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


	for (int i=0;i<nTribesHuman;++i)
	{
		Tribe_Human * t = new Tribe_Human;
		t->init(this);
		if (t->spawn() == false)
		{
			delete t;
			continue;
		}

		t->generateCouples(7);
	}
	
	for (int i=0;i<nTribesDwarven;++i)
	{
		Tribe_Dwarven * t = new Tribe_Dwarven;
		t->init(this);
		t->spawn();
		t->generateCouples(7);
	}
  
	for (int i=0;i<nTribesElven;++i)
	{
		Tribe_Elf * t = new Tribe_Elf;
		t->init(this);
		t->spawn();
		t->generateCouples(7);
	}
	
	
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
	}
  
	while ( dailyCounter >= 86400 )
  {

		dailyCounter-=86400;
	}
	
	//std::cout<<"Incrementing the world by "<<nTicks<<" ticks.\n";
	//std::cout<<"Date: "<<calendar.toString()<<".\n";

	
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
      aWorldTile(_x,_y).init(aTerrain(_x,_y), aSeed(_x,_y));
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
	
	// lock for entire scope
	//std::lock_guard<std::mutex> guard(render_mutex);
	

	
	/* WIPE OLD WORLD. */
	
	//Timer threadTimer;
	//threadTimer.init();
	//threadTimer.start();
	
	
	// Multithreading this segment runs 23% faster. Saves about 0.1 seconds or something.

  #define THREADED
//#undef THREADED
	
	#ifndef THREADED
		//aHeightMap.init(x,y,0);
		aWorldObject.init(x,y,0);
		aTopoMap.init(x,y,3,0);
		aTerrain.init(x,y,NOTHING);
		//aInfluence.init(x,y,0);
		aSeed.init(x,y,0);
		
		aLandmassID.init(x,y,-1);
		aBiomeID.init(x,y,-1);
		aIsLand.init(x,y,true);
	#else
//		std::thread t1 (&ArrayS2 <unsigned char>::init, &this->aHeightMap, x,y,0);
		WorldObject* const woNull = 0;
		std::thread t2 (&ArrayS2 <WorldObject*>::init, &this->aWorldObject, x,y,woNull);
		std::thread t3 (ArrayS2 <enumBiome>::init, &this->aTerrain, x,y,NOTHING);
		std::thread t4 (&ArrayS3 <unsigned char>::init, &this->aTopoMap, x,y,3,0);
		
		std::map<Tribe*,int> * const influenceNull = 0;
		
		//std::thread t5 (&ArrayS2 <std::map<Tribe*,int> *>::init, &this->aInfluence, x,y,influenceNull);
		std::thread t6 (&ArrayS2 <int>::init, &this->aSeed, x,y,0);
		std::thread t7 (&ArrayS2 <int>::init, &this->aLandmassID, x,y,-1);
		
		
		std::thread t8 (&ArrayS2 <bool>::init, &this->aIsLand, x,y,true);
		std::thread t9 (&ArrayS2 <int>::init, &this->aBiomeID, x,y,-1);
	#endif
	
	aWorldTile.initClass(x,y);
//	aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
//	aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  
	dailyCounter=0;
	monthlyCounter=0;

	
	//std::cout<<"World seed is: "<<seed<<".\n";

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
	

	for ( int i=0;i<vWorldObjectGlobal.size();++i)
	{
		delete vWorldObjectGlobal(i);
	}	
	vWorldObjectGlobal.clear();
	
	for ( int i=0;i<vLandmass.size();++i)
	{
		delete vLandmass(i);
	}	
	vLandmass.clear();
	
	for ( int i=0;i<vBiome.size();++i)
	{
		delete vBiome(i);
	}	
	vBiome.clear();
	
	// all tribe objects are already deleted through vWorldObjectGlobal.
	vTribe.clear();
	
	// for (int _y=0; _y<nY; ++_y)
	// {
		// for ( int _x=0;_x<nX;++_x)
		// {
			// std::string strTileSavePath = strSavePath+"/"+DataTools::toString(_x)+".dat";
			// //FileManager::createFile(strSavePath+"/"+DataTools::toString(_x)+"x"+DataTools::toString(_y));
			// //std::cout<<"Writing file: "<<strTileSavePath<<".\n";
			// FileManager::createFile(strSavePath+"/"+DataTools::toString(_x)+"x"+DataTools::toString(_y)+".dat");
		// }
	// }
	
	// Create master file.
  worldFilePath = strSavePath+"/main.dat";
  

  
	FileManager::createFile(worldFilePath);
  FileManager::writeTag("LANDSEED",DataTools::toString(landmassSeed),worldFilePath);
  FileManager::writeTag("WORLDNAME",name,worldFilePath);
  FileManager::writeTag("SIZEX",DataTools::toString(nX),worldFilePath);
  FileManager::writeTag("SIZEY",DataTools::toString(nY),worldFilePath);
  //FileManager::writeString(DataTools::toString(landmassSeed)+"\n"+name+"\n"+DataTools::toString(nX)+"\n"+DataTools::toString(nY)+"\n",worldFilePath);
	
  
	#ifdef THREADED
	//	t1.join();   // thread waits for the thread t to finish
		t2.join();
		t3.join();
		t4.join();
		//t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
	#endif


	wg.generate();
	
		// We could remove this later by using a pointer.
	aTerrain = wg.aTerrainType;
	
	buildArrays();
	
	
	// BUILD LANDMASS ID ARRAY
	int currentID = 0;

	std::cout<<"Building landmass info.\n";
		
	for ( int _y=0;_y<nY;++_y)
	{
		//break;
		for ( int _x=0;_x<nX;++_x)
		{
			if (aLandmassID(_x,_y) == -1 && aIsLand(_x,_y) == true )
			{
				Vector <HasXY*>* vFill = aIsLand.floodFillVector(_x,_y,false);
				
				for (int i=0;i<vFill->size();++i)
				{
					HasXY* v = (*vFill)(i);
					aLandmassID(v->x,v->y) = currentID;

				}
				
				World_Landmass* landmass = new World_Landmass;
				landmass->name = globalNameGen.generateName();
				landmass->size = vFill->size();
				landmass->getAverageCoordinates(vFill);
				vLandmass.push(landmass);
				
				vFill->clear();
				delete vFill;
				
				++currentID;
			}
			
		}
	}
	std::cout<<"Filled "<<currentID<<" landmasses.\n";
	
	// Trying another algorithm
	
	// std::cout<<"Building landmass info with second algorithm.\n";
	
	// int nLandmass2 = 0;
	
	// ArrayS2 <int>* aLandmassID2 = aIsLand.floodFillUniqueID(false, &nLandmass2);
	
	// std::cout<<"Filled "<<nLandmass2<<" landmasses.\n";
	
	std::cout<<"Building biome info.\n";
	
	currentID=0;
		
		// BUILD BIOME VECTOR
	for ( int _y=0;_y<nY;++_y)
	{
		//break;
		for ( int _x=0;_x<nX;++_x)
		{
			if (aBiomeID(_x,_y) == -1 )
			{
				Vector <HasXY*>* vFill = aTerrain.floodFillVector(_x,_y,true);
				int biomeType = aTerrain(_x,_y);

				
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
				
				vFill->clear();
				delete vFill;
				
				++currentID;
			}
			
		}
	}
	std::cout<<"Filled "<<currentID<<" landmasses.\n";
		
	
	generated = true;
	
	
	// Build and shuffle the tile vector
	//Vector < Vector <HasXY> > vAllTiles;
	
	for (int i=0;i<vAllTiles.size();++i)
	{
		delete vAllTiles(i);
	}
	vAllTiles.clear();
	
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
	
	
	//Vector < Vector <HasXY* >* > vAllTiles;
  
  
  // SAVE WORLD DATA HERE (LATER PUT INTO FUNCTION)
  std::string tileData = "";
	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
      tileData+=",";
      tileData+=DataTools::toString(aWorldTile(_x,_y).biome);
    }
  }
  FileManager::writeTag("BIOME",tileData,worldFilePath);
	
	
	worldGenTimer.update();
	
	std::cout<<"world generated in: "<<worldGenTimer.fullSeconds<<" seconds.\n";
  
  std::cout<<"The world's uid is: "<<getUID()<<".\n";

  
  //std::cout<<"Doing test save.\n";
  
  // worldFilePath = strSavePath+"/main2.dat";
  // saveFileManager.vSaveObjects.clear();
  // saveFileManager.vSaveObjects.push(this);
  // saveFileManager.saveFile(worldFilePath);
}


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
  
  if ( vWorldLocal.size() > 3 )
  {
      // UNLOAD LOCAL MAP HERE
    delete vWorldLocal(0);
    vWorldLocal.eraseSlot(0);
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
	vTerrain->clearData();
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
			std::cout<<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name<<".\n";
      Console (Stream() <<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name);
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
  int genX = playerCharacter->tribe->worldX;
  int genY = playerCharacter->tribe->worldY;
  
  //worldViewer.localX = genX;
  //worldViewer.localY = genY;
  
  worldViewer.centerTileX = genX;
  worldViewer.centerTileY = genY;
  
  /* Generate local map */
  generateLocal(genX,genY);
  
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
