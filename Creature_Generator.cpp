#pragma once
#ifndef WORLDSIM_CREATURE_GENERATOR_CPP
#define WORLDSIM_CREATURE_GENERATOR_CPP

/* WorldSim: Creature_Generator
  #include "Creature_Generator.cpp"
  
  Implementation of Creature_Generator.hpp
*/

#include "Creature_Generator.hpp"

#include "Creature_Species.hpp"

#include <Container/WordList/WordList.hpp>
#include <File/FileManager.hpp>

#include <File/WTFManager.hpp> // loading raws


	Creature_Generator::Creature_Generator()
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
	
	// load the raws data from raw file
	void Creature_Generator::loadRaw(std::string strRaw)
	{
		std::cout<<"Creature generator: Getting raw.\n";
		WTFManager wtfManager;
		wtfManager.parse(strRaw);
		
		// for now we'll simply get the species names.
		
		// alternate: get all species
		Vector <WTFNode*>* vNode = wtfManager.getAllSub("CREATURE_SPECIES");
		for (int i=0;i<vNode->size();++i)
		{
			//Vector <std::string> * vRGB = (*vNode)(i)->getValues("RGB");
			//std::string colourName = (*vNode)(i)->getValue("NAME");
			
			// if (vRGB->size() == 3 && colourName != "")
			// {
				// colourManager.makeColour(DataTools::toInt( (*vRGB)(0) ) ,DataTools::toInt( (*vRGB)(1) ),
					// DataTools::toInt( (*vRGB)(2) ),colourName);
			// }
			// else
			// {
				// std::cout<<"Error: Colours are wrong\n";
			// }
			
			std::string speciesName = (*vNode)(i)->getValue("NAME_SINGULAR");
			
			std::cout<<"Adding creature: "<<speciesName<<"\n";
			
			Creature_Species * species = new Creature_Species(speciesName,1);
			vSpecies.push(species);
			
			
		}
		//std::cout<<"Colours built:\n"<<colourManager.toString()<<"\n";
	}
	
	std::string Creature_Generator::generateName()
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
	Vector <Creature_Species*> * Creature_Generator::generate(int amount)
	{		
		std::cout<<"Generate creature\n";
		Vector <Creature_Species*>* vSpecies2 = new Vector <Creature_Species*>;
		
		int currentPoints=1000;
		for (int i=0;i<amount;++i)
		{
			
			// randomly pick base creature.
			Creature_Species * base = vSpecies.getRandom(globalRandom);
			std::cout<<"Creature picked: "<<base->name<<"\n";
		
		
			Creature_Species * creature = new Creature_Species(generateName(),currentPoints);
			vSpecies2->push(creature);
			currentPoints/=2;
		}
		return vSpecies2;
	}
	
	// generate and return a single creature type
	// note this is Flora type, not object.
	Creature_Species * Creature_Generator::get(const int spawnWeighting/*=0*/)
	{
		// randomly pick base creature.
		Creature_Species * base = vSpecies.getRandom(globalRandom);
		//std::cout<<"Creature picked: "<<base->name<<"\n";
		//base->name += " " + generateName();
		return new Creature_Species(base->name + " " + generateName(),spawnWeighting);
	}

#endif
