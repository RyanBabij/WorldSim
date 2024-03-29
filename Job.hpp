#pragma once

#include <Container/Vector/Vector.hpp>

#include "Item_Attributes.hpp"

class Job
{
public:
	bool requiresItem;
	ItemAction requiredAction; /* The required action(s) for the job. */
	
	Item_Attributes desiredItemAttributes;
	
	enumLocation requiredLocation; // Set to LOCATION_NONE if location not needed.

	int desiredWoodcuttingValue;
	int desiredFarmingValue;
	int desiredMiningValue;

	enumJob type;

	Job();
	virtual ~Job();

	virtual bool canDoJob();
	virtual int jobValue();
	virtual Item* getBestItem(Vector<Item*>* vItem);
	virtual Item* getBestItem(Item* item1, Item* item2);
	
	virtual std::string getName();
};

class Job_Woodcutting : public Job
{
public:
	Job_Woodcutting();
	
	virtual std::string getName() override;
};

class Job_Mining : public Job
{
public:
	Job_Mining();
	
	virtual std::string getName() override;
};

class Job_Farming : public Job
{
public:
	Job_Farming();
	
	virtual std::string getName() override;
};

class Job_Prospecting: public Job
{
	public:
	Job_Prospecting();
	
	virtual std::string getName() override;
};

class Job_Hunting : public Job
{
public:
	Job_Hunting();
	
	virtual std::string getName() override;
};