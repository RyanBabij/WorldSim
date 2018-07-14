#pragma once
#ifndef GUILD_CIV_CPP
#define GUILD_CIV_CPP

#include "Civ.hpp"
#include "Settlement.hpp"

Civ::Civ()
{
	name="N/A";
	money=100;
	world=0;

	//nFood=0;
}

void Civ::init(World* _world)
{
	//std::cout<<"Civ::init().\n";
	world = _world;
	aVisible.init(world->nX,world->nY,false);
		// DEBUG.
	//aVisible.init(world->nX,world->nY,true);
	
	//std::cout<<"Generating family name pool.\n";
	// for(int i=0;i<25;++i)
	// {
		// familyNamePool.push(globalNameGen.generateName());
		// //std::cout<<"Name: "<<familyNamePool(i)<<".\n";
	// }

}

void Civ::addSettlement(Settlement * _settlement)
{
  _settlement->world = world;
  vSettlement.push(_settlement);
}

			// // RETURN THE NUMBER OF NEIGHBORING FOG TILES. (0-8).
		// int neighboringFog ( int _x, int _y );
			// // RETURN THE NEIGHBORING TILE WITH THE MOST FOG. IF ALL OR SOME TILES ARE EQUALLY FOGGY, RETURN A RANDOM ONE OF THE FOGGIEST ONES.
		// int foggiestNeighbor (int _x, int _y, int* _rx, int* _ry);
		
// int Civ::neighboringFog ( int _x, int _y )
// {
	// return aVisible.nNeighborsEqual(_x,_y,false);

	// //return 0;
// }

// void Civ::getExploreTile(int* _rx, int* _ry, bool _land = false)
// {
	// // For now set it to a random tile to test pathfinding.
	// world->getRandomTile(_rx,_ry);
// }

