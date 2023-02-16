#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_CPP
#define GUILD_SETTLEMENT_DWARVEN_CPP

#include "Settlement_Dwarven.hpp"

Settlement_Dwarven::Settlement_Dwarven()
{

	name = "Dwarven Settlement";
	world = 0;
	colourRed = 0;
	colourGreen = 0;
	colourBlue = 0;

	race = DWARVEN;

	nMetalStockpile=0;
	monthlyCounter=0;
	dailyCounter=0;

}

Texture* Settlement_Dwarven::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

/* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement_Dwarven::incrementTicks ( int nTicks )
{
	std::cout<<"Incrementing Dwarven settlement.\n";
	
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
		return;

	}

	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;


	for (int i=0; i<vCharacter.size(); ++i)
	{
		Character* c = vCharacter(i);

		std::cout<<"Incrementing: "<<c->getFullName()<<".\n";
		c->incrementTicks(nTicks);



	while (monthlyCounter >= 2592000)
	{
		if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
		{
			nMetalStockpile+=30;
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

		monthlyCounter-=2592000;
	}

	while ( dailyCounter >= 86400 )
	{
		if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
		{
			++nMetalStockpile;
		}
		dailyCounter-=86400;
	}
	
	
	}

	return;
}


#endif
  