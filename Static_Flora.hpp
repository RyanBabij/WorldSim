#pragma once
#ifndef WORLDSIM_STATIC_FLORA_HPP
#define WORLDSIM_STATIC_FLORA_HPP

/* WorldSim: Static_Flora
   #include "Static_Flora.hpp" */

/**
	Flora are plants which can be found in the different Biomes of the World. Flora has multiple purposes:
		* Food for Creatures
		* Harvestable materials
		* Harvestable ingredients
		* Harvestable alchemy ingredients

	Each flora will be randomly generated.

	Abstract flora plan.

	alternatively: flora ID (0-255) -> lookup table.
	flora stats update once per 24 hours on average.
	flora growth/death won't be modelled when player is absent because it won't be noticable.
	each biome gets its own list of flora

	flora should replenish to max amount every 7 days or so, depending on scaling.

	flora generation:
	each biome will randomly generate its own unique flora
	in future flora could be given names based on various attributes

	flora may be pickable and/or choppable

	Flora is a little bit complicated because each Flora may have unique characteristics, but mostly will draw from a 
	static class. I will probably give Flora objects a Package which will store any unique characteristics.
*/

/*
	Types of Flora classes:
	
	Flora - Each object represents a species of Flora.
	Flora_Instance - Represents an individual instance of a Flora species.
	Flora_Abstract - Same as above, but storing only the minimum amount of data, most likely simply a pointer to Flora.
	
	During generation the world is populated with Flora_Abstract, and then when interacted with or viewed, it is
	converted into a Flora_Instance with a generate() function.
	
	A Flora_Instance is visible on the World and can be interacted with. If a creature interacts with a Flora_Instance,
	it recieved a value which represents energy gained from eating it. If a Character interacts with the
	Flora_Instance, it recieves Ingredients which can then be used for eating/cooking/crafting/alchemy.

	Harvesting may be done by hand, or with a tool such as a knife.

	Flora are a type of Static and therefore there may be maximum 1 Flora_Instance per tile and are stored in the
	statics file for the map.
	
	Statics will probably be compressed to a byte lookup table. I think it's unlikely there will ever need to be more
	than about 16 Flora per Biome, so it shouldn't be a problem reserving this much space for Flora.
	
	Trees should probably be regarded largely the same as Flora, with the exeption they may block movement/LOS. As Flora may be harvested with tools, so may Trees be harvested, except of course they need something like an Axe.'
	
	For simplicity, a Tree shouldn't return anything except wood products, basically logs or bark. Complications would otherwise arise about whether something can or cannot be reached.
	
	Unlike with other Flora, there are a fixed number of Tree types, once again for simplicity. Each Biome probably gets 2 or 3 unique Tree types which are common across all Biomes of that type. Trees cannot be partially chopped or damaged so they can easily be stored as a Static lookup ID.
*/


// Consolidates all Flora info for a biome to save space.
// There are max 255 Flora per biome therefore a lookup table is more efficient than a pointer.
// flora should just be in a map of values... easy, medium, hard, and current values of each.
// 4 bits per value should suffice, but for now we might just go with 1 byte per value

// 1 creature may occupy 1 space.
// (this is resolved for the player and NPCs by allowing people to swap their position with another person)

// Each map loaded should have an array of values for the flora

// to simplify things a bit better, a flora will have a single type of food (hard, medium, easy)
// all flora will prodide the same nutrition for each type. Additionally replenishment can be random. This means only the flora type needs to be stored on the array, a single value from 1-3.

// 00000000 - 111 - Type (multiple/none allowed). 111 - Eaten/not eaten.
// Flora will randomly fully replenish.
// Extra 2 bits reserved for trees (extra food type and movement blocking)
// 1 bit symbolises trees (movement blockers), and the final bit represents if a creature is on the tile.

// 00000010 - tree
// 00000001 - creature is here
// 11111111 - natural feature blocking movement (creature may not be on tree) 

// grazers - can live off grass (any non-blocked tile, as a final elemant)
// speed/energy tradeoff allows multiple types of creatures to survive. Slower creatures get more food for less, but faster creatures can get there first.
// creatures breed by bring adjacent with at over 50% energy. Breeding brings energy down to 50%. Old age is not simulated.

// more complex ai can be added over time.
// Replenishment gives a creature X turns to keep living. This will also occur in full-mode in a similar way..
// Therefore each biome can run flora/wildlife simulation with 1 byte per tile ( 



