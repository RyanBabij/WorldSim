#pragma once
#ifndef WORLDSIM_LOCATION_REQUEST_CPP
#define WORLDSIM_LOCATION_REQUEST_CPP

/* WorldSim: LocationRequest
	#include "LocationRequest.cpp"

	Requests for an location to be built. Requests can be made by any entity with a HasMoney interface, typically
	Characters or Government.
	
*/

#include <set>
#include <optional>


class LocationRequest
{
	public:
	
	HasMoney * requester;
	enumLocation type;
	int value;
	bool privateContract;
	
	LocationRequest(HasMoney* c, enumLocation t, int v) : requester(c), type(t), value(v)
	{
		privateContract=true;
	}

	std::string toString() const
	{
		std::string locationType = "UNKNOWN_LOCATION";
		
		if ( type == LOCATION_HALL )
		{
			locationType = "HALL";
		}
		else if ( type == LOCATION_MINE )
		{
			locationType = "MINE";
		}
		else if ( type == LOCATION_DWELLING )
		{
			locationType = "DWELLING";
		}
		else if ( type == LOCATION_FARM )
		{
			locationType = "FARM";
		}
		
		std::string str = "Request for "+locationType+" at $"+DataTools::toString(value)+".";
		return str;
	}

	
};

// Comparator for LocationRequest
struct LocationRequestComparator
{
	bool operator() (const LocationRequest& lhs, const LocationRequest& rhs) const
	{
		// Sort in descending order of value
		return lhs.value > rhs.value;
	}
};

class LocationRequestManager
{
	private:
	std::multiset<LocationRequest, LocationRequestComparator> requests;
	
	public:
	
	LocationRequestManager()
	{
		
	}
	
	void add(HasMoney *requester, enumLocation type, int value, bool deleteDuplicates=true)
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
			requests.insert(LocationRequest(requester, type, value));
		}
	}
	
	void addTreasuryRequest(Character *requester, ResourceManager* money, ItemManager* stockpile, enumLocation type, int value)
	{
		requests.insert(LocationRequest(requester, type, value));
	}
	
	void removeAll(HasMoney* requester, enumLocation type)
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


	std::optional<LocationRequest> pullMostValuableRequest(bool returnZeroValues)
	{
		if (requests.empty())
		{
			return std::nullopt; // Return an empty optional if there are no requests
		}

		auto it = requests.begin(); // Iterator to the least valuable request

		while (it != requests.end())
		{
			const LocationRequest& mostValuableRequest = *it;

			if (mostValuableRequest.value > 0 || returnZeroValues)
			{
				requests.erase(it); // Erase the most valuable request from the set
				return mostValuableRequest; // Return the most valuable request
			}
			++it; // Move to the next request
		}

		return std::nullopt; // Return an empty optional if no suitable request is found
	}
		
	double getAverageValue(enumLocation type) const
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
