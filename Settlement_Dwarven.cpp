#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_CPP
#define GUILD_SETTLEMENT_DWARVEN_CPP

#include "Settlement_Dwarven.hpp"

#include "Stockpile.hpp"
#include "ItemFactory.hpp"

#include "ItemRequest.cpp"
#include "LocationRequest.cpp"
#include "ResourceRequest.cpp"

#include "Job.cpp"


Settlement_Dwarven::Settlement_Dwarven(): Settlement()
{
	race = DWARVEN;

	monthlyCounter=0;
	dailyCounter=0;

	name.insert(0, "Fort ");
}

Texture* Settlement_Dwarven::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

void Settlement_Dwarven::checkStockpileForBestItem(Character* character, Job* job)
{
	Item* bestItemOnHand = character->getBestItemFor(job);
	Item* bestStockpileItem = stockpile.getBestItemFor(job);
	
	if ( bestItemOnHand == nullptr )
	{
		if ( bestStockpileItem == nullptr ) // There's no farming equipment anywhere
		{
			//putMarketRequest(character,job->requiredAction);
			// put in a category request here by mapping the item to its category.
			
			//ItemAction category = getActionOfItem(job->requiredItem);
			ItemAction action = job->requiredAction;

			if (action != ITEM_ACTION_NONE)
			{
				putMarketRequest(character,action,1);
				// also need to add code asking for better items once we have an item
			}
			
		}
		else // we don't have equipment but the stockpile does
		{
			stockpile.take(bestStockpileItem);
			character->giveItem(bestStockpileItem);
		}
	}
	else // we have equipment and the stockpile does too
	{
		Item* bestItem = job->getBestItem(bestItemOnHand,bestStockpileItem);
		
		if (bestItem == bestStockpileItem)
		{
			stockpile.take(bestStockpileItem);
			character->giveItem(bestStockpileItem);
		}
	}
}

void Settlement_Dwarven::putMarketRequest(Character* c, enumLocation type)
{
	if ( c->getMoney() > 0 )
	{
		int marketValue = locationRequestManager.getAverageValue(type) + 1;
		
		int amountCanPay = c->getMoney();
		if (marketValue < amountCanPay)
		{
			amountCanPay = marketValue;
		}
		
		locationRequestManager.removeAll(c,type);
		locationRequestManager.add(c,type,marketValue);
		//std::cout<<"Request for "<<enumLocationStr[type]<<" at price of "<<amountCanPay<<".\n";
	}
}

void Settlement_Dwarven::putMarketRequest(Character* c, enumResource type)
{
}

void Settlement_Dwarven::putMarketRequest(Character* c, ItemType type)
{
	if ( c->getMoney() > 0 )
	{
		int marketValue = requestManager.getAverageValue(type) + 1;
		
		int amountCanPay = c->getMoney();
		if (marketValue < amountCanPay)
		{
			amountCanPay = marketValue;
		}
		
		requestManager.removeAll(c,type);
		requestManager.add(c,type,marketValue);
		//std::cout<<"Request for "<<enumItemTypeStr[type]<<" at price of "<<amountCanPay<<".\n";
	}
}

void Settlement_Dwarven::putMarketRequest(Character* c, ItemAction category, int minLevel)
{
	// we might prefer some kind of functionality request. Eg "An item for digging". "An item which can shoot".
	
	if ( c->getMoney() > 0 )
	{
		int marketValue = requestManager.getAverageValue(category,minLevel) + 1;
		
		int amountCanPay = c->getMoney();
		if (marketValue < amountCanPay)
		{
			amountCanPay = marketValue;
		}
		
		requestManager.removeAll(c,category);
		requestManager.add(c,category,marketValue,minLevel);
		std::cout<<"Request for "<<actionToString(category)<<" item at price of "<<amountCanPay<<".\n";
	}
}

bool Settlement_Dwarven::hasLocation(enumLocation _location)
{
	Vector <Location*>* vMatchLocation = location.getLocation(_location);
	bool hasLoc = vMatchLocation->empty();
	delete vMatchLocation;
	return !hasLoc;
}

void Settlement_Dwarven::buildLocation(enumLocation _location)
{
	// Find a suitable location for it.
	// Make sure we don't run out of branches, otherwise we also need to build a hall.
	
	int totalFreeBranches = location.totalAvailableBranches();
	
	if ( totalFreeBranches==1 )
	{
		// we must build a hall.
		location.addLocation(LOCATION_HALL);
	}
	location.addLocation(_location);
	
}



