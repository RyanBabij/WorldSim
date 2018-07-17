#pragma once
#ifndef WORLDSIM_TRIBE_HUMAN_CPP
#define WORLDSIM_TRIBE_HUMAN_CPP

/* Tribe_Human.hpp
	#include "Tribe_Human.hpp"

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Human tribes build towns and cities.

*/

#include "Tribe_Human.hpp"

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

#include "WorldObjectGlobal_TribalArtifact.cpp"

Tribe_Human::Tribe_Human()
{
  race = HUMAN;
  nArtifactsDropped = 0;
  nameType = "Human Tribe";
  name = "HUM";
}



bool Tribe_Human::spawn()
{
	//return false;
	
	if ( world == 0 )
	{
		//std::cout<<"ABORT: Tribe doesn't have pointer to world object.\n";
		return false;
	}
	
	HasXY* spawnTile = world->getRandomTileOfType(GRASSLAND);
	
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Human couldn't find tile to spawn into.\n";
		return false;
	}

	name = globalNameGen.generateName();
	nFood = 10;
	
	worldX=spawnTile->x;
	worldY=spawnTile->y;

	world->putObject(this,worldX,worldY);
	world->vTribe.push(this);
  

	
	return true;

}


Texture* Tribe_Human::currentTexture()
{
	if ( foundSettlement )
	{
		return &TEX_WORLD_SETTLEMENT_TOWN_URBAN01;
	}
	return &TEX_WORLD_UNIT_NOMAD_01;
}

void Tribe_Human::wander()
{
	if (world==0) { return; }
	
	int destinationX = worldX + random.randomIntRange(-1,1);
	int destinationY = worldY + random.randomIntRange(-1,1);
  
  
    // Sometimes drop an artifact on the map. (max 3)
  if (nArtifactsDropped < 3 && random.oneIn(1000))
  {
    TribalArtifact * testArtifact = new TribalArtifact;
    testArtifact->worldX = worldX;
    testArtifact->worldY = worldY;
    
    world->putObject(testArtifact);
    ++nArtifactsDropped;
  }
	
	// Moving options:
	//  Move to food
	//  Move to unexplored territory
	
	//aTerrain.getNeighborVector(_x,_y,&vTerrain,false /* DON'T INCLUDE SELF */);
	Vector <HasXY*> * vXY  = world->aTerrain.getNeighbors(worldX, worldY, false /* includeself */, true /* shuffle */);
	
	HasXY* xyDestination = 0;
  
  
  // IF THE TRIBE IS NOT IN ITS OWN TERRITORY, BUT ADJACENT TO IT, MOVE BACK.
  if ( world->getDominantInfluence(worldX,worldY) != this)
  {
		for (auto xy : *vXY)
		{
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
        // If we own this tile, move to it.
				if ( world->getDominantInfluence(xy) == this )
				{
					xyDestination=xy;
					
					worldX=xy->x;
					worldY=xy->y;
          combat(world->combatCheck(this));
					return;
				}
			}
		}
  }
  // If the tribe is in its own territory.
  else
  {
    // OCCASIONALLY MOVE RANDOMLY (INCLUDING INTO ENEMY TERRITORY)
    // UPDATE: LESS LIKELY TO DO THIS IF SMALL TRIBE.
    if ((vCharacter.size() > 100 && random.oneIn(12)) || random.oneIn(20) )
    //if (random.oneIn(12))
    {
      for (auto xy : *vXY)
      {
        if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
        {
          worldX=xy->x;
          worldY=xy->y;
          combat(world->combatCheck(this));
          return;
        }
      }
    }
    
    // Explore unclaimed territory.
    if (random.oneIn(5))
    {
      for (auto xy : *vXY)
      {
        if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
        {
          if ( world->getHighestInfluence(xy) == 0 )
          {
            xyDestination=xy;
            
            worldX=xy->x;
            worldY=xy->y;
            combat(world->combatCheck(this));
            return;
          }
        }
      }
    }
    
    
    // MOVE AROUND WITHIN OWNED TERRITORY
    for (auto xy : *vXY)
    {
      if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
      {
        if ( world->getDominantInfluence(xy)==this)
        {
          
          worldX=xy->x;
          worldY=xy->y;
          combat(world->combatCheck(this));
          return;
        }
      }
    }

    
    
      /* Otherwise move to a weak territory to reinforce/capture it */
   // if ( random.oneIn(2) )
   // {
      /* Move to weakest territory */
      // int weakestInfluence = -1;
      // HasXY * weakestTile = 0;
        
      // for (auto xy : *vXY)
      // {
        // if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
        // {
          // if ( world->getHighestInfluence(xy)>0 && world->getHighestInfluence(xy) < weakestInfluence)
          // {
            // xyDestination=xy;
            // weakestInfluence = world->getHighestInfluence(xy);
          // }
        // }
      // }
      // if ( xyDestination != 0)
      // {
        // worldX=xyDestination->x;
        // worldY=xyDestination->y;
        // return;
      // }
    //}
    

    
    
  }
	

  

	

    /* If there is no unowned territory. Prefer own territory. Prefer weakest influence tiles to promote holding on to territory */
  // for (auto xy : *vXY)
  // {
    
    // int weakestInfluence = -1;
    // HasXY * weakestTile = 0;
    
    // if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
    // {
      // if ( world->getDominantInfluence(xy) == this)
      // {
        // if ( weakestInfluence == -1 || weakestTile == 0 || world->getHighestInfluence(xy) < weakestInfluence)
        // {
          // xyDestination=xy;
          // weakestInfluence = world->getHighestInfluence(xy);
        // }
      // }
    // }
  // }
  // if ( xyDestination != 0)
  // {
    // worldX=xyDestination->x;
    // worldY=xyDestination->y;
    // return;
  // }
  
  // IF ALL ELSE FAILS, MOVE RANDOMLY (INCLUDING INTO ENEMY TERRITORY
  for (auto xy : *vXY)
  {
    if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
    {
      worldX=xy->x;
      worldY=xy->y;
    }
  }
  combat(world->combatCheck(this));

		/* If all else fails, move randomly. */
	// if (world->isSafe(destinationX,destinationY) && world->isLand(destinationX,destinationY))
	// {
		// // MOVE THE TRIBE TO THE LOCATION.
		// worldX=destinationX;
		// worldY=destinationY;
	// }
}

