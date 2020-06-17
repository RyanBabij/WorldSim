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

	flora should replenish to max amount every 24 hours on average

	flora generation:
	each biome will randomly generate its own unique flora
	in future flora could be given names based on various attributes

	flora may be pickable and/or choppable

	Flora is a little bit complicated because each Flora may have unique characteristics, but mostly will draw from a 
	static class. I will probably give Flora objects a Package which will store any unique characteristics.
*/

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
   
   unsigned short int spawnWeight; // probability of being selected to spawn. higher = more common
   
   Flora(const std::string _name = "Flora", const unsigned short int _spawnWeight=1)
   {
      name=_name;
      spawnWeight=_spawnWeight;
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
   void setFoodValues(unsigned char _easyIncrement, unsigned char _maxEasy,
      unsigned char _mediumIncrement, unsigned char _maxMedium,
      unsigned char _hardIncrement, unsigned char _maxHard)
   {
      easyFood = _maxEasy;
      maxEasyFood = _maxEasy;
      easyFoodIncrement = _easyIncrement;
      
      mediumFood = _maxMedium;
      maxMediumFood = _maxMedium;
      mediumFoodIncrement = _mediumIncrement;
      
      hardFood = _maxHard;
      maxHardFood = _maxHard;
      hardFoodIncrement = _hardIncrement;
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

#include <Container/WordList/WordList.hpp> // for random flora names

class FloraGenerator
{
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
			// generate flora name.
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
			
			Flora * flora = new Flora(_name,currentPoints);
			vFlora->push(flora);
			currentPoints/=2;
		}
		return vFlora;
	}
};


#endif
