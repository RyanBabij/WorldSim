#pragma once
#ifndef WORLDSIM_ITEM_REQUEST_CPP
#define WORLDSIM_ITEM_REQUEST_CPP

/* WorldSim: ItemRequest
	#include "ItemRequest.cpp"

	Requests for an Item to be made. Requests can be made by any entity with a HasMoney interface, typically
	Characters or Government.
	
	Item requests should be more general, like "hunting items with minimum level 1"
	
	we might prefer some kind of functionality request. Eg "An item for digging". "An item which can shoot".
	We might need to build a table of item and functionality for different basic tasks
	
*/

#include <set>
#include <optional>

class ItemRequestCategory
{
	public:
	
	CanRequestItem * requester;
	ItemAction category;
	int value;
	int minimumLevel;
	bool privateContract;
	

	
	ItemRequestCategory(CanRequestItem* c, ItemAction t, int v, int m) :
	requester(c), category(t), value(v), minimumLevel(m)
	{
		privateContract=true;
	}
	
	std::string toString() const
	{
		return "Unknown";
	}

	
};

// Comparator for ItemRequestCategory
struct ItemRequestCategoryComparator
{
	bool operator() (const ItemRequestCategory& lhs, const ItemRequestCategory& rhs) const
	{
		// Sort in descending order of value
		return lhs.value > rhs.value;
	}
};


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
		std::multiset<ItemRequestCategory, ItemRequestCategoryComparator> categoryRequests;

	public:
		ItemRequestManager()
		{
		}

		void add(CanRequestItem *requester, ItemType type, int value, bool deleteDuplicates = true)
		{
			if (requester->takeMoney(value))
			{
				if (deleteDuplicates)
				{
					for (auto it = requests.begin(); it != requests.end(); )
					{
						if (it->requester == requester && it->type == type)
						{
							requester->giveMoney(it->value);
							it = requests.erase(it);
							continue;
						}
						++it;
					}
				}
				requests.insert(ItemRequest(requester, type, value));
			}
		}

		void add
		(CanRequestItem *requester, ItemAction category, int value, int minimumLevel, bool deleteDuplicates = true)
		{
			if (requester->takeMoney(value))
			{
				if (deleteDuplicates)
				{
					for (auto it = categoryRequests.begin(); it != categoryRequests.end(); )
					{
						if (it->requester == requester && it->category == category)
						{
							requester->giveMoney(it->value);
							it = categoryRequests.erase(it);
							continue;
						}
						++it;
					}
				}
				categoryRequests.insert(ItemRequestCategory(requester, category, value, minimumLevel));
			}
		}

		void removeAll(HasMoney* requester, ItemType type)
		{
			for (auto it = requests.begin(); it != requests.end(); )
			{
				if (it->requester == requester && it->type == type)
				{
					requester->giveMoney(it->value);
					it = requests.erase(it);
					continue;
				}
				++it;
			}
		}

		void removeAll(HasMoney* requester, ItemAction category)
		{
			for (auto it = categoryRequests.begin(); it != categoryRequests.end(); )
			{
				if (it->requester == requester && it->category == category)
				{
					requester->giveMoney(it->value);
					it = categoryRequests.erase(it);
					continue;
				}
				++it;
			}
		}

		std::optional<ItemRequest> pullMostValuableRequest(bool returnZeroValues)
		{
			if (requests.empty())
			{
				return std::nullopt;
			}

			auto it = requests.begin();
			while (it != requests.end())
			{
				const ItemRequest& mostValuableRequest = *it;
				if (mostValuableRequest.value > 0 || returnZeroValues)
				{
					requests.erase(it);
					return mostValuableRequest;
				}
				++it;
			}

			return std::nullopt;
		}
		
		std::optional<ItemRequestCategory> pullMostValuableRequestCategory(bool returnZeroValues)
		{
			if (categoryRequests.empty())
			{
				return std::nullopt;
			}

			auto it = categoryRequests.begin();
			while (it != categoryRequests.end())
			{
				const ItemRequestCategory& mostValuableRequest = *it;
				if (mostValuableRequest.value > 0 || returnZeroValues)
				{
					categoryRequests.erase(it);
					return mostValuableRequest;
				}
				++it;
			}

			return std::nullopt;
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
				return 0.0;
			}
			return static_cast<double>(totalValue) / count;
		}
		
		double getAverageValue(ItemAction category, int minLevel) const
		{
			int totalValue = 0;
			int count = 0;

			for (const auto& request : categoryRequests)
			{
				if (request.category == category && request.minimumLevel == minLevel)
				{
					totalValue += request.value;
					++count;
				}
			}

			if (count == 0)
			{
				return 0.0; // Avoid division by zero
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
			std::cout << "Individual Item Requests:\n";
			for (const auto& request : requests)
			{
				std::cout << request.toString() << "\n";
			}

			std::cout << "\nCategory Requests:\n";
			for (const auto& categoryRequest : categoryRequests)
			{
				std::cout << categoryRequest.toString() << "\n";
			}
		}
};


#endif // WORLDSIM_ITEM_REQUEST_CPP