// void Civ::foggiestNeighbor (const int _x, const int _y, int* _rx, int* _ry, const bool _landOnly = false)
// {
	// // GET ALL SURROUNDING TILES.
	// Vector <HasXY*> * vNeighbors = aVisible.getNeighbors(_x,_y,false);
	
	// HasXY* foggiestTile = 0;
	// int mostFog = 0;
	
	// // CHECK EACH SURROUNDING TILE.
	// for (int i=0;i<vNeighbors->size();++i)
	// {
		// // COUNT THE FOG SURROUNDING THIS TILE.
		// HasXY* currentNeighbor = (*vNeighbors)(i);
		// //Vector <HasXY*>* vFogCount = aVisible.getNeighbors(currentNeighbor,false);
		// Vector <bool>* vFogCount = aVisible.getNeighborValues(currentNeighbor,false);
		
		// int totalFog = vFogCount->count(false);
		
		// // for (int i2=0;i2<vFogCount->size();++i2)
		// // {
			// // if ( aVisible((*vFogCount)(i2)) == false )
			// // {
				// // ++totalFog;
			// // }
		// // }
	// //	std::cout<<"Totalfog for this tile: "<<totalFog<<".\n";
		
	// }


	// //std::cout<<"Looking for foggiest neighbor...\n";

	// //Vector < ArrayS2 <bool>::Index * > * vIndex = aVisible.getNeighbors(_x,_y,false);
	// Vector < HasXY* > * vIndex = aVisible.getNeighbors(_x,_y,false);
	// //Vector <ArrayS2::Index*> * vIndex = aVisible.getNeighbors(_x,_y,false);
	
	// if ( _landOnly == true )
	// {
		// for ( int i=0; i<vIndex->size(); ++i)
		// {
			// //ArrayS2<bool>::Index* currentIndex=(*vIndex)(i);
			// HasXY* currentIndex=(*vIndex)(i);
			
			
			
		// }
	// }

	// //ArrayS2<bool>::Index* bestIndex=0;
	// HasXY* bestIndex=0;
	// int mostFog = 0;
	// //Vector < ArrayS2<bool>::Index* > vCandidates;
	// Vector < HasXY* > vCandidates;
	
	// for ( int i=0; i<vIndex->size(); ++i)
	// {
		// //ArrayS2<bool>::Index* currentIndex=(*vIndex)(i);
		// HasXY* currentIndex=(*vIndex)(i);
		

		
		// //std::cout<<"Checking: "<<currentIndex->x<<", "<<currentIndex->y<<".\n";
		
		// if (bestIndex==0)
		// {
			// //std::cout<<"Set default.\n";
			// bestIndex = currentIndex;
			// mostFog = aVisible.nNeighborsEqual(bestIndex,false);
			// vCandidates.push(bestIndex);
			// //std::cout<<"Mostfog: "<<mostFog<<".\n";
		// }
		// else
		// {
			// //std::cout<<"Fog: "<<(int)aVisible.nNeighborsEqual(currentIndex,false)<<".\n";
			// if ( aVisible.nNeighborsEqual(currentIndex,false) > mostFog )
			// {
				// //std::cout<<"Superior.\n";
				// bestIndex = currentIndex;
				// mostFog = aVisible.nNeighborsEqual(bestIndex,false);
				// vCandidates.clear();
				// vCandidates.push(bestIndex);
			// }
			// else if ( aVisible.nNeighborsEqual(currentIndex,false) == mostFog )
			// {
				// //std::cout<<"Equal.\n";
				// if ( Random::flip() == true )
				// {
					// bestIndex = currentIndex;
					// mostFog = aVisible.nNeighborsEqual(bestIndex,false);
					// vCandidates.push(bestIndex);
				// }
			// }
		// }
	// }
	
	// if ( bestIndex == 0 )
	// {
		// std::cout<<"ERROR: No good index.\n";
		// return;
	// }
	
	// //std::cout<<"Current position: "<<_x<<", "<<_y<<".\n";
	
	// if ( vCandidates.size() == 0 )
	// {
		// //std::cout<<"No candidates.\n";
		// return;
	// }
	// else if ( vCandidates.size() == 1 )
	// {
		// //std::cout<<"single candidate.\n";
	// }
	// else
	// {
		// //std::cout<<"Multiple candidates.\n";
		// bestIndex = vCandidates( Random::randomInt(vCandidates.size()-1) );
	// }
	
	
	// //std::cout<<"Foggiest neighbor: "<<bestIndex->x<<", "<<bestIndex->y<<".\n";
	
	// if ( _rx!=0 && _ry!=0 )
	// {
		// *_rx=bestIndex->x;
		// *_ry=bestIndex->y;
	// }
// }


// void Civ::addContact(Civ* _c)
// {
	// vAware.push(_c);
// }

// void Civ::updateFogOfWar()
// {
		// // REVEAL FOG AROUND CITIES.
	// for ( int i=0;i<vCity.size();++i)
	// {
		// City* c = vCity(i);
		// //aVisible.setNeighbors(c->worldX,c->worldY,5,true);
		
		// if ( PLAYER_CIV == this)
		// {
			// //std::cout<<"PC\n";
			// //aPlayerFogOfWar.setNeighbors(c->worldX,c->worldY,true);
		// }
	
	// }
	
	// for ( int i=0;i<vArmy.size();++i)
	// {
		// Army* a = vArmy(i);
		// aVisible.setNeighbors(a->worldX,a->worldY,true);
		
		// if ( PLAYER_CIV == this)
		// {
			// //std::cout<<"PC\n";
			// //aPlayerFogOfWar.setNeighbors(a->worldX,a->worldY,true);
		// }
	
	// }
	
	// // NOW DO ANOTHER PASS TO SEE IF WE'VE UNCOVERED SOME CIVS. ALSO, CIV DISCOVERY IS ALWAYS MUTUAL.
	// // THE IMPLICATION OF THIS IS THAT ANY OVERLAP OF LINE OF SIGHT BETWEEN 2 CIVS IS EQUIVALENT TO DISCOVERY.
	
	// //for ( int i=0;i<vCiv.size();++i)
	// //{
		
	// //}
	
	
