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

		
		
		void checkStockpileForBestItem(Character* character, Job* job);
		
		void putMarketRequest(Character* c, enumLocation type); // Put market request for location to be built.
		void putMarketRequest(Character* c, enumResource type); // Put market request for resource to be obtained.
		void putMarketRequest(Character* c, ItemType type); // Put market request for item to be built.
		void putMarketRequest
		(Character* c, ItemAction category, int minLevel); // Put market request for a type of item.
		
		bool abstractDayJob(Character* character, Job* job);
		bool abstractDayConstruction(Character* character);
		bool abstractDayProduction(Character* character);
		void abstractDayResearch(Character* character);
		void abstractDaySocial(Character* character);
		void abstractDaySplit(); // Settlement split
		void abstractDayBiology(); // Character biology
		
		void moveAllCharacterHome(); // Move all characters back to their dwelling or a hall.
		
		Character* getFarmer(Vector <Character*>* vExclude);
		Character* getMiner(Vector <Character*>* vExclude);
		
		bool hasLocation(enumLocation _location); // True if there's an area that can be mined.
		void buildLocation(enumLocation _location);
		
		bool miningNeeded(); // True if any more mining resources are required.
		bool woodNeeded(); // True if any more mining resources are required.
		bool coinsNeeded();
		

		
		Item* createItem(ItemType type, Character* creator, Location* location);
		Item* produceItem(ItemType type, Character* creator, Location* location, CanRequestItem* recipient);
		Item* produceItem(ItemAction type, Character* creator, Location* location, CanRequestItem* recipient, int minimumLevel);
		
		void payCharacter(Character* character, int amount);
		void payCharacterFromTreasury(Character* character, int amount);

		// WorldObjectGlobal virtual.
		Texture* currentTexture();
};

#endif
