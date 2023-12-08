#pragma once
#ifndef WORLDSIM_SETTLEMENT_HPP
#define WORLDSIM_SETTLEMENT_HPP

#include "Location.hpp"
#include "Government.hpp"

#include "Stockpile.hpp"
#include "WorldObjectGlobal.hpp"
#include "Civ.hpp"

#include "ItemRequest.cpp"
#include "LocationRequest.cpp"

class Settlement: public WorldObjectGlobal, public TableInterface
{
	private:

		
	protected:
		RandomNonStatic random;

	public:
	
		Civ* parentCiv;
		Vector <Character*> vCharacter;
		Vector <Character*> vDeceased;
		Vector <Item*> vItem;
		// Ideas can become technologies and also books and other things.
		Vector <Idea> vIdea;
		
		Government government;

		Stockpile stockpile;
		//IntermediateManager stockpileIntermediate;
		ItemRequestManager requestManager;
		LocationRequestManager locationRequestManager;
		//ResourceManager resourceManager;
		Technology technology;
		
		LocationManager location;
		
		int colourRed;
		int colourGreen;
		int colourBlue;

		/* Counters to regulate logic ticks */
		int dailyCounter;
		int monthlyCounter;

		int race;

		Settlement();
		void initSimulation(); // biomes, creatures etc have been initialised.
		
		/* GETTER AND SETTER */
		int getPopulation();
		Character* getRandomCharacter();
		Character* getCharacter(Vector <Character*>* vExclude);
		
		bool has ( enumLocation loc );
		bool canBuild ( enumLocation loc );
		
		int getMiningCapacity();
		int getFarmingCapacity();
		
		World_Biome* getBiome();
		
		void printAllMoneyInSettlement(); // Including Character money
		int getAllMoneyInSettlement(); // Including Character money
		double getMoneyPercentInTreasury();
		double getAverageCharacterWealth();
				
		/* SIMULATE X TURNS OF THE SETTLEMENT. */
		virtual void incrementTicks ( int /* nTicks */ );

		bool removeCharacter( Character* /* _character */ );
		
		bool hasIdea(Idea);
		bool isSpecialIdea(Idea idea); // If the idea leads to a breakthrough
		void giveIdea(Idea);
		
		void buildDwarvenFortress();
		
		virtual std::string getName() override;

		// WorldObjectGlobal virtual.
		Texture* currentTexture() override;

		/* TABLE INTERFACE */
		std::string getColumn(std::string _column) override;
		std::string getColumnType(std::string _column) override;
};

#endif
