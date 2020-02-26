#pragma once
#ifndef WORLDSIM_STATIC_HPP
#define WORLDSIM_STATIC_HPP

/* WorldSim:Static.hpp
   #include "Static.hpp"

   Statics are objects which can't move on their own, and therefore
   they don't need to know their coordinates.
   They also only tick once per day.

   Static subclasses:
      Plant
      Tree
      Building
      Furniture

   Statics may block movement and/or LOS
*/

class Static: public HasTexture
{
   public:
   unsigned char blockMove; // bitfield blocks movement in direction starting NW and going clockwise
   unsigned char blockLOS; // bitfield blocks LOS in direction starting NW and going clockwise
   
   // HP
   // DAMAGE RESIST
   
   Static()
   {
      blockMove=0;
      blockLOS=0;
   }
   
   virtual Texture* currentTexture()
   {
      return 0;
   }
};

// object capable of holding Ingredients
class Harvestable
{
   public:
};


// Abstract flora plan.

// alternatively: flora ID (0-255) -> lookup table.
// flora stats update once per 24 hours on average.
// flora growth/death won't be modelled when player is absent because it won't be noticable.
// each biome gets its own list of flora

// flora should replenish to max amount every 24 hours on average

// flora generation:
// each biome will randomly generate its own unique flora
// in future flora could be given names based on various attributes

class Flora: public Static
{
   // Easy food: Food which can easily be obtained/eaten such as fruit
   // Medium food: Food which is moderately easy to obtain/eat such as bark
   // Hard food: Food which is hard to obtain/eat such as leaves
   // This should allow a balance of large and slow, and small and fast herbivores
   // Increment is per 24 hours
   // 1 food = 1 day of food for 1 herbivore
   unsigned char easyFood, maxEasyFood, easyFoodIncrement;
   unsigned char mediumFood, maxMediumFood, mediumFoodIncrement;
   unsigned char hardFood, maxHardFood, hardFoodIncrement;
   
   Vector <unsigned char> vAllowedBiomeTypes;
   
   public:
   Flora()
   {
   }
   void increment(unsigned short int nDays)
   {
      for (int i=0;i<nDays;++i)
      {
         const unsigned short int newEasy = easyFood+easyFoodIncrement;
         if ( newEasy > 255 ) { easyFood = 255; }
         else { easyFood = newEasy; }
         if ( easyFood > maxEasyFood ) { easyFood = maxEasyFood; }

         const unsigned short int newMedium = mediumFood+mediumFoodIncrement;
         if ( newMedium > 255 ) { mediumFood = 255; }
         else { mediumFood = newMedium; }
         if ( mediumFood > maxMediumFood ) { mediumFood = maxMediumFood; }
         
         const unsigned short int newHard = hardFood+hardFoodIncrement;
         if ( newHard > 255 ) { hardFood = 255; }
         else { hardFood = newHard; }
         if ( hardFood > maxHardFood ) { hardFood = maxHardFood; }
      }
   }
   void allowBiome(unsigned char biomeType)
   {
      vAllowedBiomeTypes.push(biomeType);
   }
   virtual Texture* currentTexture() override
   {
      return 0;
   }
};
// we would probably want to use a flora generator as it can handle rarity, attributes, balance etc.
// nearby biomes could probably also share the same or similar flora

// new idea: each abstract map gets a biome lookup ID.
// biome guarantees max 255 flora types, and therefore
// we can guarantee 1 byte to ID flora on abstract

class FloraGenerator
{
   public:
   
   Vector <Flora*> vFloraType;
   
   FloraGenerator()
   {
   }
   
   Flora* generate(unsigned char biomeType, unsigned char floratype)
   {
      return 0;
   }
   
};
FloraGenerator floraGenerator;

class Static_Tree: public Static
{
   public:
   
   int chopAmount;
   int growth;
   
   Static_Tree(int _growth=100)
   {
      chopAmount=100;
      growth=_growth;
      
      if ( growth > 0 )
      {
         blockMove = 255;
         blockLOS = 255;
      }
      else
      {
         blockMove = 0;
         blockLOS = 0;
      }
   }
   
   Texture* currentTexture() override
   {
      if (chopAmount==0)
      { return &TEX_OBJECT_STUMP;
      }

      if ( growth==0 )
      {
         return &TEX_WORLD_TERRAIN_FOREST_SAPLING;
      }
      return &TEX_WORLD_TERRAIN_FOREST_TREE;
   }
};

#endif