void Tribe_Human::combat (Tribe* _target)
{
  if (_target == 0) { return; }

  std::cout<<"Combat initiated.\n";
  std::cout<<"Attacker: "<<name<<". ("<<vCharacter.size()<<").\n";
  std::cout<<"Defender: "<<_target->name<<". ("<<_target->vCharacter.size()<<").\n";
  
  // Only Characters that want to fight will join the attack.
  // Some factors to consider are marriage, children, age, strength, starvation, etc.
  
  if ( vCharacter.size() > _target->vCharacter.size() )
  {
    //std::cout<<"Picking force.\n";
    
    Vector <Character*> vAttackingForce;
    
    vCharacter.shuffle();
    
    for (int i=0; i<vCharacter.size();++i)
    {
      Character* c = vCharacter(i);
      //if (vCharacter(i)->isMale && vCharacter(i)->age >= 15 )
      //if ( vCharacter(i)->age >= 15 )
      //{
        // Weak/pregnant/young people will be less likely to fight.
        // But they will also not get the spoils.
      if ( c->age == 15 || c-> age == 16 || c->age == 17 || c->isPregnant || c->strength < 50)
      {
        if ( random.oneIn(10) )
        {
          vAttackingForce.push(c);
        }
      }
        // Strong people will always fight, because they want the spoils.
      else if ( c->age > 17 && c->strength >= 50 )
      {
        vAttackingForce.push(c);
      }
    }
    
   // std::cout<<"Attacking with a force of "<<vAttackingForce.size()<<".\n";
   
    if ( vAttackingForce.size() <= 0 )
    {
      std::cout<<"Declined: No eligible attackers.\n";
    }
    else
    {
      for ( int i=0; i<vAttackingForce.size(); ++i )
      {
        //Character* _targetCharacter = _target->vCharacter.getRandom(random);
        Character* _targetCharacter = _target->getDefender();
        
        if ( _targetCharacter != 0 )
        {
          std::cout<<"Attacking: "<<_targetCharacter->firstName<<".\n";
          vAttackingForce(i)->attack(_targetCharacter);
        }
        else
        {
          std::cout<<"Tribe has been annihilated.\n";
          _target->kill();
        }

      }
    }
    
    // if ( vAttackingForce.size() > _target->vCharacter.size() )
    // {
      // // DIRECT ATTACK ON TARGET CAMP.
      // std::cout<<"The target was annihilated.\n";
      // _target->isAlive = false;
    // }
    // else
    // {
      // // SKIRMISH / RAID / AMBUSH
      // std::cout<<"The target was ambushed.\n";
      
      // //Each attacker will pick a target.


      
    // }

    TribalArtifact_BattleSite * testArtifact = new TribalArtifact_BattleSite;
    testArtifact->worldX = worldX;
    testArtifact->worldY = worldY;
    world->putObject(testArtifact);
    
    
  }
  else
  {
    std::cout<<"Declined.\n";
  }
  
  return;
}

