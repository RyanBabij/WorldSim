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

World::World(): seaLevel(0), mountainLevel(0)
{
	// aHeightMap.init(1,1,0);
	// aWorldObject.init(1,1,0);
	// WorldTile w;
	// aWorldTile.init(1,1,w);
	// aInfluence.init(1,1,0);
	// aSeed.init(1,1,0);
	
	random.seed();
	
//	seaLevel=0;
//	mountainLevel=0;
	
	//date.setDaysPerMonth(30);
	//date.setMonthsPerYear(12);
	//date.setUnitsPerDay(10000);
	
	//lastDate.setDaysPerMonth(30);
//	lastDate.setMonthsPerYear(12);
//	lastDate.setUnitsPerDay(10000);
	nX=-1;
	nY=-1;
 
	active=false;
	generated=false;
	
	ticksBacklog=0;
	relinquishTimer.init();
	
	name="";
}

void World::nameRegions()
{
}

bool World::putObject(WorldObjectGlobal* _object, int x /* =-1 */, int y /* =-1 */)
{
	//return false;
	if ( x != -1 && y != -1 )
	{
		//x=_object->worldX;
		//y=_object->worldY;
		_object->worldX=x;
		_object->worldY=y;
	}
	else
	{
		//_object->worldX=x;
		//_object->worldY=y;
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

void World::dump(std::string fileName)
{
	// const std::string DUMP_PATH = "log/dump.txt";

	// std::cout<<"World::dump()\n";
	// FileLog::clear(DUMP_PATH);
	// RyanTime rt;
	// rt.update();
	// std::string ryanTime = rt.toString("-");
	// FileLog::log("Commencing full dump.\n",DUMP_PATH);
	// FileLog::log("Real time: "+ryanTime+"\n",DUMP_PATH);
	// FileLog::log("Game time: XXX\n",DUMP_PATH);
	
	// std::string _civData = "# civs: ";
	// _civData+=DataTools::toString(vCiv.size());
	// _civData+="\n\n";
	// FileLog::log(_civData,DUMP_PATH);
	
	// for ( int i=0;i<vCiv.size();++i)
	// {
		// Civ* currentCiv = vCiv(i);
		// std::string _civData2 = "Civ " + DataTools::toString(i)+": "+currentCiv->name+".\n";
		// FileLog::log(_civData2,DUMP_PATH);
		
		// for ( int iCity=0;iCity<currentCiv->vCity.size();++iCity)
		// {
			// City* currentCity = currentCiv->vCity(iCity);
			// std::string _cityData = "\tCity: "+DataTools::toString(iCity)+": "+currentCity->name+".\n";
			// FileLog::log(_cityData,DUMP_PATH);
		// }
		// FileLog::log("\n",DUMP_PATH);
		
	// }
	
}

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
	
	
	//std::cout<<"Incrementing the world by "<<nTicks<<" ticks.\n";
	//std::cout<<"Date: "<<calendar.toString()<<".\n";
	calendar.advanceSecond(nTicks);
	
	//for ( int i=0;i<vCiv.size();++i)
	//{
		//vCiv(i)->incrementTicks(nTicks);
	//}
	
	for ( int i=0;i<vTribe.size();++i)
	{
		vTribe(i)->incrementTicks(nTicks);
	}
  
	for ( int i=0;i<vCiv.size();++i)
	{
		vCiv(i)->incrementTicks(nTicks);
	}
	
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

bool World::loadWorld(std::string filePath)
{

	std::cout<<"Loading world png.\n";
	
	int fileSize;
	unsigned char* data = FileManager::getFile(filePath,&fileSize);
	Png png;
	png.load(data,fileSize);
	//aHeightMap.init(png.nX,png.nY,0);
	aWorldObject.init(png.nX,png.nY,0);
	aTopoMap.init(png.nX,png.nY,3,0);
	aInfluence.init(png.nX,png.nY,0);


	//std::cout<<"PNG size: "<<png.nX<<","<<png.nY<<".\n";

	for(int x=0;x<png.nX;++x)
	{
		//std::cout<<"Loading in x: "<<x<<".\n";
		for(int y=0;y<png.nY;++y)
		{ /* for some reason, the map needs to loaded upside-down */
			//aHeightMap(x,png.nY-y-1)=png.getPixel3D(x,y,0);
			
			aTopoMap(x,png.nY-y-1,0)=png.getPixel3D(x,y,0);
			aTopoMap(x,png.nY-y-1,1)=png.getPixel3D(x,y,1);
			aTopoMap(x,png.nY-y-1,2)=png.getPixel3D(x,y,2);
		}
	}
	delete [] data;
	delete [] png.data;

	return false;
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

bool World::loadWorldData(std::string filePath)
{
	std::cout<<"Getting world data.\n";
	int fileSize;
	unsigned char* data = FileManager::getFile(filePath,&fileSize);
	seaLevel = data[0];
	mountainLevel = data[1];
	std::cout<<"Data read: " << (int)seaLevel << ","<<(int)mountainLevel<<"\n";
	return true;
}

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
			
			aSeed(_x,_y) = random.randInt(INT_MAX-1);
			aWorldTile(_x,_y).biome = aTerrain(_x,_y);
			aWorldTile(_x,_y).seed = aSeed(_x,_y);
      
        //Initialise the WorldTile with biome enum.
      aWorldTile(_x,_y).init(aTerrain(_x,_y));
			
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
				// _red=255;
				// _green=0;
				// _blue=0;
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
	png.encodeS3("world.png",&aTopoMap);
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
		aInfluence.init(x,y,0);
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
		
		std::thread t5 (&ArrayS2 <std::map<Tribe*,int> *>::init, &this->aInfluence, x,y,influenceNull);
		std::thread t6 (&ArrayS2 <int>::init, &this->aSeed, x,y,0);
		std::thread t7 (&ArrayS2 <int>::init, &this->aLandmassID, x,y,-1);
		
		
		std::thread t8 (&ArrayS2 <bool>::init, &this->aIsLand, x,y,true);
		std::thread t9 (&ArrayS2 <int>::init, &this->aBiomeID, x,y,-1);
	#endif
	
	aWorldTile.initClass(x,y);

	
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
	FileManager::createFile(strSavePath+"/master.dat");
	
	#ifdef THREADED
	//	t1.join();   // thread waits for the thread t to finish
		t2.join();
		t3.join();
		t4.join();
		t5.join();
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
	
	
	worldGenTimer.update();
	
	std::cout<<"world generated in: "<<worldGenTimer.fullSeconds<<" seconds.\n";
}

void World::generateLocal(const int _x, const int _y)
{
}

int World::getTileFertility(const int _x, const int _y)
{
	int fertility = 0;
	if ( aTerrain.isSafe(_x,_y) )
	{
		int terrainType = aTerrain(_x,_y);
		
		if ( terrainType == OCEAN )
		{
			fertility = 2;
		}
		else if (terrainType == GRASSLAND )
		{
			fertility = 20;
		}
		else if (terrainType == FOREST )
		{
			fertility = 20;
		}
		else if (terrainType == DESERT )
		{
			fertility = 10;
		}
		else if (terrainType == JUNGLE )
		{
			fertility = 20;
		}
		else if (terrainType == SNOW )
		{
			fertility = 10;
		}
		else if (terrainType == HILLY )
		{
			fertility = 15;
		}
		else if (terrainType == WETLAND )
		{
			fertility = 25;
		}
		else if (terrainType == STEPPES )
		{
			fertility = 10;
		}
		else if (terrainType == ICE )
		{
			fertility = 0;
		}
		else if (terrainType == RIVER )
		{
			fertility = 30;
		}
	}

	return fertility;
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

HasXY* World::getRandomTileOfType(int _type)
{
	int randomRow = random.randomInt(nY-1);
	int nRowsSearched = 0;
	
	while ( nRowsSearched < nY )
	{
		for (int _x=0;_x<nX;++_x)
		{
			int randomX = (*vAllTiles(randomRow))(_x);
			
			if ( aTerrain(randomX,randomRow) == _type )
			{
				return new HasXY (randomX,randomRow);
			}

		}
		
		vAllTiles(randomRow)->shuffle();
		
		++randomRow;
		if (randomRow >= nY) { randomRow=0; }
		++nRowsSearched;
	}
	
	


	return 0;
}


void World::queryTile( int hoveredXTile, int hoveredYTile)
{
	//std::cout<<"Nx: "<<nX<<".\n";
	if ( hoveredXTile < 0 || hoveredYTile < 0 || hoveredXTile >= nX || hoveredYTile >= nY )
	{
		//std::cout<<"Invalid\n";
		return;
	}

  std::cout<<"\n*** QUERY WORLD INFO ("<<hoveredXTile<<", "<<hoveredYTile<<") ***\n";
	int terrain = aTerrain(hoveredXTile,hoveredYTile);
	std::cout<<"Terrain value: "<<terrain<<" ("<<biomeName[terrain]<<").\n";
  
  std::cout<<"Metal: "<<aWorldTile(hoveredXTile,hoveredYTile).baseMetal<<".\n";
	
	
	for (int i=0;i<vTribe.size();++i)
	{
		if ( vTribe(i)->worldX == hoveredXTile && vTribe(i)->worldY == hoveredYTile)
		{
			//std::cout<<"Tribe: "<<vTribe(i)->name<<".\n";
		}
	}
	
	for (int i=0;i<vWorldObjectGlobal.size();++i)
	{
		if ( vWorldObjectGlobal(i)->worldX == hoveredXTile && vWorldObjectGlobal(i)->worldY == hoveredYTile)
		{
			std::cout<<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name<<".\n";
      Console (Stream() <<vWorldObjectGlobal(i)->nameType<<": "<<vWorldObjectGlobal(i)->name);
		}
	}
  Console (Stream() <<"Objects:");
  
	std::cout<<"Landmass name: "<<getLandmassName(hoveredXTile,hoveredYTile)<<".\n";
	
	std::cout<<"Local seed: "<<aSeed(hoveredXTile,hoveredYTile)<<".\n";
  
  std::cout<<"************\n";
  
  Console (Stream() <<"Landmass name: "<<getLandmassName(hoveredXTile,hoveredYTile));
  Console(Stream() <<"Metal: "<<aWorldTile(hoveredXTile,hoveredYTile).baseMetal);
  Console (Stream() <<"Terrain: "<<biomeName[terrain]);
  Console (Stream() <<"("<<hoveredXTile<<", "<<hoveredYTile<<")");
}

void World::addInfluence(Tribe* tribe, int amount)
{
	if ( tribe==0 )
	{
		return;
	}
	int tribeX = tribe->worldX;
	int tribeY = tribe->worldY;
	
	if ( aInfluence.isSafe(tribeX,tribeY))
	{
		// Create a map for this tile if there isn't one already.
		if ( aInfluence(tribeX,tribeY) == 0 )
		{
			aInfluence(tribeX, tribeY) = new std::map <Tribe*, int>;
		}

		// Search for existing influence from this tribe.
		auto search = aInfluence(tribeX,tribeY)->find(tribe);
		if(search != aInfluence(tribeX,tribeY)->end())
		{
			// Increment influence if this tribe has been here.
			search->second+=amount;
			
				// Prevent excessive influence on a tile.
			if (search->second > 500)
			{
				search->second = 500;
			}
		}
		else
		{
			// Create new map entry if this tribe has never been here before.
			aInfluence(tribeX,tribeY)->insert(std::make_pair(tribe, amount));
		}
		

	}
	//std::cout<<"Addinfluence4\n";
}

void World::degradeInfluence(Tribe* tribe)
{
	//std::cout<<"Degrading influence.\n";
	if ( tribe==0 )
	{
		return;
	}
	
	for (std::map <Tribe*,int>* mInfluence : aInfluence )
	{
		//std::cout<<"Address is: "<<mInfluence<<".\n";
		if ( mInfluence != 0)
		{
			// Search for existing influence from this tribe.
			auto search = mInfluence->find(tribe);
			if(search != mInfluence->end())
			{
				//std::cout<<"Influence is now: "<<search->second<<".\n";
				// Decrement influence if this tribe has been here.
				if(search->second > 0)
				{ search->second--;
				}
				
				//std::cout<<"Influence is now: "<<search->second<<".\n";
				
			}
		}
	}
	//std::cout<<"\nEND\n";
	
	// int tribeX = tribe->worldX;
	// int tribeY = tribe->worldY;
	
	// if ( aInfluence.isSafe(tribeX,tribeY))
	// {
		// // Create a map for this tile if there isn't one already.
		// if ( aInfluence(tribeX,tribeY) == 0 )
		// {
			// aInfluence(tribeX, tribeY) = new std::map <Tribe*, int>;
		// }

		// // Search for existing influence from this tribe.
		// auto search = aInfluence(tribeX,tribeY)->find(tribe);
		// if(search != aInfluence(tribeX,tribeY)->end())
		// {
			// // Increment influence if this tribe has been here.
			// search->second++;
		// }
		// else
		// {
			// // Create new map entry if this tribe has never been here before.
			// aInfluence(tribeX,tribeY)->insert(std::make_pair(tribe, 1));
		// }
		

	// }
	// std::cout<<"Addinfluence4\n";
}

	// Note that 0 influence doesn't count as influence. In this case it will return 0.
Tribe* World::getDominantInfluence (const int _x, const int _y)
{
	if ( aInfluence.isSafe(_x,_y) && aInfluence(_x,_y) != 0)
	{
		std::map<Tribe* , int>::iterator it;
		
		int largestInfluence = 0;
		Tribe * dominantTribe = 0;
		
		for ( it = aInfluence(_x,_y)->begin(); it != aInfluence(_x,_y)->end(); it++ )
		{
			if ( it->second > largestInfluence )
			{
				dominantTribe = it->first;
				largestInfluence = it->second;
			}
		}
		return dominantTribe;
	}
	return 0;
}

Tribe* World::getDominantInfluence (HasXY* _xy)
{
	return getDominantInfluence(_xy->x,_xy->y);
}
	
	
int World::getHighestInfluence(const int _x, const int _y)
{
	if ( aInfluence.isSafe(_x,_y) && aInfluence(_x,_y) != 0)
	{
		
		std::map<Tribe* , int>::iterator it;
		
		int largestInfluence = 0;
		Tribe * dominantTribe = 0;
		
		for ( it = aInfluence(_x,_y)->begin(); it != aInfluence(_x,_y)->end(); it++ )
		{
			if ( dominantTribe == 0 || it->second > largestInfluence )
			{
				dominantTribe = it->first;
				largestInfluence = it->second;
			}
		}
		return largestInfluence;
	}
	return 0;
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
	
	
#endif
