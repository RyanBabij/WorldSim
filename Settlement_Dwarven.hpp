#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_HPP
#define GUILD_SETTLEMENT_DWARVEN_HPP

#include "Settlement.hpp"

class Settlement_Dwarven: public Settlement
{
	private:


	public:
		Settlement_Dwarven();

		/* SIMULATE X TURNS OF THE SETTLEMENT. */
		void incrementTicks ( int /* nTicks */ );
		
		void abstractMonthFood(Character* character);
		void abstractMonthMine(Character* character);
		void abstractMonthProduction(Character* character);
		void abstractMonthResearch(Character* character);
		void abstractMonthSocial(Character* character);
		
		Character* getFarmer(Vector <Character*>* vExclude);
		Character* getMiner(Vector <Character*>* vExclude);
		Character* getCharacter(Vector <Character*>* vExclude);

		// WorldObjectGlobal virtual.
		Texture* currentTexture();
};

#endif
