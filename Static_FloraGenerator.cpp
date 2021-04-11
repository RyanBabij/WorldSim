#pragma once
#ifndef WORLDSIM_STATIC_FLORA_GENERATOR_CPP
#define WORLDSIM_STATIC_FLORA_GENERATOR_CPP

/* WorldSim: Static_FloraGenerator
   #include "Static_FloraGenerator.cpp"

	Implementation of Static_FloraGenerator.hpp
	
*/

#include "Static_FloraGenerator.hpp"

#include <Container/WordList/WordList.hpp> // for random flora names

FloraGenerator::FloraGenerator()
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

std::string FloraGenerator::generateName()
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
Vector <Flora*> * FloraGenerator::generate(int amount)
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
Flora* FloraGenerator::get(const int spawnWeighting /* =0 */)
{
	return new Flora(generateName(),spawnWeighting);
}

#endif
