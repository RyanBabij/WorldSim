#pragma once
#ifndef WORLDSIM_WORLDOBJECTGLOBAL_TRIBALARTIFACT_HPP
#define WORLDSIM_WORLDOBJECTGLOBAL_TRIBALARTIFACT_HPP

/* WorldSim: WorldObjectGlobal_TribalArtifact.hpp
	#include "WorldObjectGlobal_TribalArtifact.hpp"

  Tribal Artifacts are artifacts created by Tribes. Some of them are a curiosity, others may grant powers.

*/

#include <string>
#include <Container/Vector/Vector.hpp>

class World;
class Tribe;
class Character;

//#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"
#include <Container/Table/TableInterface.hpp>

class TribalArtifact: public WorldObjectGlobal, public TableInterface
{
  private:
  
	public:
  
		//std::string name; (WorldObjectGlobal)
		World* world;
    Tribe* ownerTribe;


		
		TribalArtifact();
		~TribalArtifact();

    
    /* WORLD OBJECT GLOBAL */
		Texture* currentTexture();
		
		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

class TribalArtifact_Jewellery: public TribalArtifact
{
  private:
  
	public:
    Character* ownerCharacter;


		
		TribalArtifact_Jewellery();
		~TribalArtifact_Jewellery();

    
    /* WORLD OBJECT GLOBAL */
		Texture* currentTexture();
		
		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

class TribalArtifact_BattleSite: public TribalArtifact
{
  private:
  
	public:
    //Character* ownerCharacter;


		
		TribalArtifact_BattleSite();
		//~TribalArtifact_BattleSite();

    
    /* WORLD OBJECT GLOBAL */
		Texture* currentTexture();
		
		/* TABLE INTERFACE */
		std::string getColumn(std::string _column);
		std::string getColumnType(std::string _column);
};

#endif
