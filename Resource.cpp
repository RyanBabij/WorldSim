#pragma once
#ifndef WORLDSIM_RESOURCE_CPP
#define WORLDSIM_RESOURCE_CPP

/* WorldSim: Resource
	#include "Resource.cpp"

	Manage resources for a settlement or civ.
	
	Resources are generally managed on a per-settlement basis in order to create logistics, but a civ will seek to
	move resources efficiently between settlements.
	Resources are hardcoded for simplicity.
	
	In some cases I may break the resources down into their own classes to manage quality, however generally this will
	be best handled by crafting skill.
	
*/

#include "HasMoney.cpp"

// Resources which have a higher degree of processing to obtain

#include "Creature.hpp"

class Recipe2
{
	// Required location (null if not needed)
	bool requiresLocation; // Can only be made at a designated location (the location tracks which recipes it can make)
	
	Recipe2()
	{
	}
};

class RecipeManager2
{
	Vector <Recipe2> vRecipe;
	public:
};


class Recipe_HuntingBow
{
	public:
	Recipe_HuntingBow()
	{
	}
};

enum enumIntermediate
{
	INTERMEDIATE_NONE,
	INTERMEDIATE_FIBRE,
	INTERMEDIATE_GUT,
	INTERMEDIATE_MEAT,
	INTERMEDIATE_BONE,
	INTERMEDIATE_COUNT
};

class Intermediate
{
	public:
	enumIntermediate type;
	int amount;
	
	Intermediate (enumIntermediate _type, int _amount)
	{
		type=_type;
		amount=_amount;
	}
};

class IntermediateRequirement
{
	public:
		Vector <Intermediate> vResource;

		IntermediateRequirement()
		{
		}
		
		// New constructor for single resource
		IntermediateRequirement(enumIntermediate type, int amount)
		{
			vResource.push(Intermediate(type, amount));
		}
	 
		void add(Intermediate resource)
		{
			vResource.push(resource);
		}
		
		void add (enumIntermediate _type, int _amount)
		{
			// add to existing if there is one
			for (int i=0; i<vResource.size(); ++i)
			{
				if (vResource(i).type == _type)
				{
					vResource(i).amount += _amount;
					return;
				}
			}
			vResource.push(Intermediate(_type, _amount));
		}

		void print()
		{
		}
};


class IntermediateManager: public HasMoney
{
	private:
		std::unordered_map<enumIntermediate, int> resourceMap;
		
	public:
		IntermediateManager()
		{
        // Initialize resourceMap with all resources set to 0
			for (int i = 0; i < RESOURCE_COUNT; ++i)
			{
				resourceMap[static_cast<enumIntermediate>(i)] = 0;
			}
		}
		
		void add(enumIntermediate type, int amount)
		{
			resourceMap[type] += amount;
		}
		
		int get(enumIntermediate type)
		{
			return resourceMap[type];
		}
		
		bool take (enumIntermediate type, int amount)
		{
			if (resourceMap[type] >= amount)
			{
				resourceMap[type] -= amount;
				return true;
			}
			return false;
		}
		
		bool hasEnough ( enumIntermediate type, int amount )
		{
			return resourceMap[type] >= amount;
		}
		
		bool hasEnough ( Intermediate resource )
		{
			return hasEnough (resource.type, resource.amount);
		}
		
		bool hasEnough(IntermediateRequirement requirement) // was canMake
		{
			for (const auto& res : requirement.vResource)
			{
				if (!hasEnough(res.type, res.amount))
				{
					return false;
				}
			}
			return true;
		}
		bool canMake(IntermediateRequirement requirement)
		{
			return hasEnough(requirement);
		}
		
		bool deduct(IntermediateRequirement requirement)
		{
			if (!hasEnough(requirement))
			{
				return false;
			}
			for (const auto& res : requirement.vResource)
			{
				take(res.type, res.amount);
			}
			return true;
		}

		void print()
		{
			// std::cout << "Intermediate Inventory:\n";
			// int counter=1;
			// for (const auto& resourcePair : resourceMap)
			// {
				// std::string resourceName = resourceToString(resourcePair.first);
				// int resourceAmount = resourcePair.second;
				// std::cout << resourceName << ": " << resourceAmount << ". ";
				// if (counter%5==0)
				// {
					// std::cout<<"\n";
				// }
				// ++counter;
			// }
			// std::cout<<"\n";
		}

		
};

class Resource
{
	public:
	enumResource type;
	int amount;
	
	Resource (enumResource _type, int _amount)
	{
		type=_type;
		amount=_amount;
	}
	
};

class ResourceRequirement
{
	public:
		Vector <Resource> vResource;

		ResourceRequirement()
		{
		}
		
		// New constructor for single resource
		ResourceRequirement(enumResource type, int amount)
		{
			vResource.push(Resource(type, amount));
		}
	 
		void add(Resource resource)
		{
			vResource.push(resource);
		}
		
		void add (enumResource _type, int _amount)
		{
			// add to existing if there is one
			for (int i=0; i<vResource.size(); ++i)
			{
				if (vResource(i).type == _type)
				{
					vResource(i).amount += _amount;
					return;
				}
			}
			vResource.push(Resource(_type, _amount));
		}

		void print()
		{
		}
};

class HasResourceRequirement
{
	public:
	
		HasResourceRequirement()
		{
		}
		virtual ResourceRequirement getResourceRequirement()
		{
			// Return a blank resource requirement (no resources needed)
			return ResourceRequirement();
		}
};

class ResourceManager: public HasMoney
{
	private:
		std::unordered_map<enumResource, int> resourceMap;
		
	public:
		ResourceManager()
		{
        // Initialize resourceMap with all resources set to 0
			for (int i = 0; i < RESOURCE_COUNT; ++i)
			{
				resourceMap[static_cast<enumResource>(i)] = 0;
			}
		}
		
		void add(enumResource type, int amount)
		{
			resourceMap[type] += amount;
		}
		
		int get(enumResource type)
		{
			return resourceMap[type];
		}
		
		bool take (enumResource type, int amount)
		{
			if (resourceMap[type] >= amount)
			{
				resourceMap[type] -= amount;
				return true;
			}
			return false;
		}
		
		bool hasEnough ( enumResource type, int amount )
		{
			return resourceMap[type] >= amount;
		}
		
		bool hasEnough ( Resource resource )
		{
			return hasEnough (resource.type, resource.amount);
		}
		
		bool hasEnough(ResourceRequirement requirement) // was canMake
		{
			for (const auto& res : requirement.vResource)
			{
				if (!hasEnough(res.type, res.amount))
				{
					return false;
				}
			}
			return true;
		}
		bool canMake(ResourceRequirement requirement)
		{
			return hasEnough(requirement);
		}
		
		bool deduct(ResourceRequirement requirement)
		{
			if (!hasEnough(requirement))
			{
				return false;
			}
			for (const auto& res : requirement.vResource)
			{
				take(res.type, res.amount);
			}
			return true;
		}

		void print()
		{
			std::cout << "Resource Inventory:\n";
			int counter=1;
			for (const auto& resourcePair : resourceMap)
			{
				std::string resourceName = resourceToString(resourcePair.first);
				int resourceAmount = resourcePair.second;
				std::cout << resourceName << ": " << resourceAmount << ". ";
				if (counter%5==0)
				{
					std::cout<<"\n";
				}
				++counter;
			}
			std::cout<<"\n";
		}

		
};


#endif // WORLDSIM_RESOURCE_CPP
