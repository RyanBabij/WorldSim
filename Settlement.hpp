#pragma once
#ifndef WORLDSIM_SETTLEMENT_HPP
#define WORLDSIM_SETTLEMENT_HPP


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

		//std::string name;

		//World* world;

		int colourRed;
		int colourGreen;
		int colourBlue;

		/* Counters to regulate logic ticks */
		int dailyCounter;
		int monthlyCounter;


		//enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};
		int race;

		int nFoodStockpile;
		int nMetalStockpile;

		Settlement();

		/* SIMULATE X TURNS OF THE SETTLEMENT. */
		virtual void incrementTicks ( int /* nTicks */ );

		bool removeCharacter( Character* /* _character */ );

		// WorldObjectGlobal virtual.
		Texture* currentTexture();

		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

#endif
