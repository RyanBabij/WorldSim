#pragma once
#ifndef GUILD_WORLDOBJECT_FLORA_CPP
#define GUILD_WORLDOBJECT_FLORA_CPP

#include"WorldObject_Flora.hpp"

/* WorldObject_Flora.cpp
	#include"WorldObject_Flora.cpp"

  Implementation of WorldObject_Flora.hpp.
*/

WorldObject_Flora::WorldObject_Flora()
{
	x=0; y=0;
	weight=0;
	bulk=0;
  
  growth=0;
  
  blocksView = false;
  blocksMovement = false;
}

std::string WorldObject_Flora::getName()
{
	return "Flora";
}


Texture* WorldObject_Flora::currentTexture()
{
  if ( growth==0 )
  {
    return &TEX_FLORA_PLANT;
  }
	return &TEX_FLORA_PLANT;
}

////////////////////////////////////////////////////////////////////////////////
// Blackweed
////////////////////////////////////////////////////////////////////////////////

Flora_Blackweed::Flora_Blackweed(): WorldObject_Flora()
{
}

std::string Flora_Blackweed::getName()
{
	return "Blackweed";
}

Texture* Flora_Blackweed::currentTexture()
{
	return &TEX_FLORA_BLACKWEED;
}

////////////////////////////////////////////////////////////////////////////////
// Redweed
////////////////////////////////////////////////////////////////////////////////

Flora_Redweed::Flora_Redweed(): WorldObject_Flora()
{
}

std::string Flora_Redweed::getName()
{
	return "Redweed";
}


Texture* Flora_Redweed::currentTexture()
{
	return &TEX_FLORA_REDWEED;
}

////////////////////////////////////////////////////////////////////////////////
// Blueweed
////////////////////////////////////////////////////////////////////////////////

Flora_Blueweed::Flora_Blueweed(): WorldObject_Flora()
{
}

std::string Flora_Blueweed::getName()
{
	return "Redweed";
}


Texture* Flora_Blueweed::currentTexture()
{
	return &TEX_FLORA_BLUEWEED;
}

#endif
