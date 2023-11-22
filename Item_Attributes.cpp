#pragma once
#ifndef WORLDSIM_ITEM_ATTRIBUTES_CPP
#define WORLDSIM_ITEM_ATTRIBUTES_CPP

/* Item_Attributes.cpp
	#include"Item_Attributes.cpp"

	Container for attributes of an item. Includes tools for comparing with other items.

*/

#include "Item_Attributes.hpp"

#include "Job.hpp"


Item_Attributes::Item_Attributes()
{
	woodcuttingValue=0;
	farmingValue=0;
	miningValue=0;
}

// Item must meet all minimum requirements
int Item_Attributes::suitability(Job* job)
{
	if (!job) return -1; // Return -1 if the job is null

	int score = 0;
	
	if ( job->desiredWoodcuttingValue > 0 )
	{
		if ( woodcuttingValue < job->desiredWoodcuttingValue )
		{
			return -1;
		}
		else
		{
			score+=woodcuttingValue-job->desiredWoodcuttingValue;
		}
	}
	
	if ( job->desiredFarmingValue > 0 )
	{
		if ( farmingValue < job->desiredFarmingValue )
		{
			return -1;
		}
		else
		{
			score+=farmingValue-job->desiredFarmingValue;
		}
	}
	
	if ( job->desiredMiningValue > 0 )
	{
		if ( miningValue < job->desiredMiningValue )
		{
			return -1;
		}
		else
		{
			score+=miningValue-job->desiredMiningValue;
		}
	}

	return score;
}


#endif // WORLDSIM_ITEM_ATTRIBUTES_CPP