// class FloraAbstract: public Static
// {
	// public:
	// // this only tracks food amount and for everything else uses a lookup on the Flora type
	// unsigned char easyFood, mediumFood, hardFood;
	
	// Flora* lookup;
	// //unsigned char biomeLookup;


	// FloraAbstract()
	// {
	// }
// };

// class FloraDynamic
// {
	// public:
	
	// FloraDynamic()
	// {
	// }
// };

// This class sets the stats for a type of flora. Individual instances should use another class.
#include <Container/Table/TableInterface.hpp>
#include <Container/WordList/WordList.hpp> // for random flora names

// ingredient effects
// realistically we will probably just have a hardcoded table of effects
class Effect
{
	public:
	std::string name;
	
	Effect(std::string _name)
	{
		name=_name;
	}
};

class EffectGenerator
{
	WordList wListEffects;
	public:
	
	EffectGenerator()
	{
		wListEffects.add("Death");
		wListEffects.add("Poison");
		wListEffects.add("Health");
		wListEffects.add("Magic");
	}
	
	Effect * generate()
	{
		auto effect = new Effect(wListEffects.getRandom());
		return effect;
	}
};
EffectGenerator effectGenerator;


// Flora will probably refer to both instances and types, however maybe not, in which case Flora will refer to
// types of Flora, and then Flora_Instance will refer to actual Flora instances. We can then also differentiate
// instances into Flora_Abstract, which will probably just be a pointer to the Flora species until generated properly.
// A Flora instance is necessary even if it simply tracks the time it was last harvested.

// Wildlife simulation will be greatly simplified and therefore there will not be a need ot provide different food
// values for flora. They can have a single caloric value per harvest, and optionally return an ingredient.

class Ingredient
{
	public:
	std::string name;
	Effect * effect;
	
	Ingredient(std::string _name)
	{
		name = _name;
		effect = effectGenerator.generate();
	}
};

class IngredientGenerator
{
	public:
	std::string name;
	
	WordList wListIngredient;
	
	IngredientGenerator()
	{
		// for now just give random name from small list for testing
		// Load word lists
		wListIngredient.loadString(FileManager::getFileAsString("raw/wordlists/ingredient.txt"));
		RNG_TEST.seed(SEEDER);
	}
	
	Ingredient * generateIngredient()
	{
		auto ingredient = new Ingredient(wListIngredient.getRandom());
		
		//ingredient->name=wListIngredient.getRandom();
		
		std::cout<<"Generated ingredient: "<<ingredient->name<<"\n";
		
		return ingredient;
	}

};

IngredientGenerator ingredientGenerator;

// todo: track base biome and allow migration between biomes (mutation of 1 trait)

#include "World_Biome.hpp"

class Flora: public Static, public TableInterface
{
   // Easy food: Food which can easily be obtained/eaten such as fruit
   // Medium food: Food which is moderately easy to obtain/eat such as bark
   // Hard food: Food which is hard to obtain/eat such as leaves
   // This should allow a balance of large and slow, and small and fast herbivores
   // Increment is per 24 hours
   // 1 food = 1 day of food for 1 herbivore
   unsigned char easyFood, mediumFood, hardFood;
	
	
   
   //Vector <unsigned char> vAllowedBiomeTypes;
   
   public:
	Ingredient * ingredient; // Each Flora may contain 1 ingredient.
	World_Biome * biome; // biome in which this Flora may spawn (Flora are limited to their biome)
   
   unsigned short int spawnWeight; // probability of being selected to spawn. higher = more common
	
	Colour colour; // The base colour of the Flora (ingredients may also have their own colour)
   
   Flora(const std::string _name = "Flora", const unsigned short int _spawnWeight=1);
	
   void increment(unsigned short int /* nDays */);
   void setFoodValues(unsigned char /* _maxEasy */, unsigned char /* _maxMedium */, unsigned char /* _maxHard */);
   void allowBiome(unsigned char /* biomeType */);
	
	std::string getColour();
	
   virtual Texture* currentTexture() override;
	
	// TableInterface
	std::string getColumn(std::string _column) override;
	// TableInterface
	std::string getColumnType(std::string _column) override;
	
};


// Trees will be special-case flora because they can block LOS and movement.
class Static_Tree: public Flora
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


#include "Static_FloraManager.hpp"
#include "Static_FloraGenerator.hpp"
#include "Static_FloraGenerator.cpp"
FloraGenerator floraGenerator;
#include "Static_FloraManager.cpp"

#include "Static_Flora.cpp"




#endif
