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
}

std::string WorldObject_Tree::getName()
{
	return "Tree";
}


Texture* WorldObject_Tree::currentTexture()
{
	return &TEX_WORLD_TERRAIN_FOREST_TREE;
}

#endif
