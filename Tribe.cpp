#pragma once
#ifndef WORLDSIM_TRIBE_CPP
#define WORLDSIM_TRIBE_CPP

/* WorldSim: Tribe.cpp
	#include "Tribe.cpp"

	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization.

*/

#include "Tribe.hpp"

#include "World.hpp"

#include <Graphics/Texture/Texture.hpp>

#include <NameGen/NameGen.hpp>

Tribe::Tribe()
{
	name="N/A";
	world=0;
	nFood=0;
	actionPoints=0;
	
	colourRed=0;
	colourGreen=0;
	colourBlue=0;
	
	dailyCounter=0;
	monthlyCounter=0;
	
	foundSettlement=false;
	
	worldX=-1;
	worldY=-1;
  
  race = NONE;

}

Tribe::~Tribe()
{

	for (int i=0;i<vCharacter.size();++i)
	{
		delete vCharacter(i);
	}
	vCharacter.clear();
	
	for (int i=0;i<vTerritory.size();++i)
	{
		delete vTerritory(i);
	}
	vTerritory.clear();
	
}

void Tribe::init(World* _world)
{
	name="N/A";
	world=0;
	nFood=0;
	actionPoints=0;
	
	colourRed=0;
	colourGreen=0;
	colourBlue=0;
	
	dailyCounter=0;
	monthlyCounter=0;
	
	world = _world;
	
	foundSettlement=false;
	
	worldX=-1;
	worldY=-1;
	
	
	for (int i=0;i<vCharacter.size();++i)
	{
		delete vCharacter(i);
	}
	vCharacter.clear();
	
	for (int i=0;i<vTerritory.size();++i)
	{
		delete vTerritory(i);
	}
	vTerritory.clear();
	
	random.seed(Random::randInt(99999));
	setColour(random.randomInt(255),random.randomInt(255),random.randomInt(255));
}

int Tribe::getPopulation()
{
	int population = 0;
	
	for (int i=0;i<vCharacter.size();++i)
	{
		if(vCharacter(i)->isAlive)
		{
			++population;
		}
	}
	
	return population;
}

void Tribe::degradeInfluence()
{
	world->degradeInfluence(this);
	
	if (world->getDominantInfluence(this->worldX,this->worldY) != this)
	{
		for ( int i=0; i<vTerritory.size();++i)
		{
			if ( vTerritory(i)->x == this->worldX && vTerritory(i)->y == this->worldY)
			{
				vTerritory.removeSlot(i);
				--i;
			}
		} 	
	}
	
}

