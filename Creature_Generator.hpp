#pragma once
#ifndef WORLDSIM_CREATURE_GENERATOR_HPP
#define WORLDSIM_CREATURE_GENERATOR_HPP

/* WorldSim: Creature_Generator
  #include "Creature_Generator.hpp"
  
  Class to generate new Creature_Species.
*/

class Creature_Species;

#include <Container/WordList/WordList.hpp>

class Creature_Generator
{
	// It might be better practise to have a more centralised name generator which can handle all the different
	// names to generate, for example Flora, cities, people, etc. However there is still a lot to implement here
	// eg alchemy effects, before I'll look at doing that.
	
	//All these should become raws tbh
	WordList wlistFloraPattern;
	WordList wlistFloraColour;
	WordList wlistFloraAdjective;
	WordList wlistFloraNoun;
	// For testing
	WordList wlistCreatureType;
	
	// species templates from which to spin off creatures.
	Vector <Creature_Species*> vSpecies;
	
	public:
	Creature_Generator();
	
	// load the raws data from raw file
	void loadRaw(std::string strRaw);
	
	std::string generateName();
	
	// Flora generation.
	// Name is randomly generated from wordlists.
	// Current system makes each additional Flora half as common as the previous.
	Vector <Creature_Species*> * generate(int amount);
	
	// generate and return a single creature type
	// note this is Flora type, not object.
	Creature_Species * get(const int spawnWeighting=0);
};

#endif
