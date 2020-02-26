#pragma once
#ifndef WORLDSIM_WORLD_BIOME_HPP
#define WORLDSIM_WORLD_BIOME_HPP

/* WorldSim: World_Biome.hpp
	#include "World_Biome.hpp"

  Stores data for the various biomes in the world.
  Useful for biome-specific simulation.

*/


#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>

class World_Biome: public TableInterface
{
	public:
	
	std::string name;
	unsigned int size; /* size in tiles */
	unsigned char type; /* Biome type */
   
   Vector <HasXY> vXY; // every tile coordinate of this biome.
   
   // placeholders for objects
   Vector <std::string> vFlora;
   Vector <std::string> vHerbivore;
   Vector <std::string> vCarnivore;
	
	World_Biome()
	{
		name="";
		size=0;
	}
	
	virtual ~World_Biome()
	{
	}
	
	//  NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13
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
	std::string getColumnType(std::string _column)
	{
		if ( _column == "size" )
		{
			return "number";
		}
		return "string";
	}
   
   void generate(unsigned int _size, unsigned char _type)
   {
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
      
      RandomLehmer rng;
      rng.seed(globalRandom.rand8());
      
      unsigned int minFlora = sqrt(size)/1.5;
      if ( minFlora < 3 ) { minFlora=3; }
      
      unsigned int floraAmount = minFlora+rng.rand32(1+(sqrt(size)/1.5));
      
      if (floraAmount < 3) { floraAmount=3; }
      if (floraAmount > 255) { floraAmount = 255; }
      
      std::cout<<"Biome size: "<<size<<" generating "<<floraAmount<<" flora.\n";
      
      // generate rarities. A 3-way split would probably be fine.
      // ID 0 is always the primary flora for the biome, typically a tree
      // ID 1 is always some kind of plant
      // and ID 2 is always some kind of rare plant.
      // The rest should be random.
      
      std::cout<<"Flora names:\n";
      
      if (type == DESERT)
      {
         std::cout<<"   Cactus\n";
         std::cout<<"   shrub\n";
         std::cout<<"   plant\n";
      }
      else if (type == SNOW)
      {
         std::cout<<"   Fir tree\n";
         std::cout<<"   bush\n";
         std::cout<<"   plant\n";
      }
      else if (type == WETLAND)
      {
         std::cout<<"   Willow\n";
         std::cout<<"   plant\n";
         std::cout<<"   plant\n";
      }
      else
      {
         std::cout<<"   Tree\n";
         std::cout<<"   common plant\n";
         std::cout<<"   uncommon plant\n";
      }
      

      
      for (unsigned int i=3;i<floraAmount;++i)
      {
         std::string floraName = "";
         
         int common = rng.rand8()%3;
         
         if (common==0)
         {
            std::cout<<"   common flora\n";
         }
         else if (common==1)
         {
            std::cout<<"   uncommon flora\n";
         }
         else
         {
            std::cout<<"   rare flora\n";
         }
      }
      
   }
};

#endif