void Tribe::incrementTicks ( int nTicks )
{
	//std::cout<<"tribe increment ticks\n";
	//std::cout<<"Increment ticks: "<<nTicks<<".\n";
	actionPoints+=nTicks;
	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;
	
	for (auto & v: vCharacter)
	{
		if(v->isAlive==false)
		{
			vCharacter.erase(v);
		}
	}
	vCharacter.shrinkToFit();
	
	
	for (int i=0;i<vCharacter.size();++i)
	{
		Character* c = vCharacter(i);
		c->incrementTicks(nTicks);
	}
	
	while (monthlyCounter >= 2592000)
	{
		lastline=1;	
		//if ( vCharacter.size
		//std::cout<<"begin monthly counter.\n";
		
		degradeInfluence();
		//std::cout<<"influence degraded\n";
		//std::cout<<"Monthly\n";
		//Do monthly stuff here.
		//for (int i=0;i<vCharacter.size();++i)
			
		// if ( random.oneIn(100) )
		// {
			// Character * c = new Character;
			// c = 0;
			// c->die();
		// }
		
		//for (auto & c: vCharacter)
		for (int i=0;i<vCharacter.size();++i)
		{
			//lastline=2;
			//std::cout<<"char\n";
			//break;
			//std::cout<<"Character\n";
			Character* const c = vCharacter(i);
			
			
			if (c->isMale == false && c->isMarried == true && c->age >= 18 && c->isAlive && c->spouse!=0 && c->spouse->isAlive)
			{
				if (c->isPregnant==false)
				{
					if (c->vChildren.size() < 4 && c->age < 35)
					{
						if(random.oneIn(26))
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
				
				else if (c->isPregnant == true && c->pregnantCounter >= 10)
				{
						Character* babby = c->giveBirth();
						if ( babby!= 0) { vCharacter.push(babby); }
						c->isPregnant=false;
						c->pregnantCounter = 0;
						babby->tribe=this;
				}
				else if ( c->isPregnant == true)
				{
					c->pregnantCounter++;
				}
			}

			// Marriage searching. 18+. 10% chance per month.
			if (c->isMale == true && c->age >= 18 && c->isMarried == false && random.oneIn(48))
			{
				//std::cout<<"marriage\n";
				
				// build vector of unmarried women
				Vector <Character*> vEligibleWomen;
				
				
				// std::cout<<"Relatives of: "<<*c<<".\n";
				
				// for(auto v: *vRelatives)
				// {
					// std::cout<<"  "<<*v<<"\n";
				// }
				
					// People that c can't marry
				auto vRelatives = c->getRelatives();
				
				//Select 1000 random candidates from the vector.
				int maxRandoms = 200;
				while ( vEligibleWomen.size() < 100 && maxRandoms-- > 0)
				{
					Character* c2 = vCharacter(random.randomInt(vCharacter.size()-1));
					
					if ( c2!=c && c->canMarry(c2) && vRelatives->contains(c2)==false )
					{
						vEligibleWomen.push(c2);
					}
				}
				
				// for (int i2=0;i2<vCharacter.size();++i2)
				// {
					// Character* c2 = vCharacter(i2);
					// if (c->canMarry(c2) && vRelatives->contains(c2)==false)
					// {
						// vEligibleWomen.push(c2);
					// }
				// }
				//std::cout<<"There are "<<vEligibleWomen.size()<<" eligible women.\n";
			

				
				if(vEligibleWomen.size()>0)
				{
					const int randomWoman = random.randomInt(vEligibleWomen.size()-1);
					//std::cout<<"Marriage: "<<c->getFullName()<<" and "<<vEligibleWomen(randomWoman)->getFullName()<<".\n";
					c->marry(vEligibleWomen(randomWoman));
				}
				//std::cout<<"end marriage\n";
			}
			//lastline=5;
			
			
				//Death calculations
			if ( c->age < 50 && random.oneIn(2400))
			{
				//std::cout<<"Death: "<<*c<<".\n";
				c->die();
			}
			else if (c->age > 70 && random.oneIn(50))
			{
				//std::cout<<"Death: "<<*c<<".\n";
				c->die();
			}
			else if (c->age > 65 && random.oneIn(180))
			{
				//std::cout<<"Death: "<<*c<<".\n";
				c->die();
			}
			else if (c->age >= 50 && random.oneIn(600))
			{
				//std::cout<<"Death: "<<*c<<".\n";
				c->die();
			}
//std::cout<<"end char\n";
//lastline=6;	
		}
		//lastline=7;	
		monthlyCounter-=2592000;
		//std::cout<<"end monthly counter.\n";
	}
	///lastline=8;	
	
	while ( dailyCounter >= 86400 )
	{
		//std::cout<<"Dayloop for tribe: "<<name<<".\n";
		//Do daily stuff here.
		
		// Get food for the day.
		// Get resources for the day
		// Get tech for the day
		// 
		
		if ( foundSettlement == false )
		{
			wander();
		}
		
		updateTerritory();
		
		//hunt();
		
	//	eat();
		
		develop();
		
		
		
		dailyCounter-=86400;
	}
	
	//if ( actionPoints
	
	// Hourly
	while( actionPoints >= 3600 )
	{
		
		
		actionPoints-=3600;
	}
//std::cout<<"end tribe increment ticks\n";
}

void Tribe::wander()
{
	if (world==0) { return; }
	
	int destinationX = worldX + random.randomIntRange(-1,1);
	int destinationY = worldY + random.randomIntRange(-1,1);
	
	// Moving options:
	//  Move to food
	//  Move to unexplored territory
	
	//aTerrain.getNeighborVector(_x,_y,&vTerrain,false /* DON'T INCLUDE SELF */);
	Vector <HasXY*> * vXY  = world->aTerrain.getNeighbors(worldX, worldY, false);
	vXY->shuffle();
	
	HasXY* xyDestination = 0;
	
	// Explore new territory
	if (random.oneIn(4))
	{
		//std::cout<<"1\n";
		for (auto xy : *vXY)
		{
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getHighestInfluence(xy) == 0 )
				{
					xyDestination=xy;
					
					worldX=xy->x;
					worldY=xy->y;
					return;
				}
			}
		}
	}
	
	//if ( random.oneIn(2) )
	{
			/* If there is no unowned territory. Prefer own territory. Prefer weakest influence tiles to promote holding on to territory */
		for (auto xy : *vXY)
		{
			
			int weakestInfluence = -1;
			HasXY * weakestTile = 0;
			
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getDominantInfluence(xy) == this)
				{
					if ( weakestInfluence == -1 || weakestTile == 0 || world->getHighestInfluence(xy) < weakestInfluence)
					{
						xyDestination=xy;
						weakestInfluence = world->getHighestInfluence(xy);
					}
				}
			}
		}
		if ( xyDestination != 0)
		{
			worldX=xyDestination->x;
			worldY=xyDestination->y;
			return;
		}
	}
	
		/* Sometimes move to a weak territory to reinforce/capture it */
	if ( random.oneIn(3) )
	{
		//std::cout<<"2\n";
			/* Move to weakest territory */
		int weakestInfluence = -1;
		HasXY * weakestTile = 0;
			
		for (auto xy : *vXY)
		{
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getHighestInfluence(xy)>0 && world->getHighestInfluence(xy) < weakestInfluence)
				{
					xyDestination=xy;
					weakestInfluence = world->getHighestInfluence(xy);
				}
			}
		}
		if ( xyDestination != 0)
		{
			worldX=xyDestination->x;
			worldY=xyDestination->y;
			return;
		}
	}
	
	

		


	//std::cout<<"4\n";
		/* If all else fails, move randomly. */
	if (world->isSafe(destinationX,destinationY) && world->isLand(destinationX,destinationY))
	{
		// MOVE THE TRIBE TO THE LOCATION.
		worldX=destinationX;
		worldY=destinationY;
	}
}

