#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_CPP
#define GUILD_SETTLEMENT_DWARVEN_CPP

#include "Settlement_Dwarven.hpp"

#include "ItemManager.hpp"
#include "ItemRequest.cpp"

#include "Job.cpp"

Settlement_Dwarven::Settlement_Dwarven(): Settlement()
{
	race = DWARVEN;

	//nFoodStockpile=10;
	//nMetalStockpile=0;
	monthlyCounter=0;
	dailyCounter=0;

	name.insert(0, "Fort ");
}

Texture* Settlement_Dwarven::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

// void Settlement_Dwarven::abstractMonthJob(enumJob Job)
// {
// }

void Settlement_Dwarven::abstractMonthFood(Character* character)
{
	std::cout<<character->getFullName()<<": Farming. "<<character->getMoney()<<" money.\n";
	// for now assume the Character feeds themself plus a certain surplus
	
	// Personal output is how much a person's personal skill can affect output.
	// Technology can further improve the output as an independent variable.
	// I think without aid of tech, subsistence farming ratio of 1 farmer for 1 other person is reasonable.
	// Over time, technology of course should make a huge difference until farmers become a small minority.
	
	// Farming equipment presence and quality should also be important.
	
	// Check if we can borrow a hoe if we don't own one.
	
	//Item* usedItem = 0;
	//Item* bestFarmingEquipment = character->getBestItemFor(JOB_FARMING);
	Item* bestFarmingEquipment = character->getBestItemFor(Job_Farming());
	
	// // get best farming equipment from stockpile and compare
	Item* bestStockpileFarmingEquipment = stockpile.getBestItemFor(Job_Farming());
	
	if ( bestFarmingEquipment == nullptr )
	{
		if ( bestStockpileFarmingEquipment == nullptr ) // There's no farming equipment anywhere
		{
			if ( character->getMoney() > 0 )
			{
				
				
				int marketValue = requestManager.getAverageValue(ITEM_HOE) + 1;
				
				int amountCanPay = character->getMoney();
				if (marketValue < amountCanPay)
				{
					amountCanPay = marketValue;
				}
				
				requestManager.removeAll(character,ITEM_HOE);
				requestManager.add(character,ITEM_HOE,marketValue);
				
				std::cout<<"Request for hoe at price of "<<amountCanPay<<".\n";
			}
		}
		else // we don't have equipment but the stockpile does
		{
			stockpile.take(bestStockpileFarmingEquipment);
			character->giveItem(bestStockpileFarmingEquipment);
		}
	}
	else // we have equipment and the stockpile does too
	{
		if (bestStockpileFarmingEquipment != nullptr &&
			bestStockpileFarmingEquipment->attributes.farmingValue > bestFarmingEquipment->attributes.farmingValue )
			{
				// equip item from stockpile.
				//bestFarmingEquipment = bestStockpileFarmingEquipment;
				stockpile.take(bestStockpileFarmingEquipment);
				character->giveItem(bestStockpileFarmingEquipment);
			}
	}
	
	// if (( bestFarmingEquipment != nullptr ) && bestFarmingEquipment == bestStockpileFarmingEquipment )
	// {
		// // take this item from the stockpile
		// stockpile.take(bestFarmingEquipment);
	// }
	
	// If we don't have farming equipment, put in a request if possible.
	// if ( bestFarmingEquipment == nullptr && character->getMoney() > 0)
	// {
		// std::cout<<"Request for hoe: "<<character->getMoney()<<"\n";
		// requestManager.add(character,ITEM_HOE,character->getMoney());
	// }
	
	Item* farmingEquipment = character->getBestItemFor(Job_Farming());
	
	int farmingItemMult=1;
	if ( farmingEquipment != nullptr )
	{
		farmingItemMult = farmingEquipment->attributes.farmingValue+1;
	}
	
	int maxPersonalOutput = (character->getStrength()*farmingItemMult)+character->skillFarming;
	//int maxPersonalOutput = (8+character->skillFarming)*farmingItemMult;
	if (maxPersonalOutput > 28)
	{
		maxPersonalOutput=28;
	}
	int maxTotalOutput = maxPersonalOutput + technology.agricultureLevel;
	
	resourceManager.addFood(globalRandom.rand(maxTotalOutput));
	
	character->skillUpFarming();
	payCharacterFromTreasury(character,1);

}
void Settlement_Dwarven::abstractMonthMine(Character* character)
{
	std::cout<<character->getFullName()<<": Mining. "<<character->getMoney()<<" money.\n";
	// Character works in the mines for the month
	
	
	
	// Check if we can borrow a hoe if we don't own one.
	
	//Item* usedItem = 0;
	Item* bestMiningEquipment = character->getBestItemFor(Job_Farming());
	
	// // get best farming equipment from stockpile and compare
	Item* bestStockpileMiningEquipment = stockpile.getBestItemFor(Job_Farming());
	
	if ( bestMiningEquipment == nullptr )
	{
		if ( bestStockpileMiningEquipment == nullptr ) // There's no farming equipment anywhere
		{
			if ( character->getMoney() > 0 )
			{
				
				
				int marketValue = requestManager.getAverageValue(ITEM_HOE) + 1;
				
				int amountCanPay = character->getMoney();
				if (marketValue < amountCanPay)
				{
					amountCanPay = marketValue;
				}
				
				requestManager.removeAll(character,ITEM_HOE);
				requestManager.add(character,ITEM_HOE,marketValue);
				
				std::cout<<"Request for hoe at price of "<<amountCanPay<<".\n";
			}
		}
		else // we don't have equipment but the stockpile does
		{
			stockpile.take(bestStockpileMiningEquipment);
			character->giveItem(bestStockpileMiningEquipment);
		}
	}
	else // we have equipment and the stockpile does too
	{
		if (bestStockpileMiningEquipment != nullptr &&
			bestStockpileMiningEquipment->attributes.farmingValue > bestMiningEquipment->attributes.farmingValue )
			{
				// equip item from stockpile.
				//bestMiningEquipment = bestStockpileMiningEquipment;
				stockpile.take(bestStockpileMiningEquipment);
				character->giveItem(bestStockpileMiningEquipment);
			}
	}
	
	
	int maxPersonalOutput = 14+character->skillMining;
	if (maxPersonalOutput > 28)
	{
		maxPersonalOutput=28;
	}
	int maxTotalOutput = maxPersonalOutput + technology.miningLevel;
	
	resourceManager.addStone(globalRandom.rand(maxTotalOutput));
	resourceManager.addIron(globalRandom.rand(maxTotalOutput/2));
	
	character->skillUpMining();
	payCharacterFromTreasury(character,1);
}