bool Settlement_Dwarven::abstractDayJob( Character* character, Job* job)
{
	//std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Attempting job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
	
	checkStockpileForBestItem(character, job);
	
	Item* item = character->getBestItemFor(job);
	
	if (job->type == JOB_FARMING )
	{
		// for now assume the Character feeds themself plus a certain surplus, as this simplifies starvation issues
		
		// Personal output is how much a person's personal skill can affect output.
		// Technology can further improve the output as an independent variable.
		// I think without aid of tech, subsistence farming ratio of 1 farmer for 1 other person is reasonable.
		// Over time, technology of course should make a huge difference until farmers become a small minority.
		
		// Farming equipment presence and quality should also be important.
		
		// Check if we can borrow a hoe if we don't own one.
		
		
		
		// Move to farms. If not possible we will need to hunt/gather instead.
		if (character->moveToLocationType(LOCATION_FARM) == false )
		{
			//std::cout<<"Character unable to move to farm.\n";
			
			// Put in market request for a farm.
			putMarketRequest(character, job->requiredLocation);

			// Go hunting/gathering instead
			return false;
		}
		
		int farmingItemMult=1;
		if ( item != nullptr )
		{
			farmingItemMult = item->attributes.farmingValue+1;
		}
		
		int maxPersonalOutput = (character->getStrength()*farmingItemMult)+character->skillFarming;
		//int maxPersonalOutput = (8+character->skillFarming)*farmingItemMult;
		if (maxPersonalOutput > 28)
		{
			maxPersonalOutput=28;
		}
		int maxTotalOutput = maxPersonalOutput + technology.agricultureLevel;
		
		stockpile.add(RESOURCE_FOOD, globalRandom.rand(maxTotalOutput));
		
		character->skillUpFarming();
		payCharacterFromTreasury(character,1);

		
		std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Finished job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
		delete job;
		return true;
	}
	else if (job->type == JOB_HUNTING)
	{

		// Move to farms. If not possible we will need to hunt/gather instead.
		if (character->moveToLocationType(LOCATION_WILDERNESS) == false )
		{
			//std::cout<<"Character unable to move to wilderness.\n";
			return false;
		}

		World_Biome* biome = getBiome();
		
		if (biome == nullptr)
		{
			//std::cout<<"ERROR: No biome in Settlement\n";
		}
		//std::cout<<"Character hunting in biome: "<<biome->name<<".\n";
		
		int huntingItemMult=1;
		if ( item != nullptr )
		{
			huntingItemMult = item->attributes.huntingValue+1;
			
			if ( item->attributes.degrade(1) )
			{
				//std::cout<<"Item broke.\n";
				character->takeItem(item);
			}
		}

		
		int maxPersonalOutput = (character->getStrength()*huntingItemMult)+character->skillMarksmanship;
		if (maxPersonalOutput > 16)
		{
			maxPersonalOutput=32;
		}
		
		if ( globalRandom.flip() )
		{
			stockpile.add(RESOURCE_FOOD, globalRandom.rand(maxPersonalOutput));
			stockpile.add(INTERMEDIATE_GUT, 1);
			
			character->skillUpMarksmanship();
			
			Vector <Creature_Species*>* vCreature = biome->getAllCreatureTypes();
			
			if ( vCreature != nullptr && vCreature->size() > 0 )
			{
				for (int i=0;i<vCreature->size(); ++i)
				{
					//std::cout<<(*vCreature)(i)->name<<"\n";
				}
				int iCreature = globalRandom.rand8(vCreature->size());
				std::cout<<"Hunted a "<<(*vCreature)(iCreature)->name<<".\n";
			}
		}
		
		payCharacterFromTreasury(character,1);
		
		std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Finished job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
		delete job;
		return true;
	}
	else if (job->type == JOB_MINING)
	{
		// Character works in the mines for the month

		// Move to the mines
		if (character->moveToLocationType(job->requiredLocation) == false )
		{
			//std::cout<<"Character unable to move to mine.\n";
			
			putMarketRequest(character, job->requiredLocation);
		}
		
		int maxPersonalOutput = 14+character->skillMining;
		if (maxPersonalOutput > 28)
		{
			maxPersonalOutput=28;
		}
		int maxTotalOutput = maxPersonalOutput + technology.miningLevel;
		
		stockpile.add(RESOURCE_STONE, globalRandom.rand(maxTotalOutput));
		stockpile.add(RESOURCE_IRON, globalRandom.rand(maxTotalOutput/2));
		
		character->skillUpMining();
		payCharacterFromTreasury(character,1);
		
		std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Finished job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
		delete job;
		return true;
	}
	else if (job->type == JOB_PROSPECTING)
	{
		// Character searches the location for a mine site
		// Prospecting is linked to mining skill
		
		int maxPersonalOutput = 14+character->skillMining;
		if (maxPersonalOutput > 28)
		{
			maxPersonalOutput=28;
		}
		int maxTotalOutput = maxPersonalOutput + technology.miningLevel;
		
		// each month there's 50% chance of finding a mining site
		if (globalRandom.flip())
		{
			//std::cout<<"Mining site discovered.\n";
			
			
		}
		
		
		
		character->skillUpMining();
		payCharacterFromTreasury(character,1);
		
		std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Finished job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
		delete job;
		return true;
	}
	else if (job->type == JOB_WOODCUTTING)
	{
		// Character goes into wilderness to obtain wood
		
		if (character->moveToLocationType(LOCATION_WILDERNESS) == false )
		{
			std::cout<<"Character unable to move to wilderness.\n";
			return false;
		}
		
		stockpile.add(RESOURCE_WOOD, globalRandom.rand(8));

		std::cout<<character->getFullName()<<" at "<<character->getLocation()<<". Finished job "<<job->getName()<<". ($"<<character->getMoney()<<")\n";
		delete job;
		return true;
	}
	
	
	// if the job failed
	delete job;
	return false;
}

	// Convert enum to object
