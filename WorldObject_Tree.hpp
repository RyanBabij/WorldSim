#pragma once
#ifndef WORLDSIM_WORLDOBJECT_TREE_HPP
#define WORLDSIM_WORLDOBJECT_TREE_HPP

/* WorldObject.hpp
   #include"WorldObject_Tree.hpp"

  Trees can be harvested for wood. They are also an obstacle for movement and LOS.
  
  Tree can provide resource on destruction.
  Tree can also be container for Items/Ingredients.
*/

//Static
// Doesn't need to track coordinates because it can't move on its own.
// 1 static per tile
// Statics only tick once per day
// Example statics:
   // Tree 
   // Plant
   // Construction (wall and/or floor)
   // Furniture
// Statics may block movement and/or LOS

// Tree should be a type of static.
class WorldObject_Tree: public WorldObject
{
   public:

    // Determines the stage of growth the tree is at: Sapling, young tree, tree.
  int growth;
   
   WorldObject_Tree(int _growth = 0);
   virtual ~WorldObject_Tree() {}
  
  
   virtual std::string getName();

   virtual Texture* currentTexture();
};

#endif