void Tribe::hunt()
{
	int huntingYield = world->getHuntingYield(worldX,worldY);
	int huntedFood = random.randomInt(huntingYield);
	
	//std::cout<<"The tribe has gone hunting for food in the "<<world->getTileType(worldX,worldY)<<". They have gathered "<<huntedFood<<" food.\n";
	
	int nHunters = 0;
	for(auto const& character: vCharacter)
	{
		if ( character->age > 16 && character->age < 60)
		{
			++nHunters;
		}
	}
	//std::cout<<"Hunters: "<<nHunters<<".\n";
	
	//nFood+=huntedFood*nHunters;

		// Limit surplus food tribes can carry.
	//if (nFood > vCharacter.size() * 3) { nFood = vCharacter.size() * 3; }
}

void Tribe::eat()
{
	//nFood -= vCharacter.size();
	
	if ( nFood < vCharacter.size() )
	{
		//std::cout<<"Tribe is starving.\n";
		
		for (auto character : vCharacter)
		{
			if ( random.oneIn(10) )
			{
				character->starve();
			}
			
		}
		
		//Kill random person.
	}
}

void Tribe::updateTerritory()
{
	// Remove territory no longer owned.
	// Add current tile if necessary.
	
	world->addInfluence(this, 100);
	
	if ( random.oneIn(10) == false )
	{
		return;
	}
	
	bool currentTileChecked = false;
	nFood = 0; /* Update food capacity */
	
	for ( int i=0; i<vTerritory.size();++i)
	{
		if ( vTerritory(i)->x == this->worldX && vTerritory(i)->y == this->worldY )
		{
			currentTileChecked=true;
		}
		
		if ( world->getDominantInfluence(vTerritory(i)) != this)
		{
			delete vTerritory(i);
			vTerritory.removeSlot(i);
			--i;
		}
		else
		{
			nFood+= world->getTileFertility(vTerritory(i));
		}
	}
	
	if (currentTileChecked == false)
	{
		if ( world->getDominantInfluence(worldX,worldY) == this)
		{
			vTerritory.add(new HasXY (worldX,worldY));
			nFood+= world->getTileFertility(worldX,worldY);
		}	
	}
	
}

// This function is called when a Tribe is ready to develop into a Civilisation.
// Tribe object will be deleted and replaced with Civilisation object.
void Tribe::develop()
{
  if (foundSettlement == false)
  {
    //std::cout<<"tribe develp\n";
    
    
    //if ( vTerritory.findSlot(this) != -1 )
    //{
    //}
    
    //Update territory array

    //updateInfluence();
    
    // When a tribe gets big, it has a chance of splitting into smaller tribes. (only if there is space, otherwise there will be conflict instead.)
    
    if (vCharacter.size() > 200 && random.oneIn(50) && world->getSurroundingFertility(worldX,worldY) > 100)
    {
      //std::string cMessage = "Settlement founded: "+name;
      //vConsoleMessage.push(cMessage);
      consoleMessage(Stream() << "Settlement founded: " << name);

      foundSettlement=true;
    }
  }
}

void Tribe::setColour( const unsigned char r, const unsigned char g, const unsigned char b)
{
	colourRed=r;
	colourGreen=g;
	colourBlue=b;
}