bool Settlement_Dwarven::produceItem(ItemType type)
{
	if ( type == ITEM_HOE )
	{
		if ( resourceManager.canMake(Item_Hoe::getResourceRequirement()) )
		{
			resourceManager.deductResources(Item_Hoe::getResourceRequirement());
			
			Item_Hoe * hoe = new Item_Hoe();
			stockpile.add(hoe);
			return true;
		}	
	}
	
	return false;
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
		resourceManager.addMoney(taxAmount);
	}
	else
	{
		// If 3 or less, give all to the character without tax
		character->giveMoney(moneyToReceive);
	}
}

void Settlement_Dwarven::payCharacterFromTreasury(Character* character, int amount)
{
	int amountToGive = resourceManager.takeMoneyUpTo(amount);
	character->giveMoney(amountToGive);
}

bool Settlement_Dwarven::abstractMonthProduction(Character* character)
{
	auto mostValuableRequestOpt = requestManager.pullMostValuableRequest(false);
	if (mostValuableRequestOpt)
	{
		// If there is a most valuable request
		ItemRequest mostValuableRequest = *mostValuableRequestOpt;
		// Process the most valuable request
		//std::cout << "Most valuable request value: " << mostValuableRequest.value << std::endl;
		
		if (produceItem(mostValuableRequest.type))
		{
			std::cout<<character->getFullName()<<": Producing item. "<<character->getMoney()<<" money.\n";
			std::cout<<"There are "<<requestManager.getNumContracts()<<" contracts.\n";
			// recieve the coins promised
			
			int moneyToRecieve = mostValuableRequest.value;
			
			payCharacter(character, mostValuableRequest.value);
			return true;
		}
		else // Couldn't make the item, put the request back through.
		{
			// Recursive try to make something else.
			abstractMonthProduction(character);
			
			mostValuableRequest.requester->giveMoney(mostValuableRequest.value);
			requestManager.add(mostValuableRequest.requester,mostValuableRequest.type, mostValuableRequest.value);
			std::cout<<"Something messed up\n";
		}
	}
	else if (getMoneyPercentInTreasury() < 0.25 || getAverageCharacterWealth() < 100 )
	{
		std::cout<<character->getFullName()<<": Producing coins. "<<character->getMoney()<<" money.\n";
		
		for (int i=0;i<10;++i)
		{
			if ( resourceManager.takeIron(1) )
			{
				// make coins
				resourceManager.addMoney(10);
			}
			else if (i == 0) // couldn't make any coins.
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		return true;
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
	int specialChance = 100;
	int skillModifier = character->skillMetalsmithing.level * 10;
	int totalChance = specialChance - skillModifier;
	if ( totalChance < 1 )
	{
		totalChance = 1;
	}
	
	int qualityLevel = 0;
	
	if (random.oneIn(totalChance) )
	{
		std::cout<<"SPECIAL ITEM produced by "<<character->getFullName()<<".\n";
		
		qualityLevel = 1;
		
		while (qualityLevel < 6 )
		{
			if ( random.flip() )
			{
				++qualityLevel;
			}
			else
			{
				continue;
			}
		}
		std::cout<<"Quality level is "<<qualityLevel<<"\n";
		
	}
	else
	{
		// normal item
		//std::cout<<"Item produced by "<<vCharacter(i)->getFullName()<<".\n";
	}
	
	Item* createdItem=0;
	
	if (farmingEquipmentNeeded())
	{
		Item_Hoe * hoe = new Item_Hoe();
		createdItem=hoe;
		vItem.push(hoe);
		stockpile.add(hoe);
	}
	else
	{
	
		Item_Sword * sword = new Item_Sword();
		createdItem=sword;
		stockpile.add(sword);
		vItem.push(sword);
	
	}
	
	if ( qualityLevel > 0 )
	{				
		// if the item is special, create an Item_Information attachment for it.
		Item_Information* info = new Item_Information();
		// Who created it
		// Where it was created
		// Any additional info
		
		info->creator = character;
		info->locationMade = this;
		info->quality = qualityLevel;
		info->yearMade = globalCalendar.year;
		info->monthMade = globalCalendar.month;
		createdItem->information = info;
		// engravings should be done seperately as it's a different skillset.
		// Player should be able to pick the engraving if they are having it done.
		
	}
	
	if (qualityLevel == 6)
	{
		std::cout<<"*** ARTIFACT CREATED ***\n";
		if (world!=0)
		{
			world->eventManager.addEvent("ARTIFACT",Event::EVENT_NONE);
		}
	}
	
	//nMetalStockpile-=10;
	resourceManager.takeIron(1);
	character->skillMetalsmithing.addExp(10);
}

void Settlement_Dwarven::abstractMonthResearch(Character* character)
{
	std::cout<<character->getFullName()<<": Research. "<<character->getMoney()<<" money.\n";
	// Character works in the mines for the month
	bool breakthrough = character->abstractResearchMonth();
}

void Settlement_Dwarven::abstractMonthSocial(Character* character)
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
	if ( resourceManager.getStone() < vCharacter.size() )
	{
		return true;
	}
	else if ( resourceManager.getIron() < vCharacter.size() / 2 )
	{
		return true;
	}
	return false;
}

bool Settlement_Dwarven::coinsNeeded() // True if any more coins are required for the treasury
{
	return false;
}

bool Settlement_Dwarven::farmingEquipmentNeeded()
{
	int nFarmingEquipment = stockpile.getNumOfType(ITEM_HOE);
	if ( nFarmingEquipment < vCharacter.size() / 2 )
	{
		return true;
	}
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

	while (monthlyCounter >= TICKS_PER_MONTH)
	{
		std::cout<<"\n*** Monthly tick: "<<globalCalendar.toString()<<"\n\n";
		technology.print();
		
		Vector <Character*> vMovedCharacters; // List of characters already acted.
		
		vCharacter.shuffle();
		
		
		if (government.needsLeader())
		{
			std::cout<<"We need a leader\n";
		}
		
		government.govern();
		
		//Character* character = vCharacter(i);
		
		while (vMovedCharacters.size() < vCharacter.size())
		{

			// FOOD
			// Calculate how much food we need.
			// Progression should be foraging/hunting -> farming
			int neededFood = ((vCharacter.size()-vMovedCharacters.size())*28) - resourceManager.getFood();
			
			Character * actingCharacter = nullptr;
			
			if ( neededFood > 0)
			{
				actingCharacter = getFarmer(&vMovedCharacters);
				abstractMonthFood(actingCharacter);
			}
			else
			{
				actingCharacter = getCharacter(&vMovedCharacters);
				resourceManager.takeFood(28);
				// MINING
				if ( miningNeeded() )
				{
					abstractMonthMine(actingCharacter);
				}
				// PRODUCTION
				else if (abstractMonthProduction(actingCharacter))
				{
				}
				// RESEARCH
				else
				{
					abstractMonthResearch(actingCharacter);
				}
			}
			
			// SOCIAL
			abstractMonthSocial(actingCharacter);
			//character->social.print();
			
			vMovedCharacters.push(actingCharacter);
		
		}
		
		// POLITICS AND OTHER COMPLEX ACTIONS
		// Includes exploration etc.
		// Item bartering
		
		abstractMonthBiology();
		abstractMonthSplit();
		resourceManager.print();
		stockpile.print();
		printAllMoneyInSettlement();
		monthlyCounter-=TICKS_PER_MONTH;
	}
	

	while ( dailyCounter >= TICKS_PER_DAY )
	{
		dailyCounter-=TICKS_PER_DAY;
	}
}

void Settlement_Dwarven::abstractMonthBiology()
{
	for (int i=0; i<vCharacter.size(); ++i)
	{
		Character* const c = vCharacter(i);
		
		c->incrementTicks(TICKS_PER_MONTH);
	
		if (c->isMale == false && c->isMarried == true && c->age >= 18 && c->isAlive && c->spouse!=0 && c->spouse->isAlive)
		{
			if (c->isPregnant==false)
			{
				if (c->vChildren.size() < 6 && c->age < 35)
				{
					if(random.oneIn(24))
					{
						c->isPregnant = true;
						c->pregnantCounter = 0;
					}
				}
				else if (c->age < 55)
				{
					if(random.oneIn(c->age*3))
					{
						c->isPregnant = true;
						c->pregnantCounter = 0;
					}
				}
				else
				{

				}
			}

			else if (c->isPregnant == true && c->pregnantCounter >= 9)
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
		if (c->isMale == true && c->age >= 16 && c->isMarried == false && random.oneIn(38))
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
		if ( c->age < 50 && random.oneIn(2400))
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
		else if (c->age >= 50 && random.oneIn(600))
		{
			c->die();
		}
	}
}

void Settlement_Dwarven::abstractMonthSplit()
{
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

	if (nFreeTiles >= 3 && landmassID >=0 && vCharacter.size() > 150 && random.oneIn(12) )
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


#endif
  