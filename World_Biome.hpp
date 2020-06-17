#pragma once
#ifndef WORLDSIM_WORLD_BIOME_HPP
#define WORLDSIM_WORLD_BIOME_HPP

/* WorldSim: World_Biome.hpp
#include "World_Biome.hpp" */

/**

Biomes are essentially collections of unique environments which also include unique flora and fauna. In this respect
biomes are a kind of container for these objects.

Much processing and simulation is handled on the biome level as these are the smallest discrete "chunks" of world which
may be independently simulated. This might cause complications with overly-large biomes, so I am considering solutions
for that, including splitting the biomes up into sub-biomes.

Local Map, Creature and Flora generation is all handled per-biome. Tribes and Civilizations are not.

Biome types are currently hardcoded, and it will probably stay that way. Here are the biome enums:
NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13

OCEAN and ICE are special cases which are not simulated, because nobody will be seeing much of them anyway.

There should be a worldgen setting: LARGEST_BIOME_SIZE. If a biome is generated that is too large (except water), it should be removed and then a new biome layer is generated, preserving all good biomes.

*/


#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>

class World_Biome: public TableInterface
{
	RandomLehmer rng;
	FloraGenerator floraGenerator;

	public:

	bool isGenerated; // true if all tiles have been generated.
	bool threadAccess; // true if a thread is using this biome

	int id; // the biome's id. Used to lookup biome tables.

	std::string name;
	unsigned int size; /* size in tiles */
	unsigned char type; /* Biome type */

	Vector <HasXY> vXY; // every tile coordinate of this biome.
	Vector <World_Local*> vMap; // pointer to every map on this biome

	// placeholders for objects

	Vector <Flora*> vFlora; // Vector of Flora types
	//Vector <std::string> vHerbivore;
	//Vector <std::string> vCarnivore;
	
	

	World_Biome()
	{
		name="";
		size=0;
		rng.seed(globalRandom.rand32());

		isGenerated=false;
		threadAccess=false;

		id=-1;
	}

	virtual ~World_Biome()
	{
	}

	// generate entire biome including terrain, flora, etc.
	void generate()
	{
		if ( id == -1 )
		{ return; }
		generateFlora();
		generateLocals();
		isGenerated=true;
	}

	// generate all local maps for this biome
	// map generation will be done per-biome rather than per tile
	void generateLocals()
	{
		if ( id == -1 )
		{ return; }

		SaveFileManager sfm;

		for (int i=0;i<vMap.size();++i)
		{
			if ( QUIT_FLAG )
			{ return; }

			vMap(i)->biome=this;
			vMap(i)->generate(false);
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
		std::cout<<"saving to: "<<biomePath<<"\n";
		sfm.saveToFile(biomePath);
	}

	void generateFlora()
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

		unsigned int minFlora = sqrt(size)/1.5;
		if ( minFlora < 3 ) { minFlora=3; }

		unsigned int floraAmount = minFlora+rng.rand32(1+(sqrt(size)/1.5));

		if (floraAmount < 3) { floraAmount=3; }
		if (floraAmount > 255) { floraAmount = 255; }
		
		// Each biome should have up to 3 hardcoded flora. For example deserts will always have Cactuses.
		// only additional flora will be randomly generated.
		
		Vector <Flora*>* vFlora2 = floraGenerator.generate(floraAmount);

		std::cout<<"Flora generated for biome:\n";
		for (int i=0;i<vFlora2->size();++i)
		{
			std::cout<<" * "<<(*vFlora2)(i)->getName()<<"\n";
		}

		//std::cout<<"Biome size: "<<size<<" generating "<<floraAmount<<" flora.\n";

		// generate rarities. A 3-way split would probably be fine.
		// ID 0 is always the primary flora for the biome, typically a tree
		// ID 1 is always some kind of plant
		// and ID 2 is always some kind of rare plant.
		// The rest should be random.

		// assign base flora for each biome
		if (type == DESERT)
		{
			Flora* flora = new Flora("Cactus",100);
			flora->setFoodValues(0,0,0,0,0,0);
			vFlora.push(flora);

			Flora* flora2 = new Flora("Shrub",25);
			flora2->setFoodValues(0,0,0,0,5,1);
			vFlora.push(flora2);

			Flora* flora3 = new Flora("Desert plant",10);
			flora3->setFoodValues(0,0,5,1,4,1);
			vFlora.push(flora3);
		}
		else if (type == SNOW)
		{
			Flora* flora = new Flora("Fir tree",100);
			flora->setFoodValues(0,0,0,0,5,1);
			vFlora.push(flora);

			Flora* flora2 = new Flora("Bush",25);
			flora2->setFoodValues(0,0,5,1,5,1);
			vFlora.push(flora2);

			Flora* flora3 = new Flora("Hardy plant",10);
			flora3->setFoodValues(0,0,5,1,4,1);
			vFlora.push(flora3);
		}
		else if (type == WETLAND)
		{
			Flora* flora = new Flora("Willow tree",100);
			flora->setFoodValues(0,0,0,0,5,1);
			vFlora.push(flora);

			Flora* flora2 = new Flora("Swamp plant",25);
			flora2->setFoodValues(0,0,5,1,5,1);
			vFlora.push(flora2);

			Flora* flora3 = new Flora("Swamp plant",10);
			flora3->setFoodValues(0,0,5,1,4,1);
			vFlora.push(flora3);
		}
		else
		{
			Flora* flora = new Flora("Tree",100);
			flora->setFoodValues(0,0,0,0,5,1);
			vFlora.push(flora);

			Flora* flora2 = new Flora("Common plant",25);
			flora2->setFoodValues(0,0,5,1,5,1);
			vFlora.push(flora2);

			Flora* flora3 = new Flora("Uncommon plant",10);
			flora3->setFoodValues(0,0,5,1,4,1);
			vFlora.push(flora3);
		}

		for (unsigned int i=3;i<floraAmount;++i)
		{
			std::string floraName = "";

			int common = rng.rand8()%3;

			if (common==0)
			{
				Flora* flora2 = new Flora("Common flora",20);
				flora2->setFoodValues(0,0,5,1,5,1);
				vFlora.push(flora2);
			}
			else if (common==1)
			{
				Flora* flora2 = new Flora("Uncommon flora",10);
				flora2->setFoodValues(0,0,5,1,5,1);
				vFlora.push(flora2);
			}
			else
			{
				Flora* flora2 = new Flora("Rare flora",5);
				flora2->setFoodValues(0,0,5,1,5,1);
				vFlora.push(flora2);
			}
		}

		// //std::cout<<"Spawning 10 flora:\n";
		// for (int i=0;i<10;++i)
		// {
		// Flora * flora = getFlora();
		// if ( flora )
		// {
		// //std::cout<<"   "<<flora->name<<"\n";
		// }
		// else
		// {
		// //std::cout<<"Error: No flora\n";
		// }


		// }


		// assign local ids to the flora (1-255)

		for (int i=0;i<vFlora.size();++i)
		{
			vFlora(i)->id=i+1;
		}
	}

	// pick a flora type from the weighted list to spawn
	Flora* getFlora()
	{
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
	
	
	// TableInterface
	std::string getColumn(std::string _column)
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
	std::string getColumnType(std::string _column)
	{
		if ( _column == "size" )
		{
			return "number";
		}
		return "string";
	}
};

#endif