Item* Settlement_Dwarven::createItem(ItemType type, Character* creator=0, Location* location=0 /* TOOLS */)
{
	ItemFactory itemFactory;
	return itemFactory.produceBasic(type,creator,location);
}

Item* Settlement_Dwarven::produceItem(ItemType type, Character* creator, Location* location, CanRequestItem* recipient= nullptr)
{
	Item* newItem = createItem(type, creator, location);
	if (newItem && stockpile.canMake(newItem->getStockpileRequirement()))
	{
		stockpile.deduct(newItem->getStockpileRequirement());
		
		if ( recipient != nullptr )
		{
			recipient->recieveRequestedItem(newItem);
			return newItem;
		}
		
		stockpile.add(newItem);
		return newItem;
	}

	delete newItem; // Clean up if item was not added to stockpile
	return nullptr;
}

Item* Settlement_Dwarven::produceItem(ItemAction type, Character* creator, Location* location, CanRequestItem* recipient= nullptr, int minLevel=1)
{
	std::cout<<"Attempting to produce "<<actionToString(type)<<" item.\n";
	
	Vector <ItemType> vItems = getItemsInAction(type);
	
	// Try to make each item and see what the best one we can make is.
	
	Item* bestItem = nullptr;
	char bestActionValue = -1;
	
	std::cout<<"We can try to make:\n";
	for (int i=0;i<vItems.size();++i)
	{
		std::cout<<itemToString(vItems(i))<<"\n";
		
		// Create item and see if it has the right specs.
		Item* createdItem = createItem(vItems(i), creator, location);
		
		if ( createdItem != nullptr && stockpile.canMake(createdItem->getStockpileRequirement()) )
		{
			if ( createdItem->mAction[type] > bestActionValue )
			{
				delete bestItem;
				bestItem = createdItem;
				bestActionValue = createdItem->mAction[type];
			}
		}
		
	}
	
	if ( bestItem != nullptr )
	{
		std::cout<<"Best item for the job we can make is: "<<bestItem->getName()<<"\n";
		produceItem(bestItem->type, creator, location, recipient);
		delete bestItem;
	}
	
	
	
	
	
	// Determine the best one to make.
	// for now just make a random one
	
	//vItems.shuffle();
	
	// for (int i=0;i<vItems.size();++i)
	// {
		// std::cout<<itemToString(vItems(i))<<"\n";
		
		// Item* createdItem = produceItem(vItems(i),recipient);
		
	// }
	
	
	return nullptr;
}

void Settlement_Dwarven::payCharacter(Character* character, int amount)
{
	//character->giveMoney(amount);
	
	int moneyToReceive = amount;

	if (moneyToReceive > 3)
	{
		// Calculate 75% of the money, rounding down for the character's share
		int characterMoney = moneyToReceive * 0.75;

		// Give 75% to the character
		character->giveMoney(characterMoney);

		// The remaining amount is for tax. This includes the extra coin in case of an odd number
		int taxAmount = moneyToReceive - characterMoney;

		// Pay the 25% tax
		stockpile.addMoney(taxAmount);
	}
	else
	{
		// If 3 or less, give all to the character without tax
		character->giveMoney(moneyToReceive);
	}
}

