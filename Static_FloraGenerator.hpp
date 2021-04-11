#pragma once
#ifndef WORLDSIM_STATIC_FLORA_GENERATOR_HPP
#define WORLDSIM_STATIC_FLORA_GENERATOR_HPP

/* WorldSim: Static_FloraGenerator
   #include "Static_FloraGenerator.hpp"

	Class to generate random Flora within certain parameters.
	
	Subfunctionality of Biome objects.
	
*/

class Flora;
class WordList;

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
	FloraGenerator();
	
	std::string generateName();
	
	// Flora generation.
	// Name is randomly generated from wordlists.
	// Current system makes each additional Flora half as common as the previous.
	Vector <Flora*> * generate(int amount);
	
	// generate and return a single flora type
	// note this is Flora type, not object.
	Flora* get(const int spawnWeighting=0);
};


#endif
