#pragma once
#ifndef WORLDSIM_WORLD_BIOME_CPP
#define WORLDSIM_WORLD_BIOME_CPP

/* WorldSim: World_Biome.cpp
#include "World_Biome.cpp" */

/**

Implementation of World_Biome.hpp

*/

#include "World_Biome.hpp"

#include "Creature_Manager.hpp"


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
		isFloraGenerated=false;

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
	// this seems to not actually be called when entering Worldsim.
	void World_Biome::generate(const unsigned short int sleepTime)
	{
		if ( id == -1 )
		{ return; }
		generateFlora();
		generateCreatures();
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
		
		// if we already generated flora on this biome, abort.
		if (isFloraGenerated) { return; }

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
		// Flora are all statics, therefore there is max 1 flora per tile.
		
		std::cout<<"Generating flora\n";
		floraManager.generate(floraAmount);

		// // assign local ids to the flora (1-255)
		// this loop no longer works because we are storing Flora in FloraManager...
		// for (int i=0;i<vFlora.size();++i)
		// {
			// vFlora(i)->id=i+1;
			// vFlora(i)->spawnWeight = 10;
			// vFlora(i)->setFoodValues(0,0,0);
			// std::cout<<"SET BIOME\n";
			// vFlora(i)->biome = this;
		// }
		
		for (int i=0;i<floraManager.vFlora.size();++i)
		{
			//std::cout<<"Actual flora loop\n";
			floraManager.vFlora(i)->biome = this;
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
		
		isFloraGenerated=true;
	}
	
	void World_Biome::generateCreatures()
	{
		// generate the creatures which exist in this biome
		
		if (size==0) { return; }

		if (type==OCEAN || type == ICE) { return; }
		
		for (int i=0;i<vMap.size();++i)
		{
			vMap(i)->biome = this;
		}

		unsigned int minCreatures = sqrt(size)/1.5;
		if ( minCreatures < 3 ) { minCreatures=3; }

		unsigned int nCreatures = minCreatures+rng.rand32(1+(sqrt(size)/1.5));

		if (nCreatures < 3) { nCreatures=3; }
		if (nCreatures > 255) { nCreatures = 255; }

		creatureManager.generate(nCreatures);

		// assign local ids to the creatures (1-255)

		for (int i=0;i<creatureManager.vSpecies.size();++i)
		{
			creatureManager.vSpecies(i)->biome = this;
			//vCreature(i)->id=i+1;
			//vCreature(i)->spawnWeight = 10;
			//vCreature(i)->setFoodValues(0,0,0);
		}		
		
		// save the flora lookup list (in future we could probably hold the whole list in memory)
		SaveFileManager sfm;
		//std::string mapName = DataTools::toString(vMap(i)->globalX) + "-" + DataTools::toString(vMap(i)->globalY);
		SaveChunk chonk ("BIOME");
		chonk.add("CREATURE LIST GOES HERE");
		//chonk.add(vMap(i)->getSaveData());
		sfm.addChunk(chonk);
		// savefile can be the biomeID + f for Flora
		std::string biomePath = currentSavePath + "/" + DataTools::toString(id) + "c.dat";
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
	
	Vector <Creature_Species*> * World_Biome::getAllCreatureTypes()
	{
		return &creatureManager.vSpecies;
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