void Settlement_Dwarven::payCharacterFromTreasury(Character* character, int amount)
{
	int amountToGive = stockpile.takeMoneyUpTo(amount);
	character->giveMoney(amountToGive);
}

bool Settlement_Dwarven::abstractDayConstruction(Character* character)
{
	if (globalRandom.rand8(DAYS_PER_MONTH) == 0 )
	{
		auto mostValuableRequestOpt = locationRequestManager.pullMostValuableRequest(false);
		if (mostValuableRequestOpt)
		{
			// If there is a most valuable request
			LocationRequest mostValuableRequest = *mostValuableRequestOpt;
			// Process the most valuable request
			
			Location* loc = nullptr;
			
			if ( canBuild(mostValuableRequest.type) )	
			{
				loc = location.addLocation(mostValuableRequest.type);
			}
			
			if (loc != nullptr)
			{
				//std::cout<<character->getFullName()<<": Building "<<loc->getName()<<". "<<character->getMoney()<<" money.\n";
				//std::cout<<"There are "<<requestManager.getNumContracts()<<" contracts.\n";
				// recieve the coins promised
				
				int moneyToRecieve = mostValuableRequest.value;
				
				payCharacter(character, mostValuableRequest.value);
				return true;
			}
			else // Couldn't make the item, put the request back through.
			{
				// Recursive try to make something else.
				abstractDayConstruction(character);
				
				mostValuableRequest.requester->giveMoney(mostValuableRequest.value);
				locationRequestManager.add(mostValuableRequest.requester,mostValuableRequest.type, mostValuableRequest.value);
				//std::cout<<"Unable to make item, trying another.\n";
			}
		}
	}
	
	return false;
}

bool Settlement_Dwarven::abstractDayProduction(Character* character)
{
	// Current process:
	// Step 1: Try to make an individual item request.
	// Step 2: Try to make an item cetegory request
	// Step 3: Try to make coins
	// Step 1 and 2 should be decided based on value.
	
	
	if ( globalRandom.rand8(DAYS_PER_MONTH) == 0 )
	{
		// Pull both individual item and item category and compare them here. Then push the least valuable one back on
		// and try to make the more valuable one.
		
		
		
		// Attempt to make specific items first, then later attempt to make item categories.
		// We will later fix it so that the most valuable request overall is fulfilled
		auto mostValuableRequestOpt = requestManager.pullMostValuableRequest(false);
		if (mostValuableRequestOpt)
		{
			// If there is a most valuable request
			ItemRequest mostValuableRequest = *mostValuableRequestOpt;
			// Process the most valuable request
			//std::cout << "Most valuable request value: " << mostValuableRequest.value << std::endl;
			
			Item* item = produceItem(mostValuableRequest.type, character, 0, mostValuableRequest.requester);
			
			if (item != nullptr)
			{
				//std::cout<<character->getFullName()<<": Produced "<<item->getName()<<". "<<character->getMoney()<<" money.\n";

				int moneyToRecieve = mostValuableRequest.value;
				payCharacter(character, mostValuableRequest.value);
				return true;
			}
			else // Couldn't make the item, put the request back through.
			{
				// Recursive try to make something else.
				abstractDayProduction(character);
				
				mostValuableRequest.requester->giveMoney(mostValuableRequest.value);
				requestManager.add(mostValuableRequest.requester,mostValuableRequest.type, mostValuableRequest.value);
				//std::cout<<"Unable to make item, trying another.\n";
			}
		}
		else
		{
			auto mostValuableRequestCategoryOpt = requestManager.pullMostValuableRequestCategory(false);
			if (mostValuableRequestCategoryOpt)
			{
				colour(GREEN);
				std::cout << "Attempting to make item category.\n";
				colour(DEFAULT);
				ItemRequestCategory mostValuableRequest = *mostValuableRequestCategoryOpt;
				
				Item* item = produceItem
				(mostValuableRequest.category,character,0,mostValuableRequest.requester, mostValuableRequest.minimumLevel);

				requestManager.add(mostValuableRequest.requester,mostValuableRequest.category, mostValuableRequest.value,
				mostValuableRequest.minimumLevel);
			}
			else if (getMoneyPercentInTreasury() < 0.25 || getAverageCharacterWealth() < 10 )
			{
				//std::cout<<character->getFullName()<<": Producing coins. "<<character->getMoney()<<" money.\n";
				
				if ( stockpile.take(RESOURCE_IRON,1) )
				{
					// make coins
					stockpile.addMoney(100);
				}
				else
				{
					//std::cout<<"Unable to make coins, doing something else.\n";
					return false;
				}
				return true;
			}
		}
		

		return false;
		
		// Character spends a month in production
		// produce an item
		// item value is dependent on character stats and randomness.
		
		// determine quality
		
		// skill modifier 
		// base chance of special item is 1 in 1000. 0.1%
		// each level of metalsmithing skill can improve it by 1%
		// which means that 100 skill means 100% chance of making special item.
		//int specialChance = 1000;
		// DEBUG SETTING:
		// int specialChance = 100;
		// int skillModifier = character->skillMetalsmithing.level * 10;
		// int totalChance = specialChance - skillModifier;
		// if ( totalChance < 1 )
		// {
			// totalChance = 1;
		// }
		
		// int qualityLevel = 0;
		
		// if (random.oneIn(totalChance) )
		// {
			// std::cout<<"SPECIAL ITEM produced by "<<character->getFullName()<<".\n";
			
			// qualityLevel = 1;
			
			// while (qualityLevel < 6 )
			// {
				// if ( random.flip() )
				// {
					// ++qualityLevel;
				// }
				// else
				// {
					// continue;
				// }
			// }
			// std::cout<<"Quality level is "<<qualityLevel<<"\n";
			
		// }
		// else
		// {
			// // normal item
			// //std::cout<<"Item produced by "<<vCharacter(i)->getFullName()<<".\n";
		// }
		
		// Item* createdItem=0;
		
		// if (farmingEquipmentNeeded())
		// {
			// Item_Hoe * hoe = new Item_Hoe();
			// createdItem=hoe;
			// vItem.push(hoe);
			// stockpile.add(hoe);
		// }
		// else
		// {
		
			// Item_Sword * sword = new Item_Sword();
			// createdItem=sword;
			// stockpile.add(sword);
			// vItem.push(sword);
		
		// }
		
		// if ( qualityLevel > 0 )
		// {				
			// // if the item is special, create an Item_Information attachment for it.
			// Item_Information* info = new Item_Information();
			// // Who created it
			// // Where it was created
			// // Any additional info
			
			// info->creator = character;
			// info->locationMade = this;
			// info->quality = qualityLevel;
			// info->yearMade = globalCalendar.year;
			// info->monthMade = globalCalendar.month;
			// createdItem->information = info;
			// // engravings should be done seperately as it's a different skillset.
			// // Player should be able to pick the engraving if they are having it done.
			
		// }
		
		// if (qualityLevel == 6)
		// {
			// std::cout<<"*** ARTIFACT CREATED ***\n";
			// if (world!=0)
			// {
				// world->eventManager.addEvent("ARTIFACT",Event::EVENT_NONE);
			// }
		// }
		
		// //nMetalStockpile-=10;
		// stockpile.take(RESOURCE_IRON, 1);
		// character->skillMetalsmithing.addExp(10);
	}
	return false;
}

