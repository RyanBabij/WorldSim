#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_CPP
#define WORLDSIM_WORLD_LOCAL_CPP

/* WorldSim: World_Local
#include "World_Local.cpp" */

/**
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

//#include <Math/Fractal/DiamondSquareAlgorithm.hpp>
#include <Math/Fractal/DiamondSquareAlgorithmCustomRange.hpp> // For heightmaps
#include <Math/Fractal/LinearMidpointDisplacement.hpp> // For border values

World_Local::World_Local()
{
	globalX=0;
	globalY=0;

#ifdef WILDCAT_THREADING
	globalX_TS=0;
	globalY_TS=0;

	threadAccess=false;
#endif

	landID=-1;

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
	dataSubterranean=0;
	
	centerHeight=0;
	
	biome=0;
}

World_Local::~World_Local()
{
#ifdef FAST_EXIT
	if (QUIT_FLAG) { return; }
#endif

	initialized = false;
	active = false;

	vCreature.deleteAll();
	vItem.deleteAll();
	vObjectGeneric.deleteAll();

	if (data)
	{ delete data; }
	data=0;

	// Note that abstractData isn't deleted
	// For now it is left in memory for faster simulation
	// if (abstractData)
	// { delete abstractData; }
	// abstractData=0;

	if (dataSubterranean)
	{ delete dataSubterranean; }
	dataSubterranean=0;
}

void World_Local::unload()
{
	active = false;

#ifdef FAST_EXIT
	if (QUIT_FLAG) { return; }
#endif

	vCreature.deleteAll();
	vItem.deleteAll();
	vObjectGeneric.deleteAll();

	if (data!=0)
	{ delete data; }
	data=0;

	if (dataSubterranean!=0)
	{ delete dataSubterranean; }
	dataSubterranean=0;
}


void World_Local::init(const int _globalX, const int _globalY, const enumBiome _biomeID, const int _seed = 0, const int _hasRiver=-1)
{
	baseBiome = _biomeID;

	seed = _seed;
	hasRiver = _hasRiver;

	globalX = _globalX;
	globalY = _globalY;

	#ifdef WILDCAT_THREADING
	globalX_TS=globalX;
	globalY_TS=globalY;
	#endif

	rng.seed(seed);
	// This needs to be changed to initialise only when generated.
	//aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	//aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);

	switch(_biomeID)
	{
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
			centerHeight=1;
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
			centerHeight=1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/5)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/5)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/10)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/2)+LOCAL_MAP_SIZE;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/5)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/2)+LOCAL_MAP_SIZE/5;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/5)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/20)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/15)+1;
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
			centerHeight=rng.rand32(LOCAL_MAP_SIZE/5)+1;
			break;
		}
		default:
		{
			//baseMoveCost = 0;
			//canHaveSettlement = false;
			baseFertility = 0;
			canMove = 0;
			//baseLogisticsCost = 0;
			//defensiveBonus = 0;
			baseMetal = 0;
			centerHeight=1;
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

bool World_Local::generate(bool cache /* =true */, World_Local* c0, World_Local* c1, World_Local* c2,
	World_Local *c3, World_Local* c4, World_Local* c5, World_Local* c6, World_Local* c7)
{
	std::cout<<"Generate called for map: "<<globalX<<", "<<globalY<<".\n";
	
	std::cout<<"Maps recieved: "<<c0<<", "<<c1<<", "<<c2<<", "<<c3<<", "<<c4<<", "<<c5<<", "<<c6<<", "<<c7<<"\n";

	// World is already loaded and running
	if ( active )
	{
		std::cout<<"Already active\n";
		return true;
	}

	// World has invalid coordinates so we can't generate it.
	if ( world.isSafe(globalX,globalY) == false )
	{ return false; }

	if (cache && initialized)
	{
		std::cout<<"local is initialized\n";
		if (load())
		{
			std::cout<<"Load() called\n";
			active = true;
			return true;
		}
		std::cout<<"Warning load of: "<<globalX<<", "<<globalY<<" failed.\n";
		active=false;
		return false;
	}

	// Create data struct if necessary.
	if (data==0)
	{ data = new Data; }

	// Create abstract struct if necessary
	if (abstractData==0)
	{ abstractData = new AbstractData; }

#ifdef FAST_EXIT
	if (QUIT_FLAG) { return false; }
#endif

	std::cout<<"Generating\n";

	localDate.set(0,0,0,CALENDAR_INITIAL_HOUR,CALENDAR_INITIAL_MINUTE,0);

	initialized = true;
	active = true;

	texFar.create(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0,false);


	data->aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	//data->aStatic.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
	//data->aStatic.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);

	abstractData->bfCollision.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	abstractData->aStaticID.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);

	//abstractData->bfFlora.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	//abstractData->bfStatic.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	//abstractData->bfMob.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);

	// nulls must be passed as 1, because 0 will be overwritten by the midpoint displacement algorithm.
	generateHeightMap(c0?c0->centerHeight:1,c1?c1->centerHeight:1,c2?c2->centerHeight:1,c3?c3->centerHeight:1,
	c4?c4->centerHeight:1,c5?c5->centerHeight:1,c6?c6->centerHeight:1,c7?c7->centerHeight:1,0);

#ifdef FAST_EXIT
	if (QUIT_FLAG) { return false; }
