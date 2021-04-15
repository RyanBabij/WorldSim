#pragma once
#ifndef WORLDSIM_INGREDIENT_HPP
#define WORLDSIM_INGREDIENT_HPP

/* WorldSim: Ingredient
   #include "Ingredient.hpp"

	Flora will probably refer to both instances and types, however maybe not, in which case Flora will refer to
	types of Flora, and then Flora_Instance will refer to actual Flora instances. We can then also differentiate
	instances into Flora_Abstract, which will probably just be a pointer to the Flora species until generated properly.
	A Flora instance is necessary even if it simply tracks the time it was last harvested.

	Wildlife simulation will be greatly simplified and therefore there will not be a need ot provide different food
	values for flora. They can have a single caloric value per harvest, and optionally return an ingredient.

	
*/


class Ingredient
{
	public:
	std::string name;
	Effect * effect;
	ColourRGB <unsigned char> colour; // colour of the ingredient for description purposes.
	
	Ingredient(std::string _name)
	{
		name = _name;
		effect = effectGenerator.generate();
		colour.setRandom(globalRandom);
	}
	std::string getColour()
	{
		return colourManager.getNameOfColour(colour);
	}
};

#endif
