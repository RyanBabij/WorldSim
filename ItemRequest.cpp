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
		// if ( value == 0 )
		// {
			// return;
		// }
		
		// If the requester has enough money, add the new request
		if (requester->takeMoney(value))
		{
			// Check if the requester already has a request for the same type and remove them.
			for (auto it = requests.begin(); it != requests.end(); )
			{
				if (it->requester == requester && it->type == type)
				{
					requester->giveMoney(it->value);
					//std::cout<<"Money refunded from contract: "<<it->value<<"\n";
					it = requests.erase(it); // Erase and move to the next element
					continue;
				}
				++it;
			}
			requests.insert(ItemRequest(requester, type, value));
		}
	}
	
	void removeAll(Character* requester, ItemType type)
	{
		// Remove requests of type and refund
		for (auto it = requests.begin(); it != requests.end(); )
		{
			if (it->requester == requester && it->type == type)
			{
				requester->giveMoney(it->value);
				//std::cout<<"Money refunded from contract: "<<it->value<<"\n";
				it = requests.erase(it); // Erase and move to the next element
				continue;
			}
			++it;
		}
	}


	std::optional<ItemRequest> pullMostValuableRequest(bool returnZeroValues)
	{
		if (requests.empty())
		{
			return std::nullopt; // Return an empty optional if there are no requests
		}

		auto it = requests.begin(); // Iterator to the least valuable request

		while (it != requests.end())
		{
			const ItemRequest& mostValuableRequest = *it;

			if (mostValuableRequest.value > 0 || returnZeroValues)
			{
				requests.erase(it); // Erase the most valuable request from the set
				return mostValuableRequest; // Return the most valuable request
			}
			++it; // Move to the next request
		}

		return std::nullopt; // Return an empty optional if no suitable request is found
	}
		
	double getAverageValue(ItemType type) const
	{
		int totalValue = 0;
		int count = 0;

		for (const auto& request : requests)
		{
			if (request.type == type)
			{
				totalValue += request.value;
				++count;
			}
		}

		if (count == 0)
		{
			return 0.0; // Return 0 if there are no requests of the specified type
		}

		return static_cast<double>(totalValue) / count;
	}


	int getTotalValue() const
	{
		int totalValue = 0;
		for (const auto& request : requests)
		{
			totalValue += request.value;
		}
		return totalValue;
	}
	int getNumContracts() const
	{
		int totalContracts = 0;
		for (const auto& request : requests)
		{
			totalContracts++;
		}
		return totalContracts;
	}


	bool empty() const
	{
		return requests.empty();
	}
	
	int size()
	{
		return requests.size();
	}
	
};


#endif // WORLDSIM_ITEM_REQUEST_CPP