#endif

	// Take the seed for this world tile and expand it into a subseed for every local tile */
	rng.seed(seed);

	int midX = LOCAL_MAP_SIZE/2;

	for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
	{
		for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
		{
			data->aLocalTile(_x,_y).seed = rng.rand32(USHRT_MAX-1);
			//data->aLocalTile(_x,_y).clearObjects();
			//data->aLocalTile(_x,_y).height = aFullHeight(_x,_y);

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
				
				// make high-altitude tiles into snow tiles.
				if ( data->aLocalTile(_x,_y).height > LOCAL_MAP_SIZE / 2 )
				{
					data->aLocalTile(_x,_y).baseTerrain = SNOW;
					baseTreeChance=100;
				}

				//Put down some water for drinking
				if (rng.oneIn(500))
				{
					data->aLocalTile(_x,_y).baseTerrain = OCEAN;
					continue;
				}
				else if (rng.oneIn(400)) /* Put down some testing objects */
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
				// // Spawn Statics
				// // Spawn Flora
				// else if (rng.oneIn(baseTreeChance))
				// {
					// // spawn in a random Flora from Biome.
					// put(biome->getFlora(), _x, _y);
				// }
				// Spawn Mobs
				else if (rng.oneIn(1500) && (baseBiome == FOREST || baseBiome == GRASSLAND) )
				{
					
						Creature * c = biome->getCreature();
						if (c)
						{
							c->init();
							put(c, _x, _y);
						}
						
					//auto deer = spawnCreature();
					//deer->init();
					//put(deer,_x,_y);
				}
				else if (rng.oneIn(50) && (baseBiome == FOREST || baseBiome == GRASSLAND) )
				{
					//auto tree = new Static_Tree();
					
					if (biome)
					{
						//std::cout<<"biome\n";
						Flora * f = biome->getFlora();
						if (f)
						{
							put(f, _x, _y);
						}
						
						//std::cout<<"Put: "<<f->name<<"\n";
						
					}
					else
					{
						//std::cout<<"Nobiome\n";
					}
					
					// if (rng.oneIn(3))
					// {
						// put (new Static_Tree(), _x, _y);
					// }
					// else
					// {
						// put (new Flora(), _x, _y);
					// }
					//put (new Static_Tree(), _x, _y);
				}
				else if ( baseBiome == MOUNTAIN )
				{
					if (rng.oneIn(10))
					{
						auto * rockyBoi = new WorldObject_Rock;
						if ( rng.oneIn(10) )
						{
							rockyBoi->nGold = 100;
						}
						//put (rockyBoi,_x,_y);
					}
					
					else if (biome && rng.oneIn(100))
					{
						//std::cout<<"biome\n";
						Flora * f = biome->getFlora();
						if (f)
						{
							put(f, _x, _y);
						}
						
						//std::cout<<"Put: "<<f->name<<"\n";
						
					}
					else
					{
						///std::cout<<"Nobiome\n";
					}
					
				}
				
				
				// if the tile is mostly cliffs, or cliffs on opposite sides, make it inaccessible.
				if ( data->aLocalTile(_x,_y).bWall == 0b11111111
				// mostly cliffs
|| data->aLocalTile(_x,_y).bWall == 0b11101110
|| data->aLocalTile(_x,_y).bWall == 0b11011101
|| data->aLocalTile(_x,_y).bWall == 0b10111011
|| data->aLocalTile(_x,_y).bWall == 0b01110111

// cliffs on opposite sides

|| data->aLocalTile(_x,_y).bWall == 0b10101010
|| data->aLocalTile(_x,_y).bWall == 0b01010101

// any cliff at all
|| data->aLocalTile(_x,_y).bWall != 0

)
				{
					//std::cout<<"Found inaccassible\n";
					data->aLocalTile(_x,_y).bWall = 0;
					data->aLocalTile(_x,_y).baseTerrain = UNDERGROUND;
				}
			}
		}
	}

	//generateSubterranean();

	if ( baseBiome == GRASSLAND && rng.oneIn(100) )
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

	//This generates a distance texture of the map where 1 local tile = 1 pixel,
	// to allow fast rendering of local maps from a distance.
	// textures are upside-down for some reason
	for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
	{
		for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
		{
			const Texture * t =data->aLocalTile(_x,_y).currentTexture();
			texFar.setPixel(_x,LOCAL_MAP_SIZE-_y,0,t->averageRed);
			texFar.setPixel(_x,LOCAL_MAP_SIZE-_y,1,t->averageGreen);
			texFar.setPixel(_x,LOCAL_MAP_SIZE-_y,2,t->averageBlue);
			texFar.setPixel(_x,LOCAL_MAP_SIZE-_y,3,255);
		}
	}
	bindNearestNeighbour(&texFar,COMPRESS_TEXTURES);

#ifdef FAST_EXIT
	if (QUIT_FLAG) { return false; }
#endif

	if ( cache )
	{
		save();
	}
	return true;
}

