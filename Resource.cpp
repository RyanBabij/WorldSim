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

#include "HasMoney.cpp"

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