// }

// bool Civ::knowsAbout(Civ* _civ)
// {
	// if ( _civ==this ) { return true; }
	
	// // for ( int i=0;i<vAware.size();++i)
	// // {
		// // if(vAware(i)==_civ)
		// // {
			// // return true;
		// // }
	// // }
	// // return false;
	
	// return vAware.contains(_civ);
// }

// bool Civ::atWarWith(Civ* _civ)
// {
	// if ( _civ==this ) { return false; }
	
	// return vAtWar.contains(_civ);

// }

// bool Civ::canSee(const int _x, const int _y)
// {
	// return (aVisible(_x,_y));
// }

void Civ::incrementTicks ( int nTicks )
{
 // std::cout<<"Civ is incrementing.\n";
	//std::cout<<"Incrementing "<<nTicks<<" ticks.\n";
	// for (int i=0;i<nTicks;++i)
	// {
		// if( Random::randomInt(100) == 0 )
		// {
			// std::cout<<"Founding new city.\n";
			// foundCity();
		// }
	// }

	//std::cout<<"Civ "<<name<<" is incrementing by "<<nTicks<<" ticks.\n";
	
	//collectTaxes();
	
	/* Update each city. */
	
	for ( int i=0;i<vSettlement.size();++i)
	{
		vSettlement(i)->incrementTicks(nTicks);
	}
	
	//nFood = 0;
	//for ( int i=0;i<vCity.size();++i)
	//{
	//	nFood += vCity(i)->nFood;
	//}
	
	//std::cout<<"Money: "<<money<<".\n";
	// if ( money > 100 && vArmy.size() < 2 )
	// {
		// //std::cout<<"Make army.\n";
		// money -= 100;
		
		// Army* a = new Army;
		// a->addSoldiers(100);
		// a->worldX = vCity(0)->worldX;
		// a->worldY = vCity(0)->worldY;
		// a->world=world;
		// a->owner=this;
		// world->vWorldObjectGlobal.push(a);
		// vArmy.push(a);
	// }
	
	/* IF THE CIV HAS ENOUGH UNITS ON GARRISON DUTY, THEN ASSIGN SOME UNITS TO EXPLORE. */
	//int _nCities = vCity.size();
	
	// int _nGarrison = 0;
	// for ( int i=0;i<vArmy.size();++i)
	// {
		// Army* a = vArmy(i);
		// if (a->isGarrison() == true)
		// {
			// ++_nGarrison;
		// }
	// }
	// if ( _nGarrison>=_nCities)
	// {
		// //std::cout<<"Time to explore.\n";
		// for (int i=0;i<vArmy.size();++i)
		// {
			// Army* a = vArmy(i);
			// if (a->isIdle() == true)
			// {
				// Objective * o = new Objective;
				// o->type=Objective::EXPLORE;
				// world->getRandomTile(&o->targetX,&o->targetY);
				// a->giveObjective(o);
				// std::cout<<"Army has recieved objective: EXPLORE.\n";
				
				// world->putObject(new WorldMarker,o->targetX,o->targetY);
			// }
		// }
	// }
	// else
	// {
		// //std::cout<<"Assign garrisons.\n";
		// for ( int i=0;i<vArmy.size();++i)
		// {
			// Army* a = vArmy(i);
			// if (a->isIdle() == true)
			// {
				// Objective * o = new Objective;
				// o->type=Objective::GARRISON;
				// a->giveObjective(o);
			// }
		// }
	// }
	
	
	// for ( int i=0;i<vArmy.size();++i)
	// {
		// vArmy(i)->incrementTicks(nTicks);
	// }
	
	// /* How many people are at war with us? */
	// int nEnemies = vAtWar.size();
	// int nAware = vAware.size();
	
		// // If we have no enemies, then attack someone.
	// if (nEnemies==0)
	// {
		// if(nAware>0)
		// {
			// Civ* civToAttack = vAware(Random::randomInt(nAware-1));
		
			// vAtWar.push(civToAttack);
			// civToAttack->vAtWar.push(this);
			
			// std::cout<<name<<" has declared war on "<<civToAttack->name<<".\n";
		// }
	// }
	
	
	//updateFogOfWar();
}

