#pragma once
#ifndef WORLDSIM_WORLDOBJECT_TREE_HPP
#define WORLDSIM_WORLDOBJECT_TREE_HPP

/* WorldObject.hpp
	#include"WorldObject_Tree.hpp"

  Trees can be harvested for wood. They are also an obstacle for movement and LOS.
*/

class WorldObject_Tree: public WorldObject
{
	public:
	// int x, y;
	
	// bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	// double weight; /* in grams */
	// double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
  
    // Determines the stage of growth the tree is at: Sapling, young tree, tree.
  int growth;
	
	WorldObject_Tree(int /* _growth = 0 */);
	virtual ~WorldObject_Tree() {}
  
  
	virtual std::string getName();
	// virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	// int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		// int distanceFrom(WorldObject*); /* Alias. */

	virtual Texture* currentTexture();
};

#endif
