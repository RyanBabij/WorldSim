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

		bool abstractMonthJob(Character* character, Job* job);
		
		void checkStockpileForBestItem(Character* character, Job* job);
		
		bool abstractMonthConstruction(Character* character);
		bool abstractMonthProduction(Character* character);
		void abstractMonthResearch(Character* character);
		void abstractMonthSocial(Character* character);
		void abstractMonthSplit(); // Settlement split
		void abstractMonthBiology(); // Character biology
		
		void moveAllCharacterHome(); // Move all characters back to their dwelling or a hall.
		
		Character* getFarmer(Vector <Character*>* vExclude);
		Character* getMiner(Vector <Character*>* vExclude);
		
		bool hasLocation(enumLocation _location); // True if there's an area that can be mined.
		void buildLocation(enumLocation _location);
		
		bool miningNeeded(); // True if any more mining resources are required.
		bool woodNeeded(); // True if any more mining resources are required.
		bool farmingEquipmentNeeded();
		bool coinsNeeded();
		

		
		Item* createItem(ItemType type);
		Item* produceItem(ItemType type);
		
		void payCharacter(Character* character, int amount);
		void payCharacterFromTreasury(Character* character, int amount);

		// WorldObjectGlobal virtual.
		Texture* currentTexture();
};

#endif
