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


// LEATHER CLOTHES
void Recipe_LeatherClothes::make(Character* _character)
{
  auto _output = new Item_LeatherClothes;
  _character->giveItem(_output);
}

// Waterskin
void Recipe_Waterskin::make(Character* _character)
{
  auto _output = new Item_Waterskin;
  _character->giveItem(_output);
}

// Log shelter
void Recipe_LogShelter::make(Character* _character)
{
  auto _output = new Item_Shelter;
  _character->giveItem(_output);
}

#endif
