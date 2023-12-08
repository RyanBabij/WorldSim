#pragma once
#ifndef WORLDSIM_STOCKPILE_CPP
#define WORLDSIM_STOCKPILE_CPP

#include "Stockpile.hpp"

// Craftable class implementation
Craftable::Craftable() {
	requiresLocation = false;
}

Item* Craftable::produce() {
	// Implementation of produce method
	return nullptr;
}

// Intermediate class implementation
Intermediate::Intermediate(enumIntermediate _type, int _amount) {
	type = _type;
	amount = _amount;
}

// IntermediateRequirement class implementation
IntermediateRequirement::IntermediateRequirement() {
	// Constructor body
}

IntermediateRequirement::IntermediateRequirement(enumIntermediate type, int amount) {
	vResource.push(Intermediate(type, amount));
}

void IntermediateRequirement::add(Intermediate resource) {
	vResource.push(resource);
}

void IntermediateRequirement::add(enumIntermediate _type, int _amount) {
	// Add to existing if there is one
	for (int i = 0; i < vResource.size(); ++i) {
		if (vResource(i).type == _type) {
			vResource(i).amount += _amount;
			return;
		}
	}
	vResource.push(Intermediate(_type, _amount));
}

void IntermediateRequirement::print() {
	// Implementation of print method
}

// Resource class implementation
Resource::Resource(enumResource _type, int _amount) {
	type = _type;
	amount = _amount;
}

// ResourceRequirement class implementation
ResourceRequirement::ResourceRequirement() {
	// Constructor body
}

ResourceRequirement::ResourceRequirement(enumResource type, int amount) {
	vResource.push(Resource(type, amount));
}

void ResourceRequirement::add(Resource resource) {
	vResource.push(resource);
}

void ResourceRequirement::add(enumResource _type, int _amount) {
	// Add to existing if there is one
	for (int i = 0; i < vResource.size(); ++i) {
		if (vResource(i).type == _type) {
			vResource(i).amount += _amount;
			return;
		}
	}
	vResource.push(Resource(_type, _amount));
}

void ResourceRequirement::print() {
	// Implementation of print method
}

// HasResourceRequirement class implementation
HasResourceRequirement::HasResourceRequirement() {
	// Constructor body
}

ResourceRequirement HasResourceRequirement::getResourceRequirement() {
	return ResourceRequirement();
}

// Stockpile class implementation
Stockpile::Stockpile() {
	// Initialize resourceMap with all resources set to 0
	for (int i = 0; i < RESOURCE_COUNT; ++i) {
		resourceMap[static_cast<enumResource>(i)] = 0;
	}
	// Initialize intermediateMap with all intermediates set to 0
	for (int i = 0; i < INTERMEDIATE_COUNT; ++i) {
		intermediateMap[static_cast<enumIntermediate>(i)] = 0;
	}
}

void Stockpile::add(enumResource type, int amount) {
	resourceMap[type] += amount;
}

int Stockpile::get(enumResource type) {
	return resourceMap[type];
}

bool Stockpile::take(enumResource type, int amount) {
	if (resourceMap[type] >= amount) {
		resourceMap[type] -= amount;
		return true;
	}
	return false;
}

void Stockpile::add(enumIntermediate type, int amount) {
	intermediateMap[type] += amount;
}

int Stockpile::get(enumIntermediate type) {
	return intermediateMap[type];
}

bool Stockpile::take(enumIntermediate type, int amount) {
	if (intermediateMap[type] >= amount) {
		intermediateMap[type] -= amount;
		return true;
	}
	return false;
}

bool Stockpile::hasEnough(enumResource type, int amount) {
	return resourceMap[type] >= amount;
}

bool Stockpile::hasEnough(enumIntermediate type, int amount) {
	return intermediateMap[type] >= amount;
}

void Stockpile::add(Item* item) {
	vItem.push(item);
}

void Stockpile::take(Item* item) {
	if (item == nullptr) { return; }
	vItem.remove(item);
}

int Stockpile::getNumOfType(ItemType type) {
	int num = 0;
	for (int i = 0; i < vItem.size(); ++i) {
		if (vItem(i)->type == type) {
			++num;
		}
	}
	return num;
}

Item* Stockpile::getBestItemFor(Job job) {
	return job.getBestItem(&vItem);
}

Item* Stockpile::getBestItemFor(Job* job) {
	return job->getBestItem(&vItem);
}

int Stockpile::getNumberOfItems(ItemType type) {
	int numItems = 0;
	for (int i = 0; i < vItem.size(); ++i) {
		if (vItem(i)->type == type) {
			++numItems;
		}
	}
	return numItems;
}
		
bool Stockpile::hasEnough ( Intermediate resource )
{
	return hasEnough (resource.type, resource.amount);
}

bool Stockpile::hasEnough(IntermediateRequirement requirement) // was canMake
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
bool Stockpile::canMake(IntermediateRequirement requirement)
{
	return hasEnough(requirement);
}

bool Stockpile::deduct(IntermediateRequirement requirement)
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

bool Stockpile::hasEnough ( Resource resource )
{
	return hasEnough (resource.type, resource.amount);
}

bool Stockpile::hasEnough(ResourceRequirement requirement) // was canMake
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
bool Stockpile::canMake(ResourceRequirement requirement)
{
	return hasEnough(requirement);
}

bool Stockpile::deduct(ResourceRequirement requirement)
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




void Stockpile::print() {
	std::cout << "Resource Inventory:\n";
	for (const auto& resourcePair : resourceMap) {
		std::string resourceName = resourceToString(resourcePair.first); // Assuming a function to convert enumResource to string
		int resourceAmount = resourcePair.second;
		std::cout << resourceName << ": " << resourceAmount << "\n";
	}

	std::cout << "\nIntermediate Inventory:\n";
	for (const auto& intermediatePair : intermediateMap) {
		std::string intermediateName = intermediateToString(intermediatePair.first); // Assuming a function to convert enumIntermediate to string
		int intermediateAmount = intermediatePair.second;
		std::cout << intermediateName << ": " << intermediateAmount << "\n";
	}

	std::cout << "\nItem Manager Contents:\n";
	std::map<std::string, int> itemCounts;
	for (int i = 0; i < vItem.size(); ++i) {
		Item* currentItem = vItem(i);
		if (currentItem != nullptr) {
			std::string itemName = currentItem->getName();
			itemCounts[itemName]++;
		}
	}
	for (const auto& pair : itemCounts) {
		std::cout << "Item: " << pair.first << "  x" << pair.second << std::endl;
	}
}


#endif // WORLDSIM_STOCKPILE_CPP
