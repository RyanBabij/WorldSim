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
   
   unsigned short int spawnWeight; // probability of being selected to spawn. higher = more common
   
   Flora(const std::string _name = "Flora", const unsigned short int _spawnWeight=1)
   {
      name=_name;
      spawnWeight=_spawnWeight;
		ingredient = ingredientGenerator.generateIngredient();
   }
   void increment(unsigned short int nDays)
   {
      // for (int i=0;i<nDays;++i)
      // {
         // const unsigned short int newEasy = easyFood+easyFoodIncrement;
         // if ( newEasy > 255 ) { easyFood = 255; }
         // else { easyFood = newEasy; }
         // if ( easyFood > maxEasyFood ) { easyFood = maxEasyFood; }

         // const unsigned short int newMedium = mediumFood+mediumFoodIncrement;
         // if ( newMedium > 255 ) { mediumFood = 255; }
         // else { mediumFood = newMedium; }
         // if ( mediumFood > maxMediumFood ) { mediumFood = maxMediumFood; }
         
         // const unsigned short int newHard = hardFood+hardFoodIncrement;
         // if ( newHard > 255 ) { hardFood = 255; }
         // else { hardFood = newHard; }
         // if ( hardFood > maxHardFood ) { hardFood = maxHardFood; }
      // }
   }
   void setFoodValues(unsigned char _maxEasy, unsigned char _maxMedium, unsigned char _maxHard)
   {
      easyFood = _maxEasy;
      mediumFood = _maxMedium;
      hardFood = _maxHard;
   }
   void allowBiome(unsigned char biomeType)
   {
      //vAllowedBiomeTypes.push(biomeType);
   }
   virtual Texture* currentTexture() override
   {
      return &TEX_FLORA_PLANT;
   }
   
	// TableInterface
	std::string getColumn(std::string _column) override
	{
		if ( _column=="name" )
		{
			return name;
		}
		// else if ( _column=="size" )
		// {
			// return DataTools::toString(size);
		// }
		// else if ( _column=="type" )
		// {
			// return WorldGenerator2::biomeName[type];
		// }
		return "?";
	}
	// TableInterface
	std::string getColumnType(std::string _column) override
	{
		// if ( _column == "size" )
		// {
			// return "number";
		// }
		return "string";
	}
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

class FloraGenerator
{
	// It might be better practise to have a more centralised name generator which can handle all the different
	// names to generate, for example Flora, cities, people, etc. However there is still a lot to implement here
	// eg alchemy effects, before I'll look at doing that.
	WordList wlistFloraPattern;
	WordList wlistFloraColour;
	WordList wlistFloraAdjective;
	WordList wlistFloraNoun;
	// For testing
	WordList wlistFloraType;
	
	public:
	FloraGenerator()
	{
		// Load word lists
		wlistFloraColour.loadString(FileManager::getFileAsString("raw/wordlists/flora_colours.txt"));
		wlistFloraPattern.loadString(FileManager::getFileAsString("raw/wordlists/flora_patterns.txt"));
		wlistFloraAdjective.loadString(FileManager::getFileAsString("raw/wordlists/flora_adjectives.txt"));
		wlistFloraNoun.loadString(FileManager::getFileAsString("raw/wordlists/flora_nouns.txt"));

		RNG_TEST.seed(SEEDER);
		
		wlistFloraType.add("Tree");
		wlistFloraType.add("Plant");
		wlistFloraType.add("Herb");
		wlistFloraType.add("Mushroom");
		wlistFloraType.add("Fungus");
		wlistFloraType.add("Cap");
		wlistFloraType.add("Root");
		wlistFloraType.add("Wort");
		wlistFloraType.add("Leaf");
		wlistFloraType.add("Fruit");
		wlistFloraType.add("Flower");
		wlistFloraType.add("Weed");
		wlistFloraType.add("Grass");
		wlistFloraType.add("Bean");
		wlistFloraType.add("Sprout");
		wlistFloraType.add("Fern");
		wlistFloraType.add("Frond");
		wlistFloraType.add("Palm");
		wlistFloraType.add("Nut");
		wlistFloraType.add("Seed");
		wlistFloraType.add("Shrub");
		wlistFloraType.add("Bush");
		wlistFloraType.add("Nettle");
		wlistFloraType.add("Slime");
		wlistFloraType.add("Essence");
	}
	
	std::string generateName()
	{
		std::string _name="";
		if (RNG_TEST.flip()) // use colour
		{
			_name+=wlistFloraColour.getRandom()+" ";
		}
		if (RNG_TEST.flip()) // use pattern
		{
			_name+=wlistFloraPattern.getRandom()+" ";
		}
		if (RNG_TEST.flip()) // use adjective
		{
			_name+=wlistFloraAdjective.getRandom()+" ";
		}
		if (RNG_TEST.flip() || _name.size() == 0) // use noun
		{
			_name+=wlistFloraNoun.getRandom()+" ";
		}
		_name+=wlistFloraType.getRandom();
		return _name;
	}
	
	// Flora generation.
	// Name is randomly generated from wordlists.
	// Current system makes each additional Flora half as common as the previous.
	Vector <Flora*> * generate(int amount)
	{
		Vector <Flora*>* vFlora = new Vector <Flora*>;
		
		int currentPoints=1000;
		for (int i=0;i<amount;++i)
		{
			Flora * flora = new Flora(generateName(),currentPoints);
			vFlora->push(flora);
			currentPoints/=2;
		}
		return vFlora;
	}
	
	// generate and return a single flora type
	// note this is Flora type, not object.
	Flora* get(const int spawnWeighting=0)
	{
		return new Flora(generateName(),spawnWeighting);
	}
};
FloraGenerator floraGenerator;



class FloraManager
{
	public:
	
	int totalSpawnWeight;
	
	// All flora types in the biome (max 255 types)
	Vector <Flora*> vFlora;
	
	RandomLehmer rng;
	
	// All abstracted flora in the map
	//Vector <FloraAbstract*> vFloraAbstract;
	
	FloraManager()
	{
		totalSpawnWeight=0;
		rng.seed(SEEDER);
	}
	
	void generate (const int amount=1)
	{
		//std::cout<<"FloraManager generating "<<amount<<" flora.\n";
		int currentPoints=1000;
		for (int i=0;i<amount;++i)
		{			
			vFlora.push(floraGenerator.get(currentPoints));
			totalSpawnWeight+=currentPoints;
			currentPoints/=2;
		}
	}
	
	// return a random flora from the weighted lists
	Flora* get()
	{
		if (vFlora.size() == 0 )
		{
			return 0;
		}
		if ( vFlora.size() == 1 )
		{
			return vFlora(0);
		}
		
		int floraSlot = rng.rand32(totalSpawnWeight-1);
		
		int currentWeighting = 0;
		for (int i=0;i<vFlora.size();++i)
		{
			currentWeighting+=vFlora(i)->spawnWeight;
			
			if ( currentWeighting >= floraSlot )
			{
				//std::cout<<"Spawning: "<<vFlora(i)->name<<"\n";
				return vFlora(i);
			}
		}
		
		return 0;
		
		
	}
};


#endif
