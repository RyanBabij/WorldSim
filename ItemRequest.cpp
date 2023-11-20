#pragma once
#ifndef WORLDSIM_ITEM_REQUEST_CPP
#define WORLDSIM_ITEM_REQUEST_CPP

/* WorldSim: ItemRequest
	#include "ItemRequest.cpp"

	If a Character needs an item from a Settlement they can put in a request
	
*/

#include <set>
#include <optional>

class ItemRequest
{
	public:
	
	Character * requester;
	ItemType type;
	int value;
	
	ItemRequest(Character* c, ItemType t, int v) : requester(c), type(t), value(v)
	{
	}

	
};


// Comparator for ItemRequest
struct ItemRequestComparator
{
	bool operator() (const ItemRequest& lhs, const ItemRequest& rhs) const
	{
		// Sort in descending order of value
		return lhs.value > rhs.value;
	}
};

class ItemRequestManager
{
	public:
	
	//Vector <ItemRequest> vRequest;
	std::multiset<ItemRequest, ItemRequestComparator> requests;
	
	ItemRequestManager()
	{
		
	}
	
	void add(Character *requester, ItemType type, int value)
	{
		requests.insert(ItemRequest(requester, type, value));
	}

	std::optional<ItemRequest> pullMostValuableRequest()
	{
		if (requests.empty())
		{
			return std::nullopt; // Return an empty optional if there are no requests
		}

		auto it = requests.rbegin(); // Iterator to the most valuable request
		ItemRequest mostValuableRequest = *it; // Copy the most valuable request
		requests.erase(--it.base()); // Erase the most valuable request from the set

		return mostValuableRequest; // Return the most valuable request
	}

	bool empty() const
	{
		return requests.empty();
	}
	
};


#endif // WORLDSIM_ITEM_REQUEST_CPP