void Settlement_Dwarven::abstractDayResearch(Character* character)
{
	if ( globalRandom.rand8(DAYS_PER_MONTH) == 0 )
	{
	//std::cout<<character->getFullName()<<": Research. "<<character->getMoney()<<" money.\n";
	// Character works in the mines for the month
		bool breakthrough = character->abstractResearchMonth();
	}
}

void Settlement_Dwarven::abstractDaySocial(Character* character)
{
	if ( globalRandom.rand8(DAYS_PER_MONTH) == 0 )
	{
		// Every month the Character does a round of socialising
		const char cSocialSize = character->getCharisma();
		
		// Number of social interactions is dependant on charisma
		for (int i2=0;i2<cSocialSize;++i2)
		{
			character->abstractSocial(getRandomCharacter());
		}
		character->updateSocial();
	}
}

Character* Settlement_Dwarven::getMiner(Vector <Character*>* vExclude)
{
	Character* currentBest = nullptr;
	int highestSkillMining = -1;  // Initialize with a low value
	for (int i=0;i<vCharacter.size();++i)
	{
		Character * character = vCharacter(i);
		
		// Check if the character is in the exclusion list
		bool isExcluded = false;
		for (int j = 0; j < vExclude->size(); ++j)
		{
			if ((*vExclude)(j) == character)
			{
				isExcluded = true;
				break;
			}
		}

		// If the character is not excluded and has a higher skillFarming, update the current best
		if (!isExcluded && (currentBest == nullptr || character->skillMining > highestSkillMining))
		{
			currentBest = character;
			highestSkillMining = character->skillMining;
		}
	}
	return currentBest;
}

