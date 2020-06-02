#pragma once
#ifndef WORLDSIM_WORLDOBJECT_TREE_CPP
#define WORLDSIM_WORLDOBJECT_TREE_CPP

#include"WorldObject_Tree.hpp"

/* WorldObject_Tree.cpp
   #include"WorldObject_Tree.cpp"

  Implementation of WorldObject_Tree.hpp.
*/
WorldObject_Tree::WorldObject_Tree(int _growth /* =0 */)
{
   x=0; y=0;
   weight=0;
   bulk=0;
  
  growth=_growth;
  
  if (_growth == 0)
  {
    blocksView = false;
    blocksMovement = false;
  }
  else
  {
    blocksView = true;
    blocksMovement = true;
  }

  
  chopAmount = 100;
}

std::string WorldObject_Tree::getName()
{
  if ( chopAmount == 0 )
  {
    return "Tree stump";
  }
   return "Tree";
}


Texture* WorldObject_Tree::currentTexture()
{
  if (chopAmount==0)
  { return &TEX_OBJECT_STUMP;
  }
  
  if ( growth==0 )
  {
    return &TEX_WORLD_TERRAIN_FOREST_SAPLING;
  }
   return &TEX_WORLD_TERRAIN_FOREST_TREE;
}

#endif