// void Civ::manageArmy()
// {
// }

// void Civ::collectTaxes()
// {
	// money+=10;
// }

// void Civ::logicTick()
// {
	// return;
	// std::cout << "CIV\n";

	// nSoldiers++;

	// collectTaxes();

	
	// if(money>1000)
	// {
		// foundCity();
	// }
	
	// if (Random::oneIn(1000))
	// {
		// std::cout<<name<<" wants to declare war.\n";
		
		// //int nCiv = world->vCiv.size();
		
		// Civ* targetCiv = world->vCiv(Random::randomInt(world->vCiv.size()-1));
		// if(targetCiv != this)
		// {
			// std::cout<<name<<" declared war on "<<targetCiv->name<<".\n";
			// std::cout<<name<<" has "<<nSoldiers<<" soldiers. "<<targetCiv->name<<" has "<<targetCiv->nSoldiers<<" soldiers.\n";
		// }
	// }
	
	
	// if(Random::oneIn(100))
	// {
		// /* Citizens look for partner. */
		// for (int i=0;i<vCitizen.size();++i)
		// {
			// vCitizen(i)->lookForPartner();
			// //vCitizen(i)->updateAge();
		// }
	// }
// }

// City* Civ::foundCity()
// {
	// std::cout<<"CIV::FOUNDCITY().\n";

	// if ( world == 0 )
	// {
		// std::cout<<"ABORT: Civ doesn't have pointer to world object.\n";
		// return 0;
	// }

		// // IF THE CIV HAS NO CITIES, THEN PLACE ONE RANDOMLY IN THE WORLD.
	// //if (vCity.size()==0)
	// //{
		// //std::cout<<"Spawning in civ: " <<name<<".\n";
	
		// int randomX1=0;
		// int randomY1=0;
		
		// if (world->getRandomLandTile(&randomX1,&randomY1) == false)
		// {
			// std::cout<<"ABORT: Couldn't find land tile.\n";
			// return 0;
		// }
		
		// //For now let's just place a city anywhere on land.
		

		// // // ADJUST THE SEARCH BOX SO IT IS FULLY WITHIN THE MAP.
		// // int randomX2 = randomX1 + 24;
		// // int randomY2 = randomY1 + 24;
		// // while ( randomX2 > world->nX )
		// // {
			// // --randomX2;
			// // --randomX1;
		// // }
		// // while ( randomY2 > world->nY )
		// // {
			// // --randomY2;
			// // --randomY1;
		// // }
		// // int bestFertility = -1;
		// // int bestFertilityX = -1;
		// // int bestFertilityY = -1;
		
		
		// // // FIND THE BEST FERTILITY TILE WITHIN THE GIVEN BOX.
		// // for ( int _y = randomY1; _y<= randomY2; ++_y )
		// // {
			// // for ( int _x = randomX1; _x<= randomX2; ++_x )
			// // {
				// // // MAKE SURE IT IS A VALID PLACE TO PUT THE CITY...
				
				// // /* CONDITIONS:
					// // MUST BE GRASSLAND.
					// // CANNOT BE ON TOP OF AN EXISTING CITY.
					// // CANNOT BE WITHIN HOSTILE BORDERS.
				// // */
				
				// // if (world->aTerrain(_x,_y)==World::GRASSLAND)
				// // {
					// // // MAKE SURE THERE ISN'T A CITY HERE OR NEARBY.
				
					// // const int fertility = world->getSurroundingFertility(_x,_y);
					// // if ( fertility > bestFertility )
					// // {
						// // bool goodCity = true;
					
						// // for ( int i=0;i<world->vCity.size();++i)
						// // {
							// // if ( _x == world->vCity(i)->worldX && _y == world->vCity(i)->worldY )
							// // {
								// // goodCity=false;
							// // }
						// // }
						// // if ( goodCity == true )
						// // {
							// // bestFertility = fertility;
							// // bestFertilityX = _x;
							// // bestFertilityY = _y;
						// // }
					// // }
				// // }
			// // }
		// // }
	
		// // if(bestFertility==-1 || bestFertilityX==-1 || bestFertilityY==-1 )
		// // {
			// // std::cout<<"ABORT: No spawn tile found.\n";
			// // return 0;
		// // }
		// // //if ( bestFertility==0 )
		// // //{
		// // //	std::cout<<"ABORT: No fertile place for city.\n";
		// // //	return 0;
		// // //}
		
	// City* c = new City;
	// c->worldX = randomX1;
	// c->worldY = randomY1;
	// c->owner = this;
	// c->name = globalNameGen.generateName();
	// c->fertility = 1;
	// vCity.push(c);
	// world->vCity.push(c);
		// // REVEAL FOG AROUND CITY.
	// aVisible.setNeighbors(c->worldX,c->worldY,true,10,true);
		// // CLAIM TERRITORY AROUND CITY (BUT DO NOT OVERWRITE EXISTING TERRITORY).
	// //Civ::aOwnership.setNeighbors(c->worldX,c->worldY,this,10,true);
	// Civ::aOwnership.setNeighborsConditional(c->worldX,c->worldY,this,10,"!=",0,true);
	// c->init();
	// world->putCity(c);
	// std::cout<<name<<" founded a city: "<<c->name<<". At "<<c->worldX<<", "<<c->worldY<<".\n";		

	// // // ////
		// // City* c = new City (randomX1,randomY1,globalNameGen.generateName(),this);
		// // c->fertility = 1;
		
		// // vCity.push(c);
		// // world->vCity.push(c);
	
		// // // REVEAL FOG AROUND CITY.
		// // //aVisible.setNeighbors(c->worldX,c->worldY,true,10,true);
		// // // CLAIM TERRITORY AROUND CITY (BUT DO NOT OVERWRITE EXISTING TERRITORY).
		// // //Civ::aOwnership.setNeighborsConditional(c->worldX,c->worldY,this,10,"!=",0,true);
	
		// // c->init();
		// // world->putCity(c);
		
		// // //std::cout<<"Initial city founded.\n";
		// return c;
	
	// //}
		// // THE CIV ALREADY HAS AT LEAST ONE CITY, SO THE CITY MUST BE FOUNDED NEAR THESE.
	// // else
	// // {
		// // std::cout<<"Founding additional city.\n";
	// // }
	
	// // return 0;
	
	
	

	// // //std::cout<<"Civ: "<<name<<" wants to found another city.\n";
	// // if(world!=0)
	// // {
		// // /* Just place a random city for now. */
		// // //if(world->putRandomCity()==true)
		// // //{
		
			// // City* c = new City;
			// // c->owner = this;
			// // c->name = globalNameGen.generateName();
			
			// // c->init();
			
			// // //std::cout<<"Testing city coords.\n";
			
			// // // (*c)(0,0);
			// // // (*c)(5,5);
			// // // (*c)(10,10);
			// // // (*c)(11,11);
			// // // (*c)(12,12);
			
			// // world->putCity(c);
		
			// // std::cout<<name<<" founded a city: "<<c->name<<".\n";
			// // //money-=1000;
		// // //}
		// // return c;
	// // }
	// // return 0;
