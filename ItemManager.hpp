#pragma once
#ifndef WORLDSIM_ITEM_MANAGER_HPP
#define WORLDSIM_ITEM_MANAGER_HPP

/* ItemManager.cpp
	#include"ItemManager.hpp"
  
	Class to manage collection of items, for example a Settlement stockpile.
	
*/

#include "Item.hpp"
#include "Job.hpp"

#include <map>
#include <string>
#include <iostream>


class ItemManager
{
	public:
	
	Vector <Item*> vItem;
	
	ItemManager()
	{
	}
	
	void add(Item* item)
	{
		vItem.push(item);
	}
	
	// When a Character takes an item from the stockpile
	void take(Item* item)
	{
		if (item==nullptr)
		{ return; }
		vItem.remove(item);
	}
	
	
	int getNumOfType(ItemType type)
	{
		int num=0;
		
		for (int i=0;i<vItem.size();++i)
		{
			if ( vItem(i)->type == type )
			{
				++num;
			}
		}
		
		return num;
	}
	
	Item* getBestItemFor(Job job)
	{
		return job.getBestItem(&vItem);
	}
	
	void print()
	{
		std::map<std::string, int> itemCounts;

		// Count the items
		for (int i = 0; i < vItem.size(); ++i)
		{
			Item* currentItem = vItem(i);
			if (currentItem != nullptr)
			{
				std::string itemName = currentItem->getName();
				itemCounts[itemName]++;
			}
		}

		// Print the counts
		std::cout << "Item Manager Contents:" << std::endl;
		for (const auto& pair : itemCounts)
		{
			std::cout << "Item: " << pair.first << "  x" << pair.second << std::endl;
		}
	}


};

#endif // WORLDSIM_ITEM_MANAGER_HPP