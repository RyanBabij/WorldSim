#pragma once
#ifndef WORLDSIM_RECIPE_CPP
#define WORLDSIM_RECIPE_CPP

/* Recipe.cpp
	#include "Recipe.cpp"
  
Implementation of Recipe.hpp

*/

#include "Recipe.hpp"

Recipe::Recipe()
{
  vInput.push(new Item_Plank);
  vInput.push(new Item_Sword);
}
Recipe::~Recipe()
{
}

void Recipe::addRequirement(WorldObject* _object, int count, bool isConsumed)
{
}
void Recipe::addRequirement(Item* _object, int count, bool isConsumed)
{
}

#endif