// }

void Civ::setColour( const int r, const int g, const int b)
{
	colourRed=r;
	colourGreen=g;
	colourBlue=b;
}

// City* foundRandomCity()
// {
	
// }

// bool Civ::spawn()
// {
	// std::cout<<"Spawning in civ: " <<name<<".\n";
	
	// if ( world == 0 )
	// {
		// std::cout<<"ABORT: Civ doesn't have pointer to world object.\n";
		// return false;
	// }
	
	// int randomX1=0;
	// int randomY1=0;
	// if (world->getRandomLandTile(&randomX1,&randomY1) == false)
	// {
		// std::cout<<"ABORT: Couldn't find land tile.\n";
		// return false;
	// }

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
	
	// //std::cout<<"Meta box: ("<<randomX1<<", "<<randomY1<<") ("<<randomX2<<", "<<randomY2<<").\n";
	
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
				// CANNOT BE ON TOP OF AN EXISTING CITY.
				// CANNOT BE WITHIN HOSTILE BORDERS.
			// */
			// //	std::cout<<"Trying: "<<_x<<", "<<_y<<"\n";
			
			// if (world->aTerrain(_x,_y)==World::GRASSLAND)
			// {
				// // MAKE SURE THERE ISN'T A CITY HERE OR NEARBY.
				// //for ( int i=0; i<
			
				// const int fertility = world->getSurroundingFertility(_x,_y);
				// if ( fertility > bestFertility )
				// {
				
					// bool goodCity = true;
				
					// for ( int i=0;i<world->vCity.size();++i)
					// {
						// if ( _x == world->vCity(i)->worldX && _y == world->vCity(i)->worldY )
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
		// std::cout<<"ERROR: No spawn tile found. Finding any tile.\n";
		// if (world->getRandomLandTile(&bestFertilityX,&bestFertilityY) == false)
		// {
			// std::cout<<"ERROR: Couldn't find any land tile.\n";
			// return false;
		// }
		// bestFertility=1;

		
	// }
	// if ( bestFertility==0 )
	// {
		// std::cout<<"ABORT: Fertility 0.\n";
		// return false;
	// }
	