// I want to have subterranean separate from normal map generation.
// It should only be loaded on demand. It could potentially be in its own file
// Subterranean can only be generated if map has been generated
bool World_Local::generateSubterranean()
{
	if (data==0 ) { return false; }

	if ( dataSubterranean == 0 )
	{ dataSubterranean = new Data_Subterranean; }
	dataSubterranean->aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);

	//int nGemSeams = rng.rand8(3);
	// int nGemSeams=0;
	// while (nGemSeams-- > 0)
	// {
	// Vector <HasXY*> * vGemSeam = getRandomWalk(rng.rand8(40)+10);

	// for (int i=0;i<vGemSeam->size();++i)
	// {
	// dataSubterranean->aSubterranean((*vGemSeam)(i)).nGems=rng.multiRoll8(3,3);
	// }
	// vGemSeam->deleteAll();
	// delete vGemSeam;
	// }

	// //int nMetalSeams = rng.rand8(6);
	// int nMetalSeams=0;
	// while (nMetalSeams-- > 0)
	// {
	// Vector <HasXY*> * vMetalSeam = getRandomWalk(rng.rand8(40)+10);

	// for (int i=0;i<vMetalSeam->size();++i)
	// {
	// dataSubterranean->aSubterranean((*vMetalSeam)(i)).nMetal=rng.multiRoll8(3,3);
	// }
	// vMetalSeam->deleteAll();
	// delete vMetalSeam;
	// }

	int midX = LOCAL_MAP_SIZE/2;

	for ( int _y=0;_y<LOCAL_MAP_SIZE;++_y)
	{
		for ( int _x=0;_x<LOCAL_MAP_SIZE;++_x)
		{
			dataSubterranean->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
			dataSubterranean->aSubterranean(_x,_y).seed = rng.rand32(PORTABLE_INT_MAX-1);
			//dataSubterranean->aSubterranean(_x,_y).clearObjects();
			dataSubterranean->aSubterranean(_x,_y).height = 0;


			// //dataSubterranean->aSubterranean(_x,_y).hasGems=Random::oneIn(100);
			// if (rng.oneIn(1000))
			// {
			// //dataSubterranean->aSubterranean(_x,_y).hasGems=true;
			// }
		}
	}

	//BUILD CAVE
	//if (Random::oneIn(2))
	//if (true)
	// if (false)
	// {
	// hasCave=true;
	// //Basically random walk with extras, and then occasionally breach the surface in the form of a cave tile.
	// int caveSize = rng.multiRoll32(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE)*2;

	// Vector <HasXY*> * vCaveMap = getRandomWalk(caveSize);

	// int nEntrances = Random::randomInt((vCaveMap->size()/500));
	// if (nEntrances > 10) { nEntrances=10; }

	// for (int i2=0;i2<vCaveMap->size();++i2)
	// {
	// dataSubterranean->aSubterranean((*vCaveMap)(i2)).baseTerrain = GRASSLAND;
	// dataSubterranean->aSubterranean((*vCaveMap)(i2)).nGems=0;
	// dataSubterranean->aSubterranean((*vCaveMap)(i2)).nMetal=0;

	// if (rng.oneIn(100))
	// {
	// auto creature = new Creature_Bat;
	// creature->init();
	// put(creature,(*vCaveMap)(i2),true);
	// }

	// if (rng.oneIn(25))
	// {
	// put(new Flora_Blackweed(),(*vCaveMap)(i2),true);
	// }

	// if (i2<nEntrances)
	// {
	// data->aLocalTile((*vCaveMap)(i2)).isCave=true;
	// dataSubterranean->aSubterranean((*vCaveMap)(i2)).isCave=true;
	// }
	// }
	// vCaveMap->deleteAll();
	// delete vCaveMap;
	// }
	return false;
}

