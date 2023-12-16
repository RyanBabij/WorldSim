#pragma once
#ifndef WORLDSIM_STOCKPILE_HPP
#define WORLDSIM_STOCKPILE_HPP

/* WorldSim: Stockpile
	#include "Stockpile.hpp"

	Manage resources for a settlement or civ.
	
	Resources are generally managed on a per-settlement basis in order to create logistics, but a civ will seek to
	move resources efficiently between settlements.
	Resources are hardcoded for simplicity.
	
	In some cases I may break the resources down into their own classes to manage quality, however generally this will
	be best handled by crafting skill.
	
*/


#include "HasMoney.cpp"
#include "Creature.hpp"
#include "Item.hpp"
#include "Job.hpp"
#include <unordered_map>
#include <string>
#include <iostream>


class Craftable
{
	public:
		bool requiresLocation;
		std::unordered_map<enumIntermediate, int> mIntermediate;
		std::unordered_map<enumResource, int> mResource;

		Craftable();
		Item* produce();
};

class CraftableManager
{
	Vector<Craftable> vRecipe;
	
	public:
		// CraftableManager methods (if any)
};

class Intermediate
{
	public:
		enumIntermediate type;
		int amount;

		Intermediate(enumIntermediate _type, int _amount);
};

class IntermediateRequirement
{
	public:
		Vector<Intermediate> vResource;

		IntermediateRequirement();
		IntermediateRequirement(enumIntermediate type, int amount);
		void add(Intermediate resource);
		void add(enumIntermediate _type, int _amount);
		void print();
};

class Resource
{
	public:
		enumResource type;
		int amount;

		Resource(enumResource _type, int _amount);
};

class ResourceRequirement
{
	public:
		Vector<Resource> vResource;

		ResourceRequirement();
		ResourceRequirement(enumResource type, int amount);
		void add(Resource resource);
		void add(enumResource _type, int _amount);
		void print();
};

// Class for items which have resource requirements.

class StockpileRequirement
{
	public:
		ResourceRequirement resourceRequirement;
		IntermediateRequirement intermediateRequirement;
		
		StockpileRequirement();
		
		void add(Resource resource);
		void add(enumResource _type, int _amount);
		void add(Intermediate intermediate);
		void add(enumIntermediate _type, int _amount);
		void print();
	
};

class HasResourceRequirement
{
	public:
		HasResourceRequirement();
		virtual ResourceRequirement getResourceRequirement();
};

class HasStockpileRequirement
{
	public:
		HasStockpileRequirement();
		virtual StockpileRequirement getStockpileRequirement();
};

class Stockpile : public HasMoney {
private:
	std::unordered_map<enumResource, int> resourceMap;
	std::unordered_map<enumIntermediate, int> intermediateMap;


public:
	Vector<Item*> vItem;
	
	Stockpile();

	void add(enumResource type, int amount);
	int get(enumResource type);
	bool take(enumResource type, int amount);
	bool hasEnough(enumResource type, int amount);

	void add(enumIntermediate type, int amount);
	int get(enumIntermediate type);
	bool take(enumIntermediate type, int amount);
	bool hasEnough(enumIntermediate type, int amount);
	
	bool hasEnough(IntermediateRequirement requirement); // was canMake
	bool hasEnough (Intermediate resource);
	bool canMake (IntermediateRequirement requirement);
	
	bool hasEnough (Resource resource);
	bool hasEnough(ResourceRequirement requirement);
	bool canMake(ResourceRequirement requirement);


	bool deduct(IntermediateRequirement requirement);
	bool deduct(ResourceRequirement requirement);

	void add(Item* item);
	void take(Item* item);
	int getNumOfType(ItemType type);
	Item* getBestItemFor(Job job);
	Item* getBestItemFor(Job* job);
	int getNumberOfItems(ItemType type);

	void print();
};

#endif // WORLDSIM_STOCKPILE_HPP
