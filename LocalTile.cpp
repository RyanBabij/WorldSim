#pragma once
#ifndef WORLDSIM_LOCALTILE_CPP
#define WORLDSIM_LOCALTILE_CPP

#include "LocalTile.hpp"

#include <WorldGenerator/Biome.hpp>

/*
	Local tile data gets stored here. Render stuff can't go here because that depends on neighboring tiles.
*/

LocalTile::LocalTile()
{

}

LocalTile::~LocalTile()
{
}

Texture* LocalTile::currentTexture()
{
  return &TEX_WORLD_TEST_00;
  
}


#endif
