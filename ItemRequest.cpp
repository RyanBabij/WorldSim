#pragma once
#ifndef WORLDSIM_ITEM_REQUEST_CPP
#define WORLDSIM_ITEM_REQUEST_CPP

/* WorldSim: ItemRequest
	#include "ItemRequest.cpp"

	Requests for an Item to be made. Requests can be made by any entity with a HasMoney interface, typically
	Characters or Government.
	
	Item requests should be more general, like "hunting items with minimum level 1"
	
*/

#include <set>
#include <optional>

// class ItemRequestCategory
// {
	// public:
	
	// CanRequestItem * requester;
	// ItemCategory category;
	// int value;
	// bool privateContract;
	
	// ItemRequest(CanRequestItem* c, ItemCategory t, int v) : requester(c), type(t), value(v)
	// {
		// privateContract=true;
	// }
	
	// std::string toString() const
	// {
		// return "Unknown";
	// }

	
// };


class ItemRequest
{
	public:
	
	CanRequestItem * requester;
	ItemType type;
	int value;
	bool privateContract;
	
	ItemRequest(CanRequestItem* c, ItemType t, int v) : requester(c), type(t), value(v)
	{
		privateContract=true;
	}
	
	std::string toString() const
	{
		std::string itemType = "UNKNOWN";
		
		if ( type == ITEM_HOE )
		{
			itemType = "HOE";
		}
		else if ( type == ITEM_PICKAXE )
		{
			itemType = "PICKAXE";
		}
		else if ( type == ITEM_AXE )
		{
			itemType = "AXE";
		}
		else if ( type == ITEM_LONGBOW )
		{
			itemType = "LONGBOW";
		}
		
		std::string str = "Request for "+itemType+" at $"+DataTools::toString(value)+".";
		return str;
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
	private:
	std::multiset<ItemRequest, ItemRequestComparator> requests;
	
	public:
	
	ItemRequestManager()
	{
		
	}
	
	void add(CanRequestItem *requester, ItemType type, int value, bool deleteDuplicates=true)
	{
		// if ( value == 0 )
		// {
			// return;
		// }
		
		// If the requester has enough money, add the new request
		if (requester->takeMoney(value))
		{
			if ( deleteDuplicates )
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
			}
			requests.insert(ItemRequest(requester, type, value));
		}
	}
	
	// void addTreasuryRequest(Character *requester, ResourceManager* money, ItemManager* stockpile, ItemType type, int value)
	// {
		// requests.insert(ItemRequest(requester, type, value));
	// }
	
	// void removeAll(CanRequestItem* requester, ItemType type)
	// {
		// // Remove requests of type and refund
		// for (auto it = requests.begin(); it != requests.end(); )
		// {
			// if (it->requester == requester && it->type == type)
			// {
				// requester->giveMoney(it->value);
				// //std::cout<<"Money refunded from contract: "<<it->value<<"\n";
				// it = requests.erase(it); // Erase and move to the next element
				// continue;
			// }
			// ++it;
		// }
	// }
	
	void removeAll(HasMoney* requester, ItemType type)
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
	
	void print()
	{
		// Remove requests of type and refund
		for (auto it = requests.begin(); it != requests.end(); )
		{
			std::cout<<it->toString()<<"\n";
			++it;
		}
	}
	
};


#endif // WORLDSIM_ITEM_REQUEST_CPP
