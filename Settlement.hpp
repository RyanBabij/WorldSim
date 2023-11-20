#pragma once
#ifndef WORLDSIM_SETTLEMENT_HPP
#define WORLDSIM_SETTLEMENT_HPP

//#include "Location.hpp"
#include "Government.hpp"

#include "Resource.cpp"
#include "WorldObjectGlobal.hpp"
#include "Civ.hpp"

#include "ItemManager.hpp"
#include "ItemRequest.cpp"

class Settlement: public WorldObjectGlobal, public TableInterface
{
	private:

		
	protected:
		RandomNonStatic random;

	public:

	
		Civ* parentCiv;
		Vector <Character*> vCharacter;
		Vector <Character*> vDeceased;
		
		Government government;

		ItemManager stockpile;
		ItemRequestManager requestManager;
		
		Vector <Item*> vItem;
		
		
		// Ideas can become technologies and also books and other things.
		Vector <Idea> vIdea;
		
		ResourceManager resourceManager;
		Technology technology;


		//std::string name; // WorldObjectGlobal

		//World* world;

		int colourRed;
		int colourGreen;
		int colourBlue;

		/* Counters to regulate logic ticks */
		int dailyCounter;
		int monthlyCounter;


		//enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};
		int race;

		//int nFoodStockpile;
		//int nMetalStockpile;

		Settlement();
		
		/* GETTER AND SETTER */
		Character* getRandomCharacter();
		Character* getCharacter(Vector <Character*>* vExclude);
				
		/* SIMULATE X TURNS OF THE SETTLEMENT. */
		virtual void incrementTicks ( int /* nTicks */ );

		bool removeCharacter( Character* /* _character */ );
		
		bool hasIdea(Idea);
		bool isSpecialIdea(Idea idea); // If the idea leads to a breakthrough
		void giveIdea(Idea);
		
		virtual std::string getName() override;

		// WorldObjectGlobal virtual.
		Texture* currentTexture() override;

		/* TABLE INTERFACE */
		std::string getColumn(std::string _column) override;
		std::string getColumnType(std::string _column) override;
};

#endif
