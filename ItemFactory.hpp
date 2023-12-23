#pragma once
#ifndef WORLDSIM_ITEM_FACTORY_HPP
#define WORLDSIM_ITEM_FACTORY_HPP

#include "Item.hpp"

/* Worldsim: ItemFactory.hpp
	#include"ItemFactory.hpp"

	Class to produce items based on Character stats and equipment used to make item.

*/

class ItemFactory
{
	public:
		ItemFactory();

		// Produce the item with stats relevant based on character skill, location, and tools used.
		Item* produce(ItemType type, Character* character, Location* location /* TOOL */);

		// Produce worst guaranteed item with the given factors
		Item* produceBasic(ItemType type, Character* character, Location* location /* TOOL */);
};

#endif // WORLDSIM_ITEM_FACTORY_HPP