// Elevation consistency across borders is done by generating the border values first using a linear midpoint
// displacement algorithm. The seed is currently fixed but in future will probably use an agreed map seed which is
// shared by both sides of the border.
// The heightmap generator then uses these border values when generating the map. This ensures a smooth elevation across
// map borders, with the minor side-effect that map borders share the same elevation values with the adjacent map.
// However I think this will probably end up being a good thing for helping pathfinding across maps.
void World_Local::generateHeightMap(const short int c0, const short int c1, const short int c2, const short int c3,
	const short int c4, const short int c5, const short int c6, const short int c7, const unsigned int _seed)
{
	//aHeightDiff.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
	
	std::cout<<"Recieved c values: "<<c0<<", "<<c1<<", "<<c2<<", "<<c3<<", "<<c4<<", "<<c5<<", "<<c6<<", "<<c7<<"\n";
	
	
	// calculate corner values (note that no corner value may be greater than the max height of a connected map)
	int corn1 = centerHeight; // top-left
	if (c0 < corn1) { corn1 = c0; }
	if (c1 < corn1) { corn1 = c1; }
	if (c3 < corn1) { corn1 = c3; }
	
	int corn2 = centerHeight; // top-right
	if (c1 < corn2) { corn2 = c1; }
	if (c2 < corn2) { corn2 = c2; }
	if (c4 < corn2) { corn2 = c4; }
	
	int corn3 = centerHeight; //bottom-left
	if (c3 < corn3) { corn3 = c3; }
	if (c5 < corn3) { corn3 = c5; }
	if (c6 < corn3) { corn3 = c6; }
	
	int corn4 = centerHeight; // bottom-right
	if (c4 < corn4) { corn4 = c4; }
	if (c6 < corn4) { corn4 = c6; }
	if (c7 < corn4) { corn4 = c7; }
	
	int leftMax = c3<centerHeight?c3:centerHeight;
	int rightMax = c4<centerHeight?c4:centerHeight;
	int topMax = c1<centerHeight?c1:centerHeight;
	int bottomMax = c6<centerHeight?c6:centerHeight;
	
	
	//int corn1 = (c0 + c1 + c3 + centerHeight) / 4; // top-left
	//int corn2 = (c1 + c2 + c4 + centerHeight) / 4; // top-right
	//int corn3 = (c3 + c5 + c6 + centerHeight) / 4; // bottom-left
	//int corn4 = (c4 + c6 + c7 + centerHeight) / 4; // bottom-right
	
	std::cout<<"Corn1: "<<c0<<" + "<<c1<<" + "<<c3<<" + "<<centerHeight<<"\n";
	
	corn1==0?corn1=1:0;
	corn2==0?corn2=1:0;
	corn3==0?corn3=1:0;
	corn4==0?corn4=1:0;
	
	std::cout<<"Corner values are: "<<corn1<<", "<<corn2<<", "<<corn3<<", "<<corn4<<"\n";
	
	
	//lpd.seed(SEEDER);
	//lpd.seed(123);
	
	// We need to generate the 4 edges using the 4 corner values (and also the same seed)
	
	int aBorderLeft [LOCAL_MAP_SIZE] = {0};
	aBorderLeft[0] = corn3;
	aBorderLeft[LOCAL_MAP_SIZE-1] = corn1;
	
	// The maximum height of the borders can't be higher then the maximum height of either 2 tiles.
	// set maximum left border val here. 
	
	int aBorderRight [LOCAL_MAP_SIZE] = {0};
	aBorderRight[0] = corn4;
	aBorderRight[LOCAL_MAP_SIZE-1] = corn2;
	
	int aBorderTop [LOCAL_MAP_SIZE] = {0};
	aBorderTop[0] = corn1;
	aBorderTop[LOCAL_MAP_SIZE-1] = corn2;
	
	int aBorderBottom [LOCAL_MAP_SIZE] = {0};
	aBorderBottom[0] = corn3;
	aBorderBottom[LOCAL_MAP_SIZE-1] = corn4;
	
	LinearMidpointDisplacement lpd;
	lpd.seed(123);
	lpd.generate(aBorderLeft,LOCAL_MAP_SIZE,leftMax,2);
	lpd.seed(123);
	lpd.generate(aBorderRight,LOCAL_MAP_SIZE,rightMax,2);
	lpd.seed(123);
	lpd.generate(aBorderTop,LOCAL_MAP_SIZE,topMax,2);
	lpd.seed(123);
	lpd.generate(aBorderBottom,LOCAL_MAP_SIZE,bottomMax,2);
	
	// generate border values here (use fixed seed to start out).
	
	// we first must calculate the 4 corner values.
	
	//get top corner value
	
	//get bottom corner value.
	
	// GENERATE HEIGHTMAP
	DiamondSquareAlgorithmCustomRange dsa2;
	dsa2.seed(seed);
	std::cout<<"Setting dsa maxvalue to: "<<centerHeight<<"\n";
	dsa2.maxValue=centerHeight;
	//dsa2.maxValue = 127; // limit this for testing.
	//dsa2.maxValue = 257; // limit this for testing.
	
	// For now we keep the array in class mem for testing.
	// In future the array is to be loaded on demand using the relative heightmap.
	//ArrayS2 <int> aFullHeight;
	aFullHeight.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
	
	std::cout<<"Final border values:\n";
	
	for (int i=0;i<LOCAL_MAP_SIZE;++i)
	{
		if ( aBorderLeft[i]==0 )
		{
			aBorderLeft[i]=1;
		}
		if ( aBorderRight[i]==0 )
		{
			aBorderRight[i]=1;
		}
		if ( aBorderTop[i]==0 )
		{
			aBorderTop[i]=1;
		}
		if ( aBorderBottom[i]==0 )
		{
			aBorderBottom[i]=1;
		}
		std::cout<<aBorderLeft[i]<<", ";
	}
	
	
	// set the border values now.
	for (int x=0;x<LOCAL_MAP_SIZE;++x)
	{
		aFullHeight(x,0) = aBorderBottom[x];
		aFullHeight(x,LOCAL_MAP_SIZE-1) =  aBorderTop[x];
		//aFullHeight(x,0) = aBorderLeft[x];
		//aFullHeight(x,LOCAL_MAP_SIZE-1) = aBorderLeft[x];
	}
	// set the border values now.
	for (int y=0;y<LOCAL_MAP_SIZE;++y)
	{
		aFullHeight(0,y) = aBorderLeft[y];
		aFullHeight(LOCAL_MAP_SIZE-1,y) =  aBorderRight[y];
		//aFullHeight(0,y) = aBorderLeft[y];
		//aFullHeight(LOCAL_MAP_SIZE-1,y) =  aBorderLeft[y];
	}
	
	

	
	// determine cliff values here
	if (baseBiome == MOUNTAIN)
	{
		//aFullHeight.fillBorder(1);
		// Generate the heightmap
		dsa2.generate(&aFullHeight,0,2,0.72,centerHeight,1);
		
		// smoothing pass
		for (int i=0;i<4;++i)
		{
			for (unsigned short int y=1;y<LOCAL_MAP_SIZE-1;++y)
			{
				for (unsigned short int x=1;x<LOCAL_MAP_SIZE-1;++x)
				{
					int avgNeighbors = (aFullHeight(x-1,y)+aFullHeight(x+1,y)+aFullHeight(x,y-1)+aFullHeight(x,y+1))/4;
					
					// smooth out small cliffs
					if ( aFullHeight(x,y) > aFullHeight(x-1,y)
						&& aFullHeight(x,y) > aFullHeight(x+1,y)
					&& aFullHeight(x,y) > aFullHeight(x,y-1)
					&& aFullHeight(x,y) > aFullHeight(x,y+1) )
					{
						aFullHeight(x,y) = avgNeighbors;
					}
				}
			}
		}
		

		for (unsigned short int y=0;y<LOCAL_MAP_SIZE;++y)
		{
			for (unsigned short int x=0;x<LOCAL_MAP_SIZE;++x)
			{
				//aFullHeight(x,y) = aFullHeight(x,y) / 14;
				//aFullHeight(x,y) = aFullHeight(x,y) * 14;
				data->aLocalTile(x,y).height = aFullHeight(x,y);
			}
		}
		
		
		for (unsigned short int y=1;y<LOCAL_MAP_SIZE-1;++y)
		{
			for (unsigned short int x=1;x<LOCAL_MAP_SIZE-1;++x)
			{
				

				
				//check left, right, up, down
				
				const short int heightDiffLeft = aFullHeight(x-1,y) - aFullHeight(x,y);
				const short int heightDiffRight = aFullHeight(x+1,y) - aFullHeight(x,y);
				const short int heightDiffUp = aFullHeight(x,y+1) - aFullHeight(x,y);
				const short int heightDiffDown = aFullHeight(x,y-1) - aFullHeight(x,y);
				
				data->aLocalTile(x,y).bWall = 0;
				
				const short int cliffDiff = 5;
				
				// set cliff bits
				if ( heightDiffLeft > cliffDiff || heightDiffLeft < -cliffDiff )
				{
					//std::cout<<"BWALL\n";
					data->aLocalTile(x,y).bWall |= 0b00010001;
				}
				if ( heightDiffRight > cliffDiff || heightDiffRight < -cliffDiff )
				{
					//std::cout<<"BWALL\n";
					data->aLocalTile(x,y).bWall |= 0b01000100;
				}
				if ( heightDiffUp > cliffDiff || heightDiffUp < -cliffDiff )
				{
					//std::cout<<"BWALL\n";
					data->aLocalTile(x,y).bWall |= 0b10001000;
				}
				if ( heightDiffDown > cliffDiff || heightDiffDown < -cliffDiff )
				{
					//std::cout<<"BWALL\n";
					data->aLocalTile(x,y).bWall |= 0b00100010;
				}
				

				
				// if ( heightDiffLeft % cliffDiff == 0 )
				// {
					// //std::cout<<"BWALL\n";
					// data->aLocalTile(x,y).bWall |= 0b00010001;
				// }
				// if ( heightDiffRight % cliffDiff == 0 )
				// {
					// //std::cout<<"BWALL\n";
					// data->aLocalTile(x,y).bWall |= 0b01000100;
				// }
				// if ( heightDiffUp % cliffDiff == 0 )
				// {
					// //std::cout<<"BWALL\n";
					// data->aLocalTile(x,y).bWall |= 0b10001000;
				// }
				// if ( heightDiffDown % cliffDiff == 0 )
				// {
					// //std::cout<<"BWALL\n";
					// data->aLocalTile(x,y).bWall |= 0b00100010;
				// }
			}
		}
	}
	else
	{
		//aFullHeight.fillBorder(1);
		// Generate the heightmap
		dsa2.generate(&aFullHeight,0,0,0.75,centerHeight);
		
		for (unsigned short int y=0;y<LOCAL_MAP_SIZE;++y)
		{
			for (unsigned short int x=0;x<LOCAL_MAP_SIZE;++x)
			{
				//aFullHeight(x,y) = aFullHeight(x,y) / 14;
				//aFullHeight(x,y) = aFullHeight(x,y) * 14;
				data->aLocalTile(x,y).height = aFullHeight(x,y);
			}
		}
	}
	
	// build the relative height map.
	
	// we build the relative heights using a defined offset based on the quadrant.
	// height is always compressed to the offset with a maximum range: -127, 127. Any offsets exceeding this range are
	// set to max/min and any resulting strange geography is ignored. It's unlikely for there to be such large
	// offsets anyway.
	
	// Since local map size is always x^2+1, there is always a center tile.
	unsigned short int cX = LOCAL_MAP_SIZE / 2;
	unsigned short int cY = LOCAL_MAP_SIZE / 2;
	
	//centerHeight = aFullHeight(cX,cY);
	
	//ArrayS2 <char> aHeightOffset; // stores height offset from point close to center to compress to 1 byte per tile.
	//aHeightOffset.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
	
	//std::cout<<"Center coord is: "<<cX<<", "<<cY<<"\n";
	
	// for (unsigned short int y=0;y<LOCAL_MAP_SIZE;++y)
	// {
		// for (unsigned short int x=0;x<LOCAL_MAP_SIZE;++x)
		// {
			// if ( x < cX ) // left quadrant
			// {
				// if ( y < cY ) // bottom-left quadrant
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x+1,y+1);
				// }
				// else if ( y > cY ) // top-left quadrant
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x+1,y-1);
				// }
				// else // exactly left of center
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x+1,y);
				// }
			// }
			// else if (x > cX) // right quadrant
			// {
				// if ( y < cY ) // bottom-right quadrant
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x-1,y+1);
				// }
				// else if ( y > cY ) // top-right quadrant
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x-1,y-1);
				// }
				// else // exactly right of center
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x-1,y);
				// }
			// }
			// else // directly above/below.
			// {
				// if ( y < cY ) // below, use above tile as offset ref.
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x,y+1);
				// }
				// else if ( y > cY ) // above, use below tile as offset ref.
				// {
					// aHeightDiff(x,y) = (char) aFullHeight(x,y) - aFullHeight(x,y-1);
				// }
				// else // center tile is always the final reference point.
				// {
					// aHeightDiff(x,y)=0;
				// }
			// }
		// }
	// }
	
	
	
	// std::cout<<"Printing heightmap orig/offset:\n";
	// for (int i=0;i<LOCAL_MAP_SIZE;++i)
	// {
		
		// //std::cout<<"("<<cX<<", "<<i<<")"<<aFullHeight(cX,i)<<" -> "<<(int)aHeightOffset(cX,i)<<"\n";
	// }
	//sleep(1000);
	//exit(0);
	
}