// //	world->aTopoMap(bestFertilityX,bestFertilityY,0)=255;
// //	world->aTopoMap(bestFertilityX,bestFertilityY,1)=255;
// //	world->aTopoMap(bestFertilityX,bestFertilityY,2)=255;

	// //std::cout<<name<<" is founding its first city.\n";
	
		// /* SPAWN IN THE CIV'S INITIAL CITY. */
	// City* c = new City;
	
	// c->worldX = bestFertilityX;
	// c->worldY = bestFertilityY;
	
	
	// //std::cout<<"Ferts: "<<bestFertilityX<<","<<bestFertilityY<<".\n";
	
	// c->owner = this;
	// c->name = globalNameGen.generateName();
	// c->fertility = bestFertility;
	
	// vCity.push(c);
	// world->vCity.push(c);
	
		// // REVEAL FOG AROUND CITY.
	// aVisible.setNeighbors(c->worldX,c->worldY,true,10,true);
		// // CLAIM TERRITORY AROUND CITY (BUT DO NOT OVERWRITE EXISTING TERRITORY).
	// //Civ::aOwnership.setNeighbors(c->worldX,c->worldY,this,10,true);
	// Civ::aOwnership.setNeighborsConditional(c->worldX,c->worldY,this,10,"!=",0,true);
	
	// c->init();
	
	// //std::cout<<"Testing city coords.\n";
	
	// // (*c)(0,0);
	// // (*c)(5,5);
	// // (*c)(10,10);
	// // (*c)(11,11);
	// // (*c)(12,12);
	
	// world->putCity(c);

	// std::cout<<name<<" founded a city: "<<c->name<<". At "<<c->worldX<<", "<<c->worldY<<".\n";
	// //money-=1000;
	
	// // City* c = foundCity();
	// // vCity.push(c);
	
	// return true;
	
	

	
	// /* Generate initial population. */
	// /* 20 married pairs. */
	// std::cout<<"Generating 20 married pairs for:"<<name<<".\n";
	
	// for(int i=0;i<20;++i)
	// {
		// //std::cout<<"********************\n";
		// Citizen* _citizen = new Citizen;
		// _citizen->firstName = globalNameGen.generateName();
		// _citizen->lastName = globalNameGen.generateName();
		// _citizen->isMale = true;
		// _citizen->birthDate.set(&world->calendar);
		// _citizen->birthDate.year-=(Random::randomInt(24)+16);
		// //_citizen->age = Random::randomInt(24)+16;
		// _citizen->city = c;
		// //_citizen->birthDate.set(&world->calendar);
		// _citizen->world = world;
		// _citizen->civ = this;
		// _citizen->isMarried=true;
		

		// //std::cout<<"Citizen #"<<i<<": "<<_citizen->getFullName()<<".\n";
		// //std::cout<<"   Sex: "<<_citizen->getSexString()<<".\n";
		// //std::cout<<"   Age: "<<_citizen->age<<".\n";
		
		// Citizen* _citizen2 = new Citizen;
		// _citizen2->firstName = globalNameGen.generateName();
		// _citizen2->lastName = _citizen->lastName;
		// _citizen2->isMale = false;
		// //_citizen2->age = Random::randomInt(24)+16;
		// _citizen2->birthDate.set(&world->calendar);
		// _citizen2->birthDate.year-=(Random::randomInt(24)+16);
		// _citizen2->city = c;
		// //_citizen2->birthDate.set(&world->calendar);
		// _citizen2->world = world;
		// _citizen2->civ = this;
		// _citizen2->isMarried=true;
		
		// /* Marry them. */
		// _citizen->partner = _citizen2;
		// _citizen2->partner = _citizen;
		
		// //std::cout<<"Citizen #"<<i<<": "<<_citizen2->getFullName()<<".\n";
		// //std::cout<<"   Sex: "<<_citizen2->getSexString()<<".\n";
		// //std::cout<<"   Age: "<<_citizen2->age<<".\n";
		
		// vCitizen.push(_citizen);
		// vCitizen.push(_citizen2);
		// c->vCitizen.push(_citizen);
		// c->vCitizen.push(_citizen2);
	// }
	
	// c->spawnCitizens();
	// c->assignJobs();
	
	// //std::cout<<"********************\n";
	// return false;
