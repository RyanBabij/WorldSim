#pragma once
#ifndef WORLDSIM_TRIBE_HPP
#define WORLDSIM_TRIBE_HPP

/* WorldSim: Tribe.hpp
	#include "Tribe.hpp"

	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization.

*/

#include "Character.hpp"

class World;

#include <System/Time/Calendar.hpp>
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

class Tribe: public WorldObjectGlobal, public TableInterface
{
	private:
		Vector <std::string> familyNamePool;
    
  protected:
		RandomNonStatic random;

	public:
	
		/* Identifying  colour of the tribe so you can see who it is quickly. */
		unsigned char colourRed;
		unsigned char colourGreen;
		unsigned char colourBlue;
    
      // Race can be Human, Dwarven or Elven.
    int race;
		
		void setColour (const unsigned char, const unsigned char, const unsigned char);

		/* ABSTRACT TEST STUFF */
		int nFood; /* 1 food can support 10 people. */
		
		int actionPoints;
		
		/* Counters to regulate logic */
		int dailyCounter;
		int monthlyCounter;
		
	
		Vector <Character*> vCharacter;
		//std::string name;
		//World* world;
    
      // Any Character that dies gets moved here */
    Vector <Character*> vDeceased;
		
		Vector <HasXY*> vTerritory;
		
		bool foundSettlement;
    bool isAlive;
		
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
		virtual void wander();
		
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
		virtual void incrementTicks ( int /* nTicks */ );
		

		
		void generateCouples(int /* amount */ = 1 );
		
			//getPopulation returns living population of tribe.
		int getPopulation();
    
      // REMOVE THE CHARACTER FROM THE VECTOR.
    bool removeCharacter( Character* );
      // ADD THE CHARACTER TO THE VECTOR.
    void addCharacter (Character * );
    
      // Everybody dies
    void kill ();
    
    //Called if tribe wants to attack another tribe.
    virtual void combat (Tribe* );
    
      // Return a Character capable of combat. If there is none, return 0.
    Character* getDefender ();
    
    // LOCATION
    
      //Return the local map the tribe is on.
    World_Local* getCurrentMap ();
    
    
    /* WORLD OBJECT GLOBAL */
		Texture* currentTexture();
		
		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

#endif