std::string World_Local::getSaveData()
{
	// Can't save world if it's not fully generated.
	if ( data==0 ) { return ""; }

	std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	std::string abstractPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + "-abstract.dat";

	if (FileManager::directoryExists(world.strSavePath) == false)
	{
		std::cout<<"Error: Unable to access directory.\n";
		return "";
	}

	// Make the file or clear it.
	//FileManager::makeNewFile(localMapPath);
	//FileManager::makeNewFile(abstractPath);
	SaveFileManager sfm;
	SaveFileManager sfmAbstract;

	std::string saveData="";

	SaveChunk chonk ("TILE ARRAY");
	//SaveChunk abstractChonk("ABSTRACT");
	SaveChunk abstractChonk2("ABSCOL");
	SaveChunk abstractStaticChonk("ABSSTAT");

	std::string strStaticID = "";

	// Only unload the local map if it is loaded.
	for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
	{
		for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
		{
			chonk.add(data->aLocalTile(_x,_y).getSaveData());
			//abstractChonk.add(data->aLocalTile(_x,_y).getAbstractData());

			if ( data->aLocalTile(_x,_y).hasMovementBlocker() )
			{
				abstractData->bfCollision.set(_x,_y,true);
			}
			// if (data->aStatic(_x,_y)) // if there's a static here, save it's ID (0-255)
			// {
			// abstractData->aStaticID(_x,_y)='A';
			// }
			if (data->aLocalTile(_x,_y).objStatic) // if there's a static here, save it's ID (0-255)
			{
				abstractData->aStaticID(_x,_y)=data->aLocalTile(_x,_y).objStatic->id;
				strStaticID+=data->aLocalTile(_x,_y).objStatic->id;
			}
			else
			{
				strStaticID+=(unsigned char)0;
			}
		}
	}
	// copying the array as a string is inefficient but safer for now
	abstractChonk2.add(abstractData->bfCollision.toString());
	sfmAbstract.addChunk(abstractChonk2);
	abstractStaticChonk.add(strStaticID);
	sfmAbstract.addChunk(abstractStaticChonk);

	if (dataSubterranean)
	{
		SaveChunk chonkSub ("SUBTERRANEAN ARRAY");
		SaveChunk chonkGem ("GEM ARRAY");
		SaveChunk chonkMetal ("METAL ARRAY");
		for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
		{
			for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
			{
				chonkSub.add(dataSubterranean->aSubterranean(_x,_y).getSaveData());
				chonkGem.add(DataTools::toString(dataSubterranean->aSubterranean(_x,_y).nGems));
				chonkMetal.add(DataTools::toString(dataSubterranean->aSubterranean(_x,_y).nMetal));
			}
		}
		sfm.addChunk(chonkSub);
		sfm.addChunk(chonkGem);
		sfm.addChunk(chonkMetal);
	}

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

	std::string sData = sfmAbstract.data;
	sData+="\n\n";
	sData+=sfm.data;
	return sData;
}

