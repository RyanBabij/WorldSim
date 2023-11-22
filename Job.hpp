#pragma once

#include <Container/Vector/Vector.hpp>

#include "Item_Attributes.hpp"

class Job
{
public:
	bool requiresItem;
	Item_Attributes desiredItemAttributes;

	int desiredWoodcuttingValue;
	int desiredFarmingValue;
	int desiredMiningValue;

	enumJob type;

	Job();

	virtual bool canDoJob();
	virtual int jobValue();
	virtual Item* getBestItem(Vector<Item*>* vItem);
};

class Job_Woodcutting : public Job
{
public:
	Job_Woodcutting();
};

class Job_Mining : public Job
{
public:
	Job_Mining();
};

class Job_Farming : public Job
{
public:
	Job_Farming();
};