void Tribe_Human::incrementTicks ( int nTicks )
{

	for (auto & v: vCharacter)
	{
		if(v->isAlive==false)
		{
			vCharacter.erase(v);
		}
	}
	vCharacter.shrinkToFit();

  if ( vCharacter.size() == 0 )
  {
    isAlive = false;
    return;
    
  }
  
	//actionPoints+=nTicks;
	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;
	

  

  
  
	
	
	for (int i=0;i<vCharacter.size();++i)
	{
		Character* c = vCharacter(i);
		c->incrementTicks(nTicks);
	}
	
	while (monthlyCounter >= 2592000)
	{

		//degradeInfluence();
    
    // TRIBE SPLIT CALCULATIONS
    // OCCURS IF: TOO MANY PEOPLE IN TRIBE, THERE IS A VIABLE AMOUNT OF FREE SPACE, RANDOM ELEMENT.
    // ONLY SPLIT INTO EMPTY TERRITORY.
    int landmassID = world->aLandmassID(worldX,worldY);
    // WHAT LANDMASS ARE WE ON?
    // DOES THE LANDMASS HAVE A SPARE TILE?
    // NEW IDEA: Only split if there are at least 3 free tiles. This will reduce requirement
    // To check entire landmass. And prevent tribes from splitting into single free tiles.
    int nFreeTiles = 0;
    
    Vector <HasXY*> * vXY  = world->aTerrain.getNeighbors(worldX, worldY, false, true /* shuffle */);
    bool canExpand = false;
    for (auto xy : *vXY)
    {
      if (world->aTerrain.isSafe(xy) && world->isLand(xy) && world->getHighestInfluence(xy) == 0)
      {
        //canExpand=true;
        nFreeTiles++;
        //break;
      }
    }

    if (nFreeTiles >= 3 && landmassID >=0 && vCharacter.size() > 150 && random.oneIn(8) ) //&& world->nFreeTerritory(landmassID) > 12 
    {
      //std::cout<<"TRIBAL SPLIT\n";
      Console ("TRIBAL SPLIT");



      // CHECK TO SEE IF THERE'S A TILE TO SPLIT INTO.
      for (auto xy : *vXY)
      {
        if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
        {
          if ( world->getHighestInfluence(xy) == 0 )
          {
            //xyDestination=xy;
            

            // WHO WILL LEAVE THE TRIBE? FOR NOW LET'S RANDOMLY PICK UNMARRIED INDIVIDUALS.
            // ONLY FORM NEW TRIBE IF THERE ARE 20 CANDIDATES.
            // NOTE THAT WE MUST SHUFFLE VECTOR FOR NOW. THE VECTOR SHOULD BE SHUFFLED EXTERNALLY AT A SET TIME PERIOD.
            vCharacter.shuffle();
            
            Vector <Character*> vNewTribeCharacter;
            
            for (int i=0;i<vCharacter.size();++i)
            {
              if ( vCharacter(i)->isMarried == false )
              {
                vNewTribeCharacter.push(vCharacter(i));
                
                if (vNewTribeCharacter.size() >= 20)
                {
                  // FORM THE NEW TRIBE AND BREAK.
                  Tribe_Human* splitTribe = new Tribe_Human;
                  splitTribe->world = world;
                  splitTribe->name = globalNameGen.generateName();
                  splitTribe->nFood = 10;
                  splitTribe->worldX = xy->x;
                  splitTribe->worldY = xy->y;
                  splitTribe->setColour(random.randomInt(255),random.randomInt(255),random.randomInt(255));
                  world->putObject(splitTribe);
                  world->vTribe.push(splitTribe);
                  
                  for (int i2=0;i2<vNewTribeCharacter.size();++i2)
                  {
                    splitTribe->vCharacter.push(vNewTribeCharacter(i2));
                    removeCharacter(vNewTribeCharacter(i2));
                  }
                  
                  
                  //std::cout<<"New tribe formed.\n";
                  break;
                }
              }
              
            }
            break;
          }
        }
      }

      

      
      //civ->vCharacter.copy(&_tribe->vCharacter);
    }
    
    vXY->clearData();
    delete vXY;
    
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
			Character* const c = vCharacter(i);
			
			
			if (c->isMale == false && c->isMarried == true && c->age >= 18 && c->isAlive && c->spouse!=0 && c->spouse->isAlive)
			{
				if (c->isPregnant==false)
				{
					if (c->vChildren.size() < 4 && c->age < 35)
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
				
				// build vector of unmarried women
				Vector <Character*> vEligibleWomen;
				
					// People that c can't marry
				auto vRelatives = c->getRelatives();
				
				//Select maxRandoms random candidates from the vector.
				int maxRandoms = 100;
				while ( vEligibleWomen.size() < 100 && maxRandoms-- > 0)
				{
					Character* c2 = vCharacter(random.randomInt(vCharacter.size()-1));
					
					if ( c2!=c && c->canMarry(c2) && vRelatives->contains(c2)==false )
					{
						vEligibleWomen.push(c2);
					}
				}
        
				if(vEligibleWomen.size()>0)
				{
					const int randomWoman = random.randomInt(vEligibleWomen.size()-1);
					c->marry(vEligibleWomen(randomWoman));
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
    
    //develop();
		monthlyCounter-=2592000;
	}
	///lastline=8;	
	
	while ( dailyCounter >= 86400 )
	{
		
		if ( foundSettlement == false )
		{
			wander();
		}
		
		updateTerritory();
		
		//hunt();
		
	//	eat();
		
		
		
		
		
		dailyCounter-=86400;
	}
	
	//if ( actionPoints
	
	// Hourly
	// while( actionPoints >= 3600 )
	// {
		
		
		// actionPoints-=3600;
	// }
//std::cout<<"end tribe increment ticks\n";
}

// void Tribe_Human::kill ()
// {
  // for (int i=0; i<vCharacter.size(); ++i)
  // {
    // //vCharacter(i)
  // }
// }


#endif