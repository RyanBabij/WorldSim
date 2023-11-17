#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_CPP
#define GUILD_SETTLEMENT_DWARVEN_CPP

#include "Settlement_Dwarven.hpp"

Settlement_Dwarven::Settlement_Dwarven(): Settlement()
{
	race = DWARVEN;

	nFoodStockpile=10;
	nMetalStockpile=0;
	monthlyCounter=0;
	dailyCounter=0;

	name.insert(0, "Fort ");
}

Texture* Settlement_Dwarven::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

/* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement_Dwarven::incrementTicks ( int nTicks )
{
	//std::cout<<"Incrementing Dwarven settlement.\n";
	
	// Abstract logic flow should be:
	// Food
	// Mining
	// Manufacture
	// Tech should improve outputs
	
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
		if (government.needsLeader())
		{
			std::cout<<"We need a leader\n";
		}
		
		government.govern();
		
		
		// Produce and deduct food
		nFoodStockpile -= vCharacter.size();
		std::cout<<"nfood: "<<nFoodStockpile<<"\n";
		// Add metal
		nMetalStockpile+=(vCharacter.size()/2);
		// Research
		// Manufacture
		
		// For now we will turn the metal into weapons for use and export.
		vCharacter.shuffle();
		
		for (int i=0;i<vCharacter.size();++i)
		{
			Character* character = vCharacter(i);
			//std::cout<<"Abstracting "<<character->getFullName()<<"\n";
			
			// Social
			
			const char cSocialSize = character->getCharisma();
			
			// Number of social interactions is dependant on charisma
			for (int i2=0;i2<cSocialSize;++i2)
			{
				character->abstractSocial(getRandomCharacter());
			}
			character->updateSocial();
			
			//character->social.print();
			
			// Research
			
			bool breakthrough = character->abstractResearchMonth();
			
			// if (breakthrough)
			// {
				
				// Technology::TECHNOLOGY_TYPE techAdvance = technology.advance();
				// Console ("Tech up: "+technology.techToString(techAdvance));
				
				// Event_Tech_Breakthrough * eventTech = new Event_Tech_Breakthrough
				// ("BREAKTHROUGH IN "+technology.techToString(techAdvance),Event::EVENT_TECH_BREAKTHROUGH);
				
				// world->eventManager.addEvent(eventTech);
				// //world->eventManager.addEvent("TECH ADVANCE",Event::EVENT_TECH_BREAKTHROUGH);
			// }
			
		
			if (nMetalStockpile > 10 )
			{
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
					std::cout<<"SPECIAL ITEM produced by "<<vCharacter(i)->getFullName()<<".\n";
					
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
				
				Item_Sword * sword = new Item_Sword();
				vItem.push(sword);
				
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
					info->yearMade = world->calendar.year;
					info->monthMade = world->calendar.month;
					sword->information = info;
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
				
				nMetalStockpile-=10;
				character->skillMetalsmithing.addExp(10);
			}
			else
			{
				continue;
			}
		
		}
		
		
		
		if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
		{
			nMetalStockpile+=30;
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
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		for (int i=0; i<vCharacter.size(); ++i)
		{
			Character* const c = vCharacter(i);

			//std::cout<<"Incrementing: "<<c->getFullName()<<".\n";
			c->incrementTicks(nTicks);
		
		

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
		monthlyCounter-=TICKS_PER_MONTH;
	}

	while ( dailyCounter >= TICKS_PER_DAY )
	{
		if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
		{
			++nMetalStockpile;
		}
		dailyCounter-=TICKS_PER_DAY;
	}
	
	

	return;
}


#endif
  