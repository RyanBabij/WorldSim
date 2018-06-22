#pragma once
#ifndef WORLDSIM_TRIBE_HPP
#define WORLDSIM_TRIBE_HPP

/* Tribe.hpp
	#include "Tribe.hpp"

	Project: WorldSim
	Created: 14/10/2017. 0272065192.
	Updated: 14/10/2017. 0272065192.

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization.

	Notes:

	0272065192 - Created.

*/

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

class Tribe: public WorldObjectGlobal, public TableInterface
{
	private:
		Vector <std::string> familyNamePool;
		RandomNonStatic random;

	public:
	
		/* Identifying  colour of the tribe so you can see who it is quickly. */
		unsigned char colourRed;
		unsigned char colourGreen;
		unsigned char colourBlue;
		
		void setColour (const unsigned char, const unsigned char, const unsigned char);

		/* ABSTRACT TEST STUFF */
		int nFood;
		
		int actionPoints;
		
		/* Counters to regulate logic */
		int dailyCounter;
		int monthlyCounter;
		
	
		Vector <Character*> vCharacter;
		std::string name;
		World* world;
		
		Vector <HasXY*> vTerritory;
		
		bool foundSettlement;
		
		Tribe();
		~Tribe();
		
			/* LINK THE CIV TO A WORLD. ALSO LOAD UP CITIZEN FAMILIES. */
		void init(World*);

		//void setColour (const int,const int,const int);
		
			/* Returns false if the spawn failed. Integer is the number of attempts to make. In the future it should guarantee to find a suitable tile. */
		bool spawn();
		
		// WALK IN A RANDOM DIRECTION.
		/* New behavior:
			If there's a food surplus, explore nearby.
			If there's a food shortage, travel to a fertile tile or raid a tribe.
		*/
		void wander();
		
			// All available characters will go out hunting/gathering for food.
		void hunt();
			// All characters will try to eat, otherwise people will starve.
		void eat();
			// The tribe will develop its crafting and technology.
		void develop();
		
			// The tribe will lose influence over tiles it hasn't visited for a while.
		void degradeInfluence();
		
			// Ensure territory vector is up to date.
		void updateTerritory();
		
		
			/* SIMULATE X TURNS OF THE CIV. */
		void incrementTicks ( int /* nTicks */ );
		
		Texture* currentTexture();
		
		void generateCouples(int /* amount */ = 1 );
		
			//getPopulation returns living population of tribe.
		int getPopulation();
		
		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

#endif
