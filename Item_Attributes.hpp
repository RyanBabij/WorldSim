#pragma once
#ifndef WORLDSIM_ITEM_ATTRIBUTES_HPP
#define WORLDSIM_ITEM_ATTRIBUTES_HPP

/* Item_Attributes.hpp
	#include"Item_Attributes.hpp"

	Container for attributes of an item. Includes tools for comparing with other items.

*/

class Job;

class Item_Attributes
{
	public:
	
		int durability; // How many actions it can perform
		int health; // How many actions left it can perform
	
		int woodcuttingValue;
		int farmingValue;
		int miningValue;
		int huntingValue;
		
		Item_Attributes();
		
		// Item must meet all minimum requirements
		int suitability(Job* job);
		
		bool degrade (int amount); // Degrade the item through activity. Returns true if the item broke.
};

#endif // WORLDSIM_ITEM_ATTRIBUTES_HPP
