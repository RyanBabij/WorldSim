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
	
		int woodcuttingValue;
		int farmingValue;
		int miningValue;
		
		Item_Attributes();
		
		// Item must meet all minimum requirements
		int suitability(Job* job);
};

#endif // WORLDSIM_ITEM_ATTRIBUTES_HPP
