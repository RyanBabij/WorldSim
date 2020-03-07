#pragma once
#ifndef WORLDSIM_WORLD_BIOME_HPP
#define WORLDSIM_WORLD_BIOME_HPP

/* WorldSim: World_Biome.hpp
	#include "World_Biome.hpp"

  Stores data for the various biomes in the world.
  Useful for biome-specific simulation.
  
  Map generation should really be done by biome,
  as should Wildlife and Creature simulation.

*/


#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>

class CreatureSpecies;

class World_Biome: public TableInterface
{
   RandomLehmer rng;
   
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
   Vector <CreatureSpecies*> vCreatureSpecies; // Vector of Creature types
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
   
   // generate entire biome including terrain, flora, etc.
   void generate()
   {
      if ( id == -1 )
      { return; }
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
   
   void generateCreatures()
   {
      // each biome should have at least a few herbivores, and at least 1 carnivore if they can be supported.
      
      
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
};

#endif