bool Settlement_Dwarven::miningNeeded() // True if any more mining resources are required.
{
	// For now just assume we need 1 of everything for each person.
	if ( stockpile.get(RESOURCE_STONE) < vCharacter.size() )
	{
		return true;
	}
	else if ( stockpile.get(RESOURCE_IRON) < vCharacter.size() / 2 )
	{
		return true;
	}
	return false;
}

bool Settlement_Dwarven::woodNeeded() // True if any more mining resources are required.
{
	// For now just assume we need 1 of everything for each person.
	if ( stockpile.get(RESOURCE_WOOD) < vCharacter.size() )
	{
		return true;
	}
	return false;
}

bool Settlement_Dwarven::coinsNeeded() // True if any more coins are required for the treasury
{
	return false;
}

Character* Settlement_Dwarven::getFarmer(Vector <Character*>* vExclude)
{
	Character* currentBest = nullptr;
	int highestSkillFarming = -1;  // Initialize with a low value
	for (int i=0;i<vCharacter.size();++i)
	{
		Character * character = vCharacter(i);
		
		// Check if the character is in the exclusion list
		bool isExcluded = false;
		for (int j = 0; j < vExclude->size(); ++j)
		{
			if ((*vExclude)(j) == character)
			{
				isExcluded = true;
				break;
			}
		}

		// If the character is not excluded and has a higher skillFarming, update the current best
		if (!isExcluded && (currentBest == nullptr || character->skillFarming > highestSkillFarming))
		{
			currentBest = character;
			highestSkillFarming = character->skillFarming;
		}
	}
	return currentBest;
}

void Settlement_Dwarven::moveAllCharacterHome()
{
	const std::vector<enumLocation> preferredLocations = {
		LOCATION_DWELLING,
		LOCATION_MAIN_HALL,
		LOCATION_HALL
	};

	for (auto &v : vCharacter)
	{
		bool moved = false;
		for (const auto &location : preferredLocations)
		{
			if (v->moveToLocationType(location))
			{
				moved = true;
				break;
			}
		}

		if (!moved)
		{
			//std::cout<<v->getFullName()<<" unable to return home due to lack of capacity.\n";
		}
		else
		{
			//std::cout<<v->getFullName()<<" returned home.\n";
		}
	}
}



/* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement_Dwarven::incrementTicks ( int nTicks )
{
	for (auto & v: vCharacter)
	{
		if(v->isAlive==false)
		{
			vCharacter.erase(v);
			vDeceased.add(v);
		}
	}
	
	if ( vCharacter.size() == 0 )
	{
		// Settlement is wiped out
		//isAlive = false;
		//std::cout<<"ret\n";
		return;
	}

	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;

	while (dailyCounter >= TICKS_PER_DAY)
	{
		//std::cout<<"\n*** Daily tick: "<<globalCalendar.toString()<<"\n\n";
		moveAllCharacterHome(); // send all characters home for now to reset location capacities
		//std::cout<<"\n";
		
		//technology.print();
		//std::cout<<"\n";
		
		Vector <Character*> vMovedCharacters; // List of characters already acted.
		
		vCharacter.shuffle();
		
		
		if (government.needsLeader())
		{
			//std::cout<<"We need a leader\n";
		}
		
		government.governDaily();
		
		//Character* character = vCharacter(i);
		
		while (vMovedCharacters.size() < vCharacter.size())
		{

			// FOOD
			// Calculate how much food we need.
			// Progression should be foraging/hunting -> farming
			int neededFood = ((vCharacter.size()-vMovedCharacters.size())*28) - stockpile.get(RESOURCE_FOOD);
			
			Character * actingCharacter = nullptr;
			
			if ( neededFood > 0)
			{
				actingCharacter = getFarmer(&vMovedCharacters);
				//abstractMonthFood(actingCharacter);
				if (abstractDayJob(actingCharacter, new Job_Farming()))
				{
				}
				else if (abstractDayJob(actingCharacter, new Job_Hunting()))
				{
				}
			}
			else
			{
				actingCharacter = getCharacter(&vMovedCharacters);
				stockpile.take(RESOURCE_FOOD, 28);
				
				// CONSTRUCTION
				if ( abstractDayConstruction(actingCharacter))
				{
				}
				// PRODUCTION
				else if ( abstractDayProduction(actingCharacter))
				{
				}
				// MINING
				else if ( miningNeeded() )
				{
					abstractDayJob(actingCharacter, new Job_Mining());
				}
				else if ( woodNeeded() )
				{
					abstractDayJob(actingCharacter, new Job_Woodcutting());
				}
				// RESEARCH
				else
				{
					abstractDayResearch(actingCharacter);
				}
			}
			
			// SOCIAL
			abstractDaySocial(actingCharacter);
			//character->social.print();
			
			vMovedCharacters.push(actingCharacter);
		
		}
		
		// POLITICS AND OTHER COMPLEX ACTIONS
		// Includes exploration etc.
		// Item bartering
		
		abstractDayBiology();
		abstractDaySplit();
		
		//resourceManager.print();
		stockpile.print();
		//printAllMoneyInSettlement();
		//requestManager.print();
		//locationRequestManager.print();
		location.printAll();
		dailyCounter-=TICKS_PER_DAY;
	}
	

	// while ( dailyCounter >= TICKS_PER_DAY )
	// {
		// dailyCounter-=TICKS_PER_DAY;
	// }
}

void Settlement_Dwarven::abstractDayBiology()
{
	for (int i=0; i<vCharacter.size(); ++i)
	{
		Character* const c = vCharacter(i);
		
		c->incrementTicks(TICKS_PER_DAY);
		
		if (globalRandom.rand8(DAYS_PER_MONTH) == 0 )
		{
	
			if (c->isMale == false && c->isMarried == true && c->age >= 18 && c->isAlive && c->spouse!=0 && c->spouse->isAlive)
			{
				if (c->isPregnant==false)
				{
					if (c->vChildren.size() < 6 && c->age < 35)
					{
						if(random.oneIn(10))
						{
							c->isPregnant = true;
							c->pregnantCounter = 0;
						}
					}
					else if (c->age < 55)
					{
						if(random.oneIn(c->age))
						{
							c->isPregnant = true;
							c->pregnantCounter = 0;
						}
					}
					else
					{

					}
				}

				else if (c->isPregnant == true && c->pregnantCounter >= 3) // 3 months of pregnancy
				{
					Character* babby = c->giveBirth();
					if ( babby!= 0) {
						vCharacter.push(babby);
					}
					c->isPregnant=false;
					c->pregnantCounter = 0;
					babby->settlement=this;
				}
				else if ( c->isPregnant == true)
				{
					c->pregnantCounter++;
				}
			}

			// Marriage searching. 18+. 10% chance per month.
			if (c->isMale == true && c->age >= 16 && c->isMarried == false && random.oneIn(12))
			{
				// build vector of unmarried women
				Vector <Character*> vEligibleWomen;

				// People that c can't marry
				auto vRelatives = c->getRelatives();

				//Select maxRandoms random candidates from the vector.
				int maxRandoms = 20;
				while ( vEligibleWomen.size() < 3 && maxRandoms-- > 0)
				{
					Character* c2 = vCharacter(random.randomInt(vCharacter.size()-1));

					if ( c2!=c && c->canMarry(c2) && vRelatives->contains(c2)==false )
					{
						vEligibleWomen.push(c2);
					}
				}

				// If the character can't find an eligible woman in the tribe.
				// Look through a neighboring tribe. (Sometimes check anyway.)
				// Tribe* neighboringTribe = world->getNearestConnectedTribe(this,true /* samerace */);

				// if ( (vEligibleWomen.size() == 0 || random.oneIn(20) ) && neighboringTribe != 0 && neighboringTribe->vCharacter.size() > 0)
				// {
					// //std::cout<<"Checking nearest tribe.\n";
					// //Select maxRandoms random candidates from the vector.
					// int maxRandoms2 = 20;
					// while ( vEligibleWomen.size() < 3 && maxRandoms2-- > 0)
					// {
						// Character* c2 = neighboringTribe->vCharacter(random.randomInt(neighboringTribe->vCharacter.size()-1));

						// if ( c2!=c && c->canMarry(c2) && vRelatives->contains(c2)==false )
						// {
							// vEligibleWomen.push(c2);
							// //std::cout<<"Success\n";
							// //Console("SUCC");
						// }
					// }
				// }

				if(vEligibleWomen.size()>0)
				{
					const int randomWoman = random.randomInt(vEligibleWomen.size()-1);

					// Order of marriage is randomised mostly to deal with movement between tribes.
					if ( random.flip() )
					{	vEligibleWomen(randomWoman)->marry(c);
					}
					else
					{	c->marry(vEligibleWomen(randomWoman));
					}
				}
			}
		
			//Death calculations
			if ( c->age < 50 && random.oneIn(3000))
			{
				c->die();
			}
			else if (c->age > 70 && random.oneIn(50))
			{
				c->die();
			}
			else if (c->age > 65 && random.oneIn(180))
			{
				c->die();
			}
			else if (c->age >= 50 && random.oneIn(800))
			{
				c->die();
			}
		}
	}
}