// }

// void Civ::aiDay()
// {
	// /* Civ-wide AI goes here. */
	// if(vArmy.size()==0)
	// {
		
	// }
	
	
	// /* Do AI for all of civ's cities. */
	// for(int i=0;i<vCity.size();++i)
	// {
		// vCity(i)->aiDay();
	// }
	
	

	// garbageCollect();
	

// }

// void Civ::aiSecond()
// {

	// /* Do AI for all of civ's cities. */
	// for(int i=0;i<vCity.size();++i)
	// {
		// vCity(i)->aiSecond();
	// }
	
// }


// void Civ::garbageCollect()
// {
	// /* Loop through all citizens of civ, and remove them from memory if they're dead. */
	// //const int vCitizenSize = vCitizen.size();
	// for(int i=0;i<vCitizen.size();++i)
	// {
		// if(vCitizen(i)->isAlive==false)
		// {
			// /* Purge. */
			// //std::cout<<"Purged.\n";
			// vCitizen.removeSlot(i);
			// --i;
		// }
	// }
	
	// /* Garbage collect for all cities. */
	// for(int i=0;i<vCity.size();++i)
	// {
		// vCity(i)->garbageCollect();
	// }
	
// }

// int Civ::getPopulation() { return vCitizen.size(); }







/* TABLE INTERFACE */

std::string Civ::getColumn(std::string _column)
{
	if ( _column=="name" )
	{
		return name;
	}
	if ( _column=="food" )
	{
		//return DataTools::toString(nFood);
	}
	if ( _column=="population" )
	{
		//return DataTools::toString(vCharacter.size());
	}
	if ( _column=="coordinates" )
	{
		//return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	}
	if ( _column=="territory" )
	{
		//return DataTools::toString(vTerritory.size());
	}
	// else if ( _column=="id" )
	// {
		// return DataTools::toString(id);
	// }
	
	return "?";
}
std::string Civ::getColumnType(std::string _column)
{
	if ( _column == "population" || _column == "territory" || _column == "food" )
	{
		return "number";
	}
	return "string";
}



#endif