bool World_Local::save()
{
	// Can't save world if it's not fully generated.
	if ( data==0 ) { return false; }

	std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	std::string abstractPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + "-abstract.dat";

	if (FileManager::directoryExists(world.strSavePath) == false)
	{
		std::cout<<"Error: Unable to access directory.\n";
		return false;
	}

	// Make the file or clear it.
	FileManager::makeNewFile(localMapPath);
	FileManager::makeNewFile(abstractPath);
	SaveFileManager sfm;
	SaveFileManager sfmAbstract;

	std::string saveData="";

	SaveChunk chonk ("TILE ARRAY");
	//SaveChunk abstractChonk("ABSTRACT");
	SaveChunk abstractChonk2("ABSCOL");
	SaveChunk abstractStaticChonk("ABSSTAT");

	std::string strStaticID = "";

	// Only unload the local map if it is loaded.
	for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
	{
		for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
		{
			chonk.add(data->aLocalTile(_x,_y).getSaveData());
			//abstractChonk.add(data->aLocalTile(_x,_y).getAbstractData());

			if ( data->aLocalTile(_x,_y).hasMovementBlocker() )
			{
				abstractData->bfCollision.set(_x,_y,true);
			}
			// if (data->aStatic(_x,_y)) // if there's a static here, save it's ID (0-255)
			// {
			// abstractData->aStaticID(_x,_y)='A';
			// }
			if (data->aLocalTile(_x,_y).objStatic) // if there's a static here, save it's ID (0-255)
			{
				abstractData->aStaticID(_x,_y)=data->aLocalTile(_x,_y).objStatic->id;
				strStaticID+=data->aLocalTile(_x,_y).objStatic->id;
			}
			else
			{
				strStaticID+=(unsigned char)0;
			}
		}
	}
	// copying the array as a string is inefficient but safer for now
	abstractChonk2.add(abstractData->bfCollision.toString());
	sfmAbstract.addChunk(abstractChonk2);
	abstractStaticChonk.add(strStaticID);
	sfmAbstract.addChunk(abstractStaticChonk);

	//sfmAbstract.addChunk(abstractChonk);
	sfm.addChunk(chonk);

	if (dataSubterranean)
	{
		SaveChunk chonkSub ("SUBTERRANEAN ARRAY");
		SaveChunk chonkGem ("GEM ARRAY");
		SaveChunk chonkMetal ("METAL ARRAY");
		for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
		{
			for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
			{
				chonkSub.add(dataSubterranean->aSubterranean(_x,_y).getSaveData());
				chonkGem.add(DataTools::toString(dataSubterranean->aSubterranean(_x,_y).nGems));
				chonkMetal.add(DataTools::toString(dataSubterranean->aSubterranean(_x,_y).nMetal));
			}
		}
		sfm.addChunk(chonkSub);
		sfm.addChunk(chonkGem);
		sfm.addChunk(chonkMetal);
	}

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
	sfmAbstract.saveToFile(abstractPath);

	return true;
}

