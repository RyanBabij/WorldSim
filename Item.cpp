#pragma once
#ifndef WORLDSIM_ITEM_CPP
#define WORLDSIM_ITEM_CPP

/* Item.hpp
	#include"Item.hpp"
  
  Implementation of Item.hpp
*/

#include "Item.hpp"

Item::Item()
{
	information=0;
	
  count = 0;
  type = 0;
  meleeDamage = 0;
  throwDamage = 0;
  ammunitionType = 0;
  
  reach = 0;
  
  owner = 0;
  
  consumeTime=-1;
  hungerRestore=-1;
  
  canCook=false;
}
Item::~Item() {}


std::string Item::getName() { return "Item"; }
std::string Item::getExtendedInfo() { return "N/A"; }

Texture* Item::currentTexture() { return 0; }

void Item::addToRecipeManager()
{
  recipeManager.addToRecipes(this);
}

std::string Item::getSaveData()
{
  std::string saveData = getName();
  

  return saveData;
}
void Item::loadData(std::string _saveData)
{
}

/* TABLE INTERFACE */

std::string Item::getColumn(std::string _column)
{
	if ( _column == "name" )
	{
		return getName();
	}
	else if ( _column == "quality" )
	{
		return getQuality();
	}
	else if ( _column == "year" )
	{
		if (information==0)
		{
			return "-1";
		}
		return DataTools::toString(information->yearMade);
	}
	else if ( _column == "month" )
	{
		if (information==0)
		{
			return "-1";
		}
		return DataTools::toString(information->monthMade);
	}
	else if ( _column == "creation" )
	{
		if (information==0)
		{
			return "Unknown";
		}
		return "Y " + DataTools::toString(information->yearMade)+" M "+
		DataTools::toString(information->monthMade);
	}

	return "?";
}
std::string Item::getColumnType(std::string _column)
{
	if ( _column == "year" || _column == "month" )
	{
		return "number";
	}
	return "string";
}

std::string Item::getQuality()
{
	if (information==0)
	{
		return "Normal";
	}
	return "Fine";
}

#include "Item_All.cpp"

#endif
