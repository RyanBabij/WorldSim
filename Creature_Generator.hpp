#pragma once
#ifndef WORLDSIM_CREATURE_GENERATOR_HPP
#define WORLDSIM_CREATURE_GENERATOR_HPP

/* WorldSim: Creature_Generator
  #include "Creature_Generator.hpp"
  
  Class to generate new Creature_Species.
*/

class Creature_Generator
{
	// It might be better practise to have a more centralised name generator which can handle all the different
	// names to generate, for example Flora, cities, people, etc. However there is still a lot to implement here
	// eg alchemy effects, before I'll look at doing that.
	WordList wlistFloraPattern;
	WordList wlistFloraColour;
	WordList wlistFloraAdjective;
	WordList wlistFloraNoun;
	// For testing
	WordList wlistCreatureType;
	
	public:
	Creature_Generator()
	{
		// Load word lists
		wlistFloraColour.loadString(FileManager::getFileAsString("raw/wordlists/flora_colours.txt"));
		wlistFloraPattern.loadString(FileManager::getFileAsString("raw/wordlists/flora_patterns.txt"));
		wlistFloraAdjective.loadString(FileManager::getFileAsString("raw/wordlists/flora_adjectives.txt"));
		wlistFloraNoun.loadString(FileManager::getFileAsString("raw/wordlists/flora_nouns.txt"));

		RNG_TEST.seed(SEEDER);
		
		wlistCreatureType.add("Wolf");
		wlistCreatureType.add("Deer");
		wlistCreatureType.add("Pig");
		wlistCreatureType.add("Sheep");
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
		_name+=wlistCreatureType.getRandom();
		return _name;
	}
	
	// Flora generation.
	// Name is randomly generated from wordlists.
	// Current system makes each additional Flora half as common as the previous.
	Vector <Creature_Species*> * generate(int amount)
	{
		Vector <Creature_Species*>* vSpecies = new Vector <Creature_Species*>;
		
		int currentPoints=1000;
		for (int i=0;i<amount;++i)
		{
			Creature_Species * creature = new Creature_Species(generateName(),currentPoints);
			vSpecies->push(creature);
			currentPoints/=2;
		}
		return vSpecies;
	}
	
	// generate and return a single flora type
	// note this is Flora type, not object.
	Creature_Species * get(const int spawnWeighting=0)
	{
		return new Creature_Species(generateName(),spawnWeighting);
	}
};
Creature_Generator creatureGenerator;

#endif
