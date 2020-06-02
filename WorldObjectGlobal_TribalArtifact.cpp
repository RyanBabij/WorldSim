#pragma once
#ifndef WORLDSIM_WORLDOBJECTGLOBAL_TRIBALARTIFACT_CPP
#define WORLDSIM_WORLDOBJECTGLOBAL_TRIBALARTIFACT_CPP

/* WorldSim: WorldObjectGlobal_TribalArtifact.cpp
   #include "WorldObjectGlobal_TribalArtifact.cpp"

  Tribal Artifacts are artifacts created by Tribes. Some of them are a curiosity, others may grant powers.

*/

#include <string>
#include <Container/Vector/Vector.hpp>

class Tribe;
class World;

//#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

#include "WorldObjectGlobal_TribalArtifact.hpp"

TribalArtifact::TribalArtifact()
{
  name = "Ancient Jewellery";
  world = 0;
  ownerTribe = 0;
}

TribalArtifact::~TribalArtifact()
{
}

Texture* TribalArtifact::currentTexture()
{
  return &TEX_WORLD_ARTIFACT_GEMS;
}

std::string TribalArtifact::getColumn(std::string _column)
{
  return "?";
}

std::string TribalArtifact::getColumnType(std::string _column)
{
  return "?";
}


TribalArtifact_Jewellery::TribalArtifact_Jewellery()
{
  name = "Ancient Jewellery";
  world = 0;
  ownerTribe = 0;
}

TribalArtifact_Jewellery::~TribalArtifact_Jewellery()
{
}

Texture* TribalArtifact_Jewellery::currentTexture()
{
  return &TEX_WORLD_ARTIFACT_GEMS;
}

std::string TribalArtifact_Jewellery::getColumn(std::string _column)
{
  return "?";
}

std::string TribalArtifact_Jewellery::getColumnType(std::string _column)
{
  return "?";
}

TribalArtifact_BattleSite::TribalArtifact_BattleSite()
{
  name = "Ancient Battleground";
  world = 0;
  ownerTribe = 0;
}

Texture* TribalArtifact_BattleSite::currentTexture()
{
  return &TEX_WORLD_ARTIFACT_BATTLE;
}

std::string TribalArtifact_BattleSite::getColumn(std::string _column)
{
  return "?";
}

std::string TribalArtifact_BattleSite::getColumnType(std::string _column)
{
  return "?";
}


#endif
