#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_HPP
#define GUILD_SETTLEMENT_DWARVEN_HPP

#include "Settlement.hpp"

class Settlement_Dwarven: public Settlement
{
	private:


	public:
	
		Vector <Character*> vCharacter;

		// //std::string name;

		// //World* world;
		
		// int colourRed;
		// int colourGreen;
		// int colourBlue;
		
    // //enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};
    // int race;
    
		Settlement_Dwarven();
    
    /* SIMULATE X TURNS OF THE SETTLEMENT. */
    void incrementTicks ( int /* nTicks */ );

		

    // WorldObjectGlobal virtual.
  Texture* currentTexture();
};

#endif
