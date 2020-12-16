#pragma once
#ifndef WORLDSIM_WORLD_BIOME_CPP
#define WORLDSIM_WORLD_BIOME_CPP

/* WorldSim: World_Biome.cpp
#include "World_Biome.cpp" */

/**

Implementation of WorlD_Biome.hpp

*/

#include "World_Biome.hpp"


#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>
#include <System/Sleep/Sleep.hpp>

	World_Biome::World_Biome()
	{
		name="";
		size=0;
		rng.seed(globalRandom.rand32());

		isGenerated=false;
		threadAccess=false;

		id=-1;
		
		averageX=-1;
		averageY=-1;
		
		centerX=-1;
		centerY=-1;
	}

	World_Biome::~World_Biome()
	{
	}

	// generate entire biome including terrain, flora, etc.
	void World_Biome::generate(const unsigned short int sleepTime)
	{
		if ( id == -1 )
		{ return; }
		generateFlora();
		generateLocals(sleepTime);
		isGenerated=true;
	}

	// generate all local maps for this biome
	// map generation will be done per-biome rather than per tile
	void World_Biome::generateLocals(const unsigned short int sleepTime)
	{
		if ( id == -1 )
		{ return; }

		SaveFileManager sfm;

		for (int i=0;i<vMap.size();++i)
		{
			sleep (sleepTime);
			if ( QUIT_FLAG )
			{ return; }
		
			short int gX = vMap(i)->globalX;
			short int gY = vMap(i)->globalY;
		
			std::cout<<"global coords: "<<gX<<", "<<gY<<"\n";
			World_Local* wl0 = 0;
			if (world.aWorldTile.isSafe(gX-1,gY+1))
			{
				wl0 = &world.aWorldTile(gX-1,gY+1);
			}
			World_Local* wl1 = 0;
			if (world.aWorldTile.isSafe(gX,gY+1))
			{
				wl1 = &world.aWorldTile(gX,gY+1);
			}
			World_Local* wl2 = 0;
			if (world.aWorldTile.isSafe(gX+1,gY+1))
			{
				wl2 = &world.aWorldTile(gX+1,gY+1);
			}
			
			World_Local* wl3 = 0;
			if (world.aWorldTile.isSafe(gX-1,gY))
			{
				wl3 = &world.aWorldTile(gX-1,gY);
			}
			World_Local* wl4 = 0;
			if (world.aWorldTile.isSafe(gX+1,gY))
			{
				wl4 = &world.aWorldTile(gX+1,gY);
			}

			World_Local* wl5 = 0;
			if (world.aWorldTile.isSafe(gX-1,gY-1))
			{
				wl5 = &world.aWorldTile(gX-1,gY-1);
			}
			World_Local* wl6 = 0;
			if (world.aWorldTile.isSafe(gX,gY-1))
			{
				wl6 = &world.aWorldTile(gX,gY-1);
			}
			World_Local* wl7 = 0;
			if (world.aWorldTile.isSafe(gX+1,gY-1))
			{
				wl7 = &world.aWorldTile(gX+1,gY-1);
			}


			vMap(i)->biome=this;
			vMap(i)->generate(true,wl0,wl1,wl2,wl3,wl4,wl5,wl6,wl7);
			vMap(i)->active=true;
			vMap(i)->initialized=true;

			std::string mapName = DataTools::toString(vMap(i)->globalX) + "-" + DataTools::toString(vMap(i)->globalY);
			SaveChunk chonk (mapName);
			chonk.add(vMap(i)->getSaveData());
			sfm.addChunk(chonk);

			vMap(i)->unload();
		}

		// savefile can be the biomeID
		std::string biomePath = currentSavePath + "/" + DataTools::toString(id) + ".dat";
		//std::cout<<"saving to: "<<biomePath<<"\n";
		sfm.saveToFile(biomePath);
	}

	void World_Biome::generateFlora()
	{
		//each biome should have at least 3 floras: 1 common, 1 medium, 1 rare.
		// Each biome should have a base flora which will normally be trees.
		// Larger biomes should have more. Maximum amount of flora types in a biome is 16 (4 bits).
		// In future flora should have non-random spawning rules, for example some flora will only
		// spawn near trees.
		// Flora will be given unique names for flavour.

		// flora diversity will be set based on biome size, however there must be a minimum of 1 common and 1 uncommon flora.

		// additional flora is up to sqrt(size)


		// it would be better if we first determine the number of valid spots to place flora, before generating flora types.
		// that way diversity can be tied to the flora capacity of the biome

		if (size==0) { return; }

		if (type==OCEAN || type == ICE) { return; }
		
		for (int i=0;i<vMap.size();++i)
		{
			vMap(i)->biome = this;
		}

		unsigned int minFlora = sqrt(size)/1.5;
		if ( minFlora < 3 ) { minFlora=3; }

		unsigned int floraAmount = minFlora+rng.rand32(1+(sqrt(size)/1.5));

		if (floraAmount < 3) { floraAmount=3; }
		if (floraAmount > 255) { floraAmount = 255; }
		
		// Each biome should have up to 3 hardcoded flora. For example deserts will always have Cactuses.
		// only additional flora will be randomly generated.
		
		// Flora are all staticsm, therefore there is max 1 flora per tile.
		


		//std::cout<<"Biome size: "<<size<<" generating "<<floraAmount<<" flora.\n";

		// generate rarities. A 3-way split would probably be fine.
		// ID 0 is always the primary flora for the biome, typically a tree
		// ID 1 is always some kind of plant
		// and ID 2 is always some kind of rare plant.
		// The rest should be random.

		// assign base flora for each biome
		// if (type == DESERT)
		// {
			// Flora* flora = new Flora("Cactus",100);
			// flora->setFoodValues(0,0,0);
			// vFlora.push(flora);

			// Flora* flora2 = new Flora("Shrub",25);
			// flora2->setFoodValues(0,0,0);
			// vFlora.push(flora2);

			// Flora* flora3 = new Flora("Desert plant",10);
			// flora3->setFoodValues(0,0,5);
			// vFlora.push(flora3);
		// }
		// else if (type == SNOW)
		// {
			// Flora* flora = new Flora("Fir tree",100);
			// flora->setFoodValues(0,0,0);
			// vFlora.push(flora);

			// Flora* flora2 = new Flora("Bush",25);
			// flora2->setFoodValues(0,0,5);
			// vFlora.push(flora2);

			// Flora* flora3 = new Flora("Hardy plant",10);
			// flora3->setFoodValues(0,0,5);
			// vFlora.push(flora3);
		// }
		// else if (type == WETLAND)
		// {
			// Flora* flora = new Flora("Willow tree",100);
			// flora->setFoodValues(0,0,0);
			// vFlora.push(flora);

			// Flora* flora2 = new Flora("Swamp plant",25);
			// flora2->setFoodValues(0,0,0);
			// vFlora.push(flora2);

			// Flora* flora3 = new Flora("Swamp plant",10);
			// flora3->setFoodValues(0,0,5);
			// vFlora.push(flora3);
		// }
		// else
		// {
			// Flora* flora = new Flora("Tree",100);
			// flora->setFoodValues(0,0,0);
			// vFlora.push(flora);

			// Flora* flora2 = new Flora("Common plant",25);
			// flora2->setFoodValues(0,0,5);
			// vFlora.push(flora2);

			// Flora* flora3 = new Flora("Uncommon plant",10);
			// flora3->setFoodValues(0,0,5);
			// vFlora.push(flora3);
		// }

		// for (unsigned int i=3;i<floraAmount;++i)
		// {
			// std::string floraName = "";

			// int common = rng.rand8()%3;

			// if (common==0)
			// {
				// Flora* flora2 = new Flora("Common flora",20);
				// flora2->setFoodValues(0,0,5,1,5,1);
				// vFlora.push(flora2);
			// }
			// else if (common==1)
			// {
				// Flora* flora2 = new Flora("Uncommon flora",10);
				// flora2->setFoodValues(0,0,5,1,5,1);
				// vFlora.push(flora2);
			// }
			// else
			// {
				// Flora* flora2 = new Flora("Rare flora",5);
				// flora2->setFoodValues(0,0,5,1,5,1);
				// vFlora.push(flora2);
			// }
		// }
		
		// Generate the random flora.
		
		//Vector <Flora*>* vFlora2 = floraGenerator.generate(floraAmount);
		floraManager.generate(floraAmount);

		// std::cout<<"Flora generated for biome:\n";
		// for (int i=0;i<vFlora2->size();++i)
		// {
			// std::cout<<" * "<<(*vFlora2)(i)->getName()<<"\n";
			// vFlora.push( (*vFlora2)(i) );
			
		// }
		//delete vFlora2;

		// assign local ids to the flora (1-255)

		for (int i=0;i<vFlora.size();++i)
		{
			vFlora(i)->id=i+1;
			vFlora(i)->spawnWeight = 10;
			vFlora(i)->setFoodValues(0,0,0);
		}		
		
		// save the flora lookup list (in future we could probably hold the whole list in memory)
		SaveFileManager sfm;
		//std::string mapName = DataTools::toString(vMap(i)->globalX) + "-" + DataTools::toString(vMap(i)->globalY);
		SaveChunk chonk ("BIOME");
		chonk.add("FLORA LIST GOES HERE");
		//chonk.add(vMap(i)->getSaveData());
		sfm.addChunk(chonk);
		// savefile can be the biomeID + f for Flora
		std::string biomePath = currentSavePath + "/" + DataTools::toString(id) + "f.dat";
		//std::cout<<"Saving Flora lookup to: "<<biomePath<<"\n";
		sfm.saveToFile(biomePath);
	}

	// pick a flora type from the weighted list to spawn
	Flora* World_Biome::getFlora()
	{
		return floraManager.get();
		
		
		unsigned long int cumulativeProb = 0;

		for (int i=0;i<vFlora.size();++i)
		{
			cumulativeProb += vFlora(i)->spawnWeight;
		}

		if ( cumulativeProb == 0 )
		{
			return 0;
		}

		unsigned long int spawnChoice = rng.rand32(cumulativeProb-1);

		cumulativeProb=0;
		for (int i=0;i<vFlora.size();++i)
		{
			cumulativeProb += vFlora(i)->spawnWeight;
			if (spawnChoice <= cumulativeProb)
			{
				return vFlora(i);
			}
		}

		return 0;
	}
	
	Vector <Flora*> * World_Biome::getAllFloraTypes()
	{
		return &floraManager.vFlora;
		//return 0;
	}
	
		// Uses a safe rolling average to prevent overflow.
	void World_Biome::getAverageCoordinates(/*Vector <HasXY*> * vXY*/)
	{
		int n = 0;
		int n2 = 0;
		averageX = 0;
		averageY = 0;
		for ( int i=0;i<vXY.size();++i)
		{
			double deltaX = vXY(i).x - averageX;
			averageX += deltaX/++n;
			
			double deltaY = vXY(i).y - averageY;
			averageY += deltaY/++n2;
		}
		
		// now find the tile closest to these average points.
		double closestDistance = -1;
		for ( int i=0;i<vXY.size();++i)
		{
			double deltaX = abs(vXY(i).x - averageX);
			double deltaY = abs(vXY(i).y - averageY);
			
			if ( closestDistance == -1 || closestDistance > deltaX+deltaY)
			{
				centerX = vXY(i).x;
				centerY = vXY(i).y;
				closestDistance = deltaX+deltaY;
			}
		}
	}
	
	void World_Biome::addMap(World_Local* map)
	{
		map->biome=this;
		vMap.push(map);
	}
	
	
	// TableInterface
	std::string World_Biome::getColumn(std::string _column)
	{
		if ( _column=="name" )
		{
			return name;
		}
		else if ( _column=="size" )
		{
			return DataTools::toString(size);
		}
		else if ( _column=="type" )
		{
			return WorldGenerator2::biomeName[type];
		}
		return "?";
	}
	// TableInterface
	std::string World_Biome::getColumnType(std::string _column)
	{
		if ( _column == "size" )
		{
			return "number";
		}
		return "string";
	}

#endif