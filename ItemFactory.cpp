#pragma once
#ifndef WORLDSIM_ITEM_FACTORY_CPP
#define WORLDSIM_ITEM_FACTORY_CPP

/* Worldsim: ItemFactory.cpp
	#include"ItemFactory.cpp"
*/

#include "ItemFactory.hpp"

ItemFactory::ItemFactory()
{
}

// Produce the item with stats relevant based on character skill, location, and tools used.
Item* ItemFactory::produce(ItemType type, Character* character, Location* location /* TOOL */)
{
	return nullptr;
}

// Produce worst guaranteed item with the given factors
Item* ItemFactory::produceBasic(ItemType type, Character* character, Location* location /* TOOL */)
{
	if ( type == ITEM_HOE )
	{
		return new Item_Hoe();
	}
	else if (type == ITEM_SWORD )
	{
		return new Item_Sword();
	}
	else if (type == ITEM_KNIFE )
	{
		return new Item_Knife();
	}
	else if (type == ITEM_LONGBOW )
	{
		return new Item_Longbow();
	}
	else if (type == ITEM_SPEAR )
	{
		return new Item_Spear();
	}
	else if (type == ITEM_PICKAXE )
	{
		return new Item_Pickaxe();
	}
	else if (type == ITEM_AXE )
	{
		return new Item_Axe();
	}
	
	return nullptr;
}

#endif // WORLDSIM_ITEM_FACTORY_CPP
