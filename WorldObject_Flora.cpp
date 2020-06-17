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
	name="";
	
	x=0; y=0;
	weight=0;
	bulk=0;
  
  growth=0;
  
  blocksView = false;
  blocksMovement = false;
}

std::string WorldObject_Flora::getName()
{
	return name;
}


Texture* WorldObject_Flora::currentTexture()
{
  if ( growth==0 )
  {
    return &TEX_FLORA_PLANT;
  }
	return &TEX_FLORA_PLANT;
}

void WorldObject_Flora::generate()
{
	name="";
	if (RNG_TEST.flip()) // use colour
	{
		name+=wlistFloraColour.getRandom()+" ";
	}
	if (RNG_TEST.flip()) // use pattern
	{
		name+=wlistFloraPattern.getRandom()+" ";
	}
	if (RNG_TEST.flip()) // use adjective
	{
		name+=wlistFloraAdjective.getRandom()+" ";
	}
	
	if (RNG_TEST.flip() || name.size() == 0) // use noun
	{
		name+=wlistFloraNoun.getRandom()+" ";
	}
	
	name+=wlistFloraType.getRandom();
	
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
