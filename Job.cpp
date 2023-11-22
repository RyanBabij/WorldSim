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
	desiredWoodcuttingValue = 0;
	desiredFarmingValue = 0;
	desiredMiningValue = 0;
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

Job_Woodcutting::Job_Woodcutting() : Job()
{
	desiredWoodcuttingValue = 1;
}

Job_Mining::Job_Mining() : Job()
{
	desiredMiningValue = 1;
}

Job_Farming::Job_Farming() : Job()
{
	desiredFarmingValue = 1;
}

#endif // WORLDSIM_JOB_CPP