void Settlement_Dwarven::abstractDaySplit()
{
	
	if ( globalRandom.rand8(DAYS_PER_MONTH*8) == 0  && getPopulation() > 100 )
	{
		std::cout << "Split check at " << getPopulation() << "\n";
		double splitProbability = 1;
		
		if (getPopulation() < 1000 )
		{
			splitProbability = 100 - (static_cast<double>(getPopulation()) / 1000.0) * 100;
		}
			std::cout<<"Split probability given as rand("<<splitProbability<<").\n";

		if (globalRandom.rand(splitProbability) == 0)
		{
			std::cout << "Split at pop: " << getPopulation() << "\n";

		}
		else
		{
			return;
		}

		
		// NEW SETTLEMENT CALCULATIONS
		// OCCURS IF: TOO MANY PEOPLE IN SETTLEMENT, THERE IS A VIABLE AMOUNT OF FREE SPACE, RANDOM ELEMENT.
		// ONLY SPLIT INTO EMPTY TERRITORY.
		int landmassID = world->aWorldTile(worldX,worldY).landID;
		// WHAT LANDMASS ARE WE ON?
		// DOES THE LANDMASS HAVE A SPARE TILE?
		// NEW IDEA: Only split if there are at least 3 free tiles. This will reduce requirement
		// To check entire landmass. And prevent tribes from splitting into single free tiles.
		int nFreeTiles = 0;

		Vector <HasXY*> * vXY  = world->aWorldTile.getNeighbors(worldX, worldY, false, true /* shuffle */);

		if (vXY==0) {
			return;
		}

		bool canExpand = false;
		for (auto xy : *vXY)
		{
			if (world->aWorldTile.isSafe(xy) && world->isLand(xy))
			{
				nFreeTiles++;
			}
		}

		if (nFreeTiles >= 3 && landmassID >=0 )
		{
			// CHECK TO SEE IF THERE'S A TILE TO SPLIT INTO.
			for (auto xy : *vXY)
			{
				if (world->aWorldTile.isSafe(xy) && world->isLand(xy) )
				{
					if ( world->getHighestInfluence(xy) == 0 )
					{
						// WHO WILL LEAVE THE TRIBE? FOR NOW LET'S RANDOMLY PICK UNMARRIED INDIVIDUALS.
						// ONLY FORM NEW TRIBE IF THERE ARE AT LEAST 20 CANDIDATES.
						// NOTE THAT WE MUST SHUFFLE VECTOR FOR NOW. THE VECTOR SHOULD BE SHUFFLED EXTERNALLY AT A SET TIME PERIOD.

						// UPDATE. THERE IS NO LIMIT TO HOW MANY PEOPLE MAY LEAVE THE ORIGINAL TRIBE.
						vCharacter.shuffle();
						Vector <Character*> vNewTribeCharacter;

						for (int i=0; i<vCharacter.size(); ++i)
						{
							if ( vCharacter(i)->isMarried == false && Random::oneIn(3))
							{
								vNewTribeCharacter.push(vCharacter(i));
							}

						}
						if (vNewTribeCharacter.size() >= 20)
						{
							// FORM THE NEW SETTLEMENT AND BREAK.
							Console ("SETTLEMENT SPLIT");
							std::cout<<"Settlement split\n";
							
							Tribe_Dwarven* splitTribe = new Tribe_Dwarven;
							splitTribe->world = world;
							splitTribe->name = globalNameGen.generate();
							splitTribe->nFood = 10;
							splitTribe->worldX = xy->x;
							splitTribe->worldY = xy->y;
							splitTribe->setColour(random.randomInt(255),random.randomInt(255),random.randomInt(255));
							world->putObject(splitTribe);
							world->vTribe.push(splitTribe);
							
							//splitTribe->generateCouples(12);

							for (int i2=0; i2<vNewTribeCharacter.size(); ++i2)
							{
								splitTribe->vCharacter.push(vNewTribeCharacter(i2));
								removeCharacter(vNewTribeCharacter(i2));
							}


							// Add legends event
							Event_Settlement_Split * eventSplit = new Event_Settlement_Split("SETTLEMENT SPLIT FROM "+getName(),Event::EVENT_SETTLEMENT_SPLIT,this,splitTribe);
							world->eventManager.addEvent(eventSplit);
							break;
						}
						break;
					}
				}
			}
		}

		vXY->clearData();
		delete vXY;
	}
}

#endif
  