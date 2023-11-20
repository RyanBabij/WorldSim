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
		void abstractMonthSplit(); // Settlement split
		void abstractMonthBiology(); // Character biology
		
		Character* getFarmer(Vector <Character*>* vExclude);
		Character* getMiner(Vector <Character*>* vExclude);
		
		bool miningNeeded(); // True if any more mining resources are required.
		
		bool farmingEquipmentNeeded();
		
		bool produceItem(ItemType type);
		
		void payCharacter(Character* character, int amount);

		// WorldObjectGlobal virtual.
		Texture* currentTexture();
};

#endif
