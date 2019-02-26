#pragma once
#ifndef WORLDSIM_ITEM_HPP
#define WORLDSIM_ITEM_HPP

/* Item.hpp
	#include"Item.hpp"
  
  An item is an object which can be picked up and manipulated. Items are reliant on
  actors (Creatures/Characters) to be moved or used.
  
  Items cannot interfere with movement or line of sight.
  
  In the future stacking should be implemented for identical objects.
  
  Only items can be stored in an inventory.

*/

#include "Item.hpp"

Item::Item()
{
  count = 0;
  type = 0;
  meleeDamage = 0;
  throwDamage = 0;
  ammunitionType = 0;
  
  owner = 0;
  

}
Item::~Item() {}


std::string Item::getName() { return "Item"; }
std::string Item::getExtendedInfo() { return "N/A"; }

Texture* Item::currentTexture() { return 0; }

#include "Item_All.cpp"

#endif
