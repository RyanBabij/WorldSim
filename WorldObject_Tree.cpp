#pragma once
#ifndef GUILD_WORLDOBJECT_TREE_CPP
#define GUILD_WORLDOBJECT_TREE_CPP

#include"WorldObject_Tree.hpp"

/* WorldObject.cpp
	#include"WorldObject.cpp"
	<DOCUMENTATION>
*/

WorldObject_Tree::WorldObject_Tree()
{
	x=0; y=0;
	weight=0;
	bulk=0;
  
  growth=0;
  
  blocksView = true;
  blocksMovement = true;
}

std::string WorldObject_Tree::getName()
{
	return "Tree";
}


Texture* WorldObject_Tree::currentTexture()
{
  if ( growth==0 )
  {
    return &TEX_WORLD_TERRAIN_FOREST_SAPLING;
  }
	return &TEX_WORLD_TERRAIN_FOREST_TREE;
}

#endif