bool Tribe::spawn()
{
	if ( world == 0 )
	{
		return false;
	}
	
	HasXY* spawnTile = world->getRandomTileOfType(GRASSLAND);
	
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Couldn't find land tile.\n";
		return false;
	}

	name = globalNameGen.generateName();
	nFood = 10;
	
	worldX=spawnTile->x;
	worldY=spawnTile->y;

	world->putObject(this,worldX, worldY);
	world->vTribe.push(this);
	
	return true;

		// /* Adjust search box so it is fully inside the map. */
	// int randomX2 = randomX1 + 20;
	// int randomY2 = randomY1 + 20;
	
	// while ( randomX2 > world->nX )
	// {
		// --randomX2;
		// --randomX1;
	// }
	// while ( randomY2 > world->nY )
	// {
		// --randomY2;
		// --randomY1;
	// }

	// int bestFertility = -1;
	// int bestFertilityX = -1;
	// int bestFertilityY = -1;
	
	// for ( int _y = randomY1; _y<= randomY2; ++_y )
	// {
		// for ( int _x = randomX1; _x<= randomX2; ++_x )
		// {
		
			// /* MAKE SURE IT IS A VALID PLACE TO PUT THE CITY... */
			
			// /* CONDITIONS:
				// MUST BE GRASSLAND.
				// CANNOT BE ON TOP OF AN EXISTING TRIBE.
				// CANNOT BE WITHIN HOSTILE BORDERS.
			// */

			// if (world->aTerrain(_x,_y)==World::GRASSLAND)
			// {
				// // MAKE SURE THERE ISN'T A TRIBE HERE OR NEARBY.
			
				// const int fertility = world->getSurroundingFertility(_x,_y);
				// if ( fertility > bestFertility )
				// {
				
					// bool goodCity = true;
				
					// for ( int i=0;i<world->vTribe.size();++i)
					// {
						// if ( _x == world->vTribe(i)->worldX && _y == world->vTribe(i)->worldY )
						// {
							// goodCity=false;
						// }
					// }
					
					// if ( goodCity == true )
					// {
						// bestFertility = fertility;
						// bestFertilityX = _x;
						// bestFertilityY = _y;
					// }

				// }
			// }
		// }
	// }
	
	
	// //std::cout<<"Best pos in box: ("<<bestFertilityX<<", "<<bestFertilityY<<"). Value: "<<bestFertility<<".\n";
	
	// if(bestFertility==-1 || bestFertilityX==-1 || bestFertilityY==-1 )
	// {
		// //std::cout<<"ERROR: No spawn tile found. Finding any tile.\n";
		// if (world->getRandomLandTile(&bestFertilityX,&bestFertilityY) == false)
		// {
			// //std::cout<<"ERROR: Couldn't find any land tile.\n";
			// return false;
		// }
		// bestFertility=1;

		
	// }
	// if ( bestFertility==0 )
	// {
		// //std::cout<<"ABORT: Fertility 0.\n";
		// return false;
	// }
	
	// //init(world);
	
	// worldX = bestFertilityX;
	// worldY = bestFertilityY;
	// name = globalNameGen.generateName();
	// nFood = 10;
	

	// //world->putObject(this);
	// //world->vTribe.push(this);

	// //std::cout<<"Tribe spawned: "<<name<<". At "<<worldX<<", "<<worldY<<".\n";

	
	// /* Populate tribe */
	// //generateCitizens(12);
	
	// return true;

}


Texture* Tribe::currentTexture()
{
	return &TEX_TEST_404;
}


void Tribe::generateCouples(int amount)
{
	while (amount-- > 0)
	{
		Character * cMan = new Character;
		cMan->init();
		cMan->isMale = true;
		cMan->age = 18+random.randomInt(18);
		cMan->tribe = this;
		Character * cWoman = new Character;
		cWoman->init();
		cWoman->isMale = false;
		cWoman->age = 16+random.randomInt(12);
		cWoman->tribe=this;
	
		cMan->marry(cWoman);
		
		vCharacter.push(cMan);
		vCharacter.push(cWoman);
	}
}


/* TABLE INTERFACE */

std::string Tribe::getColumn(std::string _column)
{
	if ( _column=="name" )
	{
		return name;
	}
	if ( _column=="race" )
	{
		if (race == NONE)
    {
      return "None";
    }
		if (race == HUMAN)
    {
      return "Human";
    }
		if (race == DWARVEN)
    {
      return "Dwarven";
    }
		if (race == ELVEN)
    {
      return "Elven";
    }
    
	}
	if ( _column=="food" )
	{
		return DataTools::toString(nFood);
	}
	if ( _column=="population" )
	{
		return DataTools::toString(vCharacter.size());
	}
	if ( _column=="coordinates" )
	{
		return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	}
	if ( _column=="territory" )
	{
		return DataTools::toString(vTerritory.size());
	}
	// else if ( _column=="id" )
	// {
		// return DataTools::toString(id);
	// }
	
	return "?";
}
std::string Tribe::getColumnType(std::string _column)
{
	if ( _column == "population" || _column == "territory" || _column == "food" )
	{
		return "number";
	}
	return "string";
}


#endif
