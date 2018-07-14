#pragma once
#ifndef GUILD_SETTLEMENT_HPP
#define GUILD_SETTLEMENT_HPP


class Settlement: public WorldObjectGlobal
{
	private:


	public:
	
		Vector <Character*> vCharacter;

		//std::string name;

		//World* world;
		
		int colourRed;
		int colourGreen;
		int colourBlue;
		
    //enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};
    int race;
    
		Settlement();
		
			/* SIMULATE X TURNS OF THE SETTLEMENT. */
		virtual void incrementTicks ( int /* nTicks */ );

    // WorldObjectGlobal virtual.
  Texture* currentTexture();
};

#endif
