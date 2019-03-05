#pragma once
#ifndef WORLDSIM_RECIPE_HPP
#define WORLDSIM_RECIPE_HPP

/* Recipe.hpp
	#include "Recipe.hpp"
  
A recipe is any collection of objects which can be consumed to make another object. Usually there are three types
of recipes: Alchemy, crafting and cooking. I think most inputs/outputs will be items. But hey you never know.

*/


//template <class T_OBJECT_TYPE>
class Recipe
{
  public:
  
  Vector <WorldObject*> vInput;
  Item* output;
  
  Recipe();
  ~Recipe();
  
  /* Add a requirement to make this recipe. count is how many of the object you need. isConsumed true if the items
  are removed from the world when you make this recipe */
  void addRequirement(WorldObject* _object, int count, bool isConsumed);
  void addRequirement(Item* _object, int count, bool isConsumed);
  
};

#endif
