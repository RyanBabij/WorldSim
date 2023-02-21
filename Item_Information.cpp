#pragma once
#ifndef WORLDSIM_ITEM_INFORMATION_CPP
#define WORLDSIM_ITEM_INFORMATION_CPP

/* Item_Informatoin.cpp
	#include "Item_Information.hpp"
  
  The item information is an optional attachment to an item which contains information about the item's history and other information.

*/

class Settlement;

class Item_Information
{
	public:
	Character * creator;
	Character * owner;
	Settlement* locationMade;
	unsigned char quality; // 0-5. Items without information are assumed to be level 0.
	
	Item_Information()
	{
		creator=0;
		owner=0;
	}
};

#endif
