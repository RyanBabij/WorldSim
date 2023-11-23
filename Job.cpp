#pragma once
#ifndef WORLDSIM_JOB_CPP
#define WORLDSIM_JOB_CPP

/* WorldSim: Job
	#include "Job.cpp"

	Manage jobs which need to be done.
	
*/

#include "Job.hpp"

Job::Job()
{
	type = JOB_UNKNOWN;
	requiredItem = ITEM_NONE;
	desiredWoodcuttingValue = 0;
	desiredFarmingValue = 0;
	desiredMiningValue = 0;
}

Job::~Job()
{
}

bool Job::canDoJob()
{
	return true;
}

int Job::jobValue()
{
	return 0;
}

Item* Job::getBestItem(Vector<Item*>* vItem)
{
	Item* bestItem = nullptr;
	int bestScore = -1;

	for (int i = 0; i < vItem->size(); ++i)
	{
		const int score = vItem->at(i)->attributes.suitability(this);
		if (score > bestScore)
		{
			bestScore = score;
			bestItem = vItem->at(i);
		}
	}

	return bestItem;
}

Item* Job::getBestItem(Item* item1, Item* item2)
{
	if ( item1 == 0 )
	{
		return item2;
	}
	if ( item2 == 0 )
	{
		return item1;
	}
	
	const int score1 = item1->attributes.suitability(this);
	const int score2 = item2->attributes.suitability(this);
	if (score1 >= score2)
	{
		return item1;
	}
	return item2;
}

std::string Job::getName()
{
	return "unknown";
}

Job_Woodcutting::Job_Woodcutting() : Job()
{
	type = JOB_WOODCUTTING;
	requiredItem = ITEM_AXE;
	desiredWoodcuttingValue = 1;
}

std::string Job_Woodcutting::getName()
{
	return "woodcutting";
}

Job_Mining::Job_Mining() : Job()
{
	type = JOB_MINING;
	requiredItem = ITEM_PICKAXE;
	desiredMiningValue = 1;
}

std::string Job_Mining::getName()
{
	return "mining";
}

Job_Farming::Job_Farming() : Job()
{
	type = JOB_FARMING;
	requiredItem = ITEM_HOE;
	desiredFarmingValue = 1;
}

std::string Job_Farming::getName()
{
	return "farming";
}

#endif // WORLDSIM_JOB_CPP