bool World_Local::load()
{
	std::string localMapPath = world.strSavePath + "/" + DataTools::toString(globalX) + "-" + DataTools::toString(globalY) + ".dat";
	if ( FileManager::directoryExists(localMapPath)  == false )
	{
		// std::cout<<"Error: Unable to access directory to load map.\n";
		return false;
	}

	initialized = true;
	active = true;

	if ( data == 0 )
	{ data=new Data; }
	data->aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
	//data->aStatic.init(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE,0);
	//dataSubterranean->aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);

	// Open the cache file for loading into memory.
	SaveFileManager sfm;

	std::string saveData="";
	sfm.loadFile(localMapPath);

	SaveChunk* chonk = sfm.getChunk("TILE ARRAY");

	if ( chonk != 0 )
	{
		int i=0;
		// Only unload the local map if it is loaded.
		for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
		{
			for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
			{
				//data->aLocalTile(_x,_y).clearObjects();
				data->aLocalTile(_x,_y).loadData((*chonk)(i));

				dataSubterranean->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
				dataSubterranean->aSubterranean(_x,_y).seed = rng.rand32(PORTABLE_INT_MAX-1);
				//dataSubterranean->aSubterranean(_x,_y).clearObjects();
				dataSubterranean->aSubterranean(_x,_y).height = 0;

				//dataSubterranean->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
				//dataSubterranean->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
				//dataSubterranean->aSubterranean(_x,_y).clearObjects();
				//dataSubterranean->aSubterranean(_x,_y).height = 0;
				//chonk.add(data->aLocalTile(_x,_y).getSaveData());
				++i;
			}
		}
		delete chonk;

		SaveChunk* chonkSub = sfm.getChunk("SUBTERRANEAN ARRAY");

		if ( chonkSub != 0 )
		{
			SaveChunk* chonkGem = sfm.getChunk("GEM ARRAY");
			SaveChunk* chonkMetal = sfm.getChunk("METAL ARRAY");
			i=0;
			// Only unload the local map if it is loaded.
			for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
			{
				for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
				{
					dataSubterranean->aSubterranean(_x,_y).loadData((*chonkSub)(i));
					dataSubterranean->aSubterranean(_x,_y).nGems=DataTools::toUnsignedShort((*chonkGem)(i));
					dataSubterranean->aSubterranean(_x,_y).nMetal=DataTools::toUnsignedShort((*chonkMetal)(i));

					//data->aLocalTile(_x,_y).clearObjects();
					//data->aLocalTile(_x,_y).loadData((*chonk)(i));

					// dataSubterranean->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
					// dataSubterranean->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
					// //dataSubterranean->aSubterranean(_x,_y).clearObjects();
					// dataSubterranean->aSubterranean(_x,_y).height = 0;

					//dataSubterranean->aSubterranean(_x,_y).baseTerrain = UNDERGROUND;
					//dataSubterranean->aSubterranean(_x,_y).seed = random.randInt(PORTABLE_INT_MAX-1);
					//dataSubterranean->aSubterranean(_x,_y).clearObjects();
					//dataSubterranean->aSubterranean(_x,_y).height = 0;
					//chonk.add(data->aLocalTile(_x,_y).getSaveData());
					++i;
				}
			}
			delete chonkSub;
		}

		chonk = sfm.getChunk("OBJECT VECTOR");

		for (i=0;i<chonk->vData.size();++i)
		{
			Vector <std::string> * vToke = DataTools::tokenize(chonk->vData(i), " \t\n\r");

			if ( vToke && vToke->size() >= 3)
			{
				std::string objectType = (*vToke)(0);
				short int objectX = DataTools::toShort((*vToke)(1));
				short int objectY = DataTools::toShort((*vToke)(2));
				if (objectType == "Tree" )
				{
					if (isSafe( objectX, objectY ) )
					{
						//put (new WorldObject_Tree(1), objectX, objectY);
						//put (new Static_Tree(1), objectX, objectY);
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
	if ( !data )
	{ return false; }

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
		dataSubterranean->aSubterranean(_x,_y).add(_object);
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
		dataSubterranean->aSubterranean(_x,_y).add(_item);
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
	if ( !data )
	{ return false; }
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
		dataSubterranean->aSubterranean(_x,_y).add(_character);
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
	if ( !data )
	{ return false; }
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
		dataSubterranean->aSubterranean(_x,_y).add(_creature);
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

bool World_Local::put (Static* _static, int _x, int _y, bool subterranean)
{
	if ( !data )
	{ return false; }
	if ( data->aLocalTile.isSafe(_x,_y) == false)
	{ return false; }
	// if ( data->aStatic.isSafe(_x,_y) == false )
	// { return false; }


	// if ( data->aStatic(_x,_y) == 0 )
	// { 
		// data->aStatic(_x,_y) = _static;
	// }

	if ( data->aLocalTile(_x,_y).objStatic == 0 )
	{
		data->aLocalTile(_x,_y).objStatic=_static;
	}

	return true;
}
bool World_Local::put (Static* _object, HasXY* _xy, bool subterranean)
{ return put(_object, _xy->x, _xy->y, subterranean); }
bool World_Local::put (Static* _object, HasXY _xy, bool subterranean)
{ return put(_object, _xy.x, _xy.y, subterranean); }

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

	if ( destination == 0 || destination->data == 0 || destination->isSafe (newX,newY) == false )
	{ return false; }


	if (_object->isUnderground)
	{
		if ( dataSubterranean->aSubterranean(newX,newY).hasMovementBlocker() )
		{ return false; }

		dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);

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

	if ( destination == 0 || destination->data == 0 || destination->isSafe (newX,newY) == false )
	{ return false; }


	if (_object->isUnderground)
	{
		if ( destination->dataSubterranean->aSubterranean(newX,newY).hasMovementBlocker() )
		{ return false; }

		dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);

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
	// destination = &dataSubterranean->aSubterranean(newX,newY);
	// source = &dataSubterranean->aSubterranean(_object->x,_object->y);
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
	// if ( dataSubterranean->aSubterranean(newX,newY).hasMovementBlocker() )
	// {
	// return false;
	// }
	// dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);


	// _object->x=newX;
	// _object->y=newY;
	// _object->worldX = globalX;
	// _object->worldY = globalY;

	// dataSubterranean->aSubterranean(newX,newY).add(_object);
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
			dataSubterranean->aSubterranean(_object->x,_object->y).add(_object);
			return true;
		}
	}
	return false;
}
bool World_Local::moveDown(Character* _object)
{
	if ( !data || !_object )
	{ return false; }

	if ( isSafe(_object) )
	{
		if ( data->aLocalTile(_object->x,_object->y).isCave )
		{
			data->aLocalTile(_object->x,_object->y).remove(_object);
			_object->isUnderground=true;
			dataSubterranean->aSubterranean(_object->x,_object->y).add(_object);
			return true;
		}
	}
	return false;
}


bool World_Local::moveUp(WorldObject* _object)
{
	if ( !data || !_object )
	{ return false; }

	if ( isSafe(_object) )
	{
		if ( data->aLocalTile(_object->x,_object->y).isCave )
		{
			dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);
			_object->isUnderground=false;
			data->aLocalTile(_object->x,_object->y).add(_object);
			return true;
		}
	}
	return false;
}
bool World_Local::moveUp(Character* _object)
{
	if ( !data || !_object )
	{ return false; }

	if ( isSafe(_object) )
	{
		if ( data->aLocalTile(_object->x,_object->y).isCave )
		{
			dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);
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
	if ( _object==0 )
	{ return false; }
	if ( !data )
	{ return false; }

	// Needs to be updated. Currently performs 2 placements, one for up/down, one for left/right

	int newX = _object->x;
	int newY = _object->y;

	int direction = rng.rand8(3);

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

	int direction = rng.rand8(3);

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

	int direction = rng.rand8(3);

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
		if (dataSubterranean->aSubterranean.isSafe(_object->x,_object->y))
		{
			dataSubterranean->aSubterranean(_object->x,_object->y).remove(_object);
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
		if (dataSubterranean->aSubterranean.isSafe(_item->x,_item->y))
		{
			dataSubterranean->aSubterranean(_item->x,_item->y).remove(_item);
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
		if (dataSubterranean->aSubterranean.isSafe(_character->x,_character->y))
		{
			dataSubterranean->aSubterranean(_character->x,_character->y).remove(_character);
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
		if (dataSubterranean->aSubterranean.isSafe(_creature->x,_creature->y))
		{
			dataSubterranean->aSubterranean(_creature->x,_creature->y).remove(_creature);
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
	if ( !data )
	{ return false; }
	if (vObjectGeneric.contains(_target))
	{
		return true;
	}
	return false;
}
bool World_Local::contains(Character* _target)
{
	if ( !data )
	{ return false; }
	if (vCharacter.contains(_target))
	{
		return true;
	}
	return false;
}

//Return a vector of coordinates visible from the given location.
// This raytrace stays within map boundaries.
Vector <HasXY*> * World_Local::rayTraceLOS (int _x, int _y, const int RANGE, bool subterranean)
{
	if ( !data )
	{ return 0; }
	if (RANGE <= 0)
	{ return 0; }

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
	if ( !data )
	{ return 0; }
	if (RANGE <= 0)
	{ return 0; }

	if ( _x >= (long unsigned int) LOCAL_MAP_SIZE*world.nX
		|| _y >= (long unsigned int) LOCAL_MAP_SIZE*world.nY )
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
	if ( !data )
	{ return; }

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
		{ vVisibleTiles->push(new HasXY (_x1,_y1) ); }
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
		{ vVisibleTiles->push(new HasXY (_x1,_y2) ); }
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
		{ vVisibleTiles->push(new HasXY (_x2,_y1) ); } 
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
	if ( !data )
	{ return 0; }
	auto xy = new HasXY ( Random::randomInt(nX-1), Random::randomInt(nY-1) );
	return xy;
}

HasXY* World_Local::getRandomNeighbor(HasXY* _source)
{
	if ( !data )
	{ return 0; }
	if (isSafe(_source->x,_source->y) == false)
	{ return 0; }

	Vector <HasXY*> * vN = data->aLocalTile.getNeighborsOrthogonal(_source->x,_source->y,false,true);

	if ( vN!=0 && vN->size()>0 )
	{
		HasXY * returnValue = new HasXY((*vN)(0));
		//returnValue->set((*vN)(0));
		vN->clearData();
		delete vN;
		return returnValue;
	}
	if ( vN !=0 )
	{ delete vN; }

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
	return false;
	// if ( !data ) { return true; }
	// return data->aLocalTile(_x,_y).hasViewBlocker();

	// // statics
	// if ( data->aLocalTile(_x,_y).objStatic != 0 )
	// { return false; }
	// return true;
}

// Increments the map by nTicks ticks. Higher values may lead to abstraction.
void World_Local::incrementTicks(int nTicks)
{
	if ( !data )
	{ return; }

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
	return;

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
	if ( tribe == 0 )
	{ return; }

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
	if ( tribe == 0 )
	{ return; }

	// Note that std::map will automatically initialise new entries.
	mInfluence[tribe] -= amount;

	if ( mInfluence[tribe] < 0 )
	{ mInfluence[tribe] = 0; }

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
		if (elem.second < 0)
		{ elem.second = 0; }
	}
}

void World_Local::destroyInfluence (Tribe* tribe)
{
	if ( tribe == 0 )
	{ return; }

	mInfluence.erase(tribe);
}

Tribe* World_Local::getDominantInfluence ()
{
	if ( mInfluence.empty() || (--mInfluence.end())->second<=0 )
	{ return 0; }
	// Maps are sorted from smallest to largest, so we simply need to return the last key.

	//std::cout<<"Val is :"<<(--mInfluence.end())->second<<".\n";
	//return 0;

	return (--mInfluence.end())->first;
}

int World_Local::getDominantInfluenceValue ()
{
	if ( mInfluence.empty() )
	{ return 0; }
	// Maps are sorted from smallest to largest, so we simply need to return the last key.
	return (--mInfluence.end())->second;
}


std::string World_Local::getTerrainName()
{
	return biomeName[baseBiome];
}

bool World_Local::isLand()
{
	return (baseBiome!=OCEAN);
}

Texture* World_Local::currentTexture()
{
	if ( active )
	{ return &texFar; }


	//enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
	if ( baseBiome == NOTHING )
	{
		return &TEX_WORLD_TEST_00;
	}
	else if (baseBiome == OCEAN )
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
	else if (baseBiome == ICE)
	{
		return &TEX_WORLD_TERRAIN_ICE;
	}
	else if (baseBiome == SNOW)
	{
		return &TEX_WORLD_TERRAIN_SNOW;
	}
	else if (baseBiome == STEPPES)
	{
		return &TEX_WORLD_TERRAIN_STEPPE;
	}
	else if (baseBiome == WETLAND)
	{
		return &TEX_WORLD_TERRAIN_SWAMP;
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
