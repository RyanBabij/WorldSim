#pragma once
#ifndef WORLDSIM_WORLDOBJECT_HPP
#define WORLDSIM_WORLDOBJECT_HPP

/* WorldObject.hpp
	#include"WorldObject.hpp"

  A World Object is an object which can have some physical presence on a map. Therefore
  items, weapons, creatures and NPCs are all World Objects. This interface provides the
  basic functions for providing information on the object's location and how to render it.
  
  WorldObject deals with local map objects. Objects on the global map are handled with
  WorldObjectGlobal.
*/

class WorldObject: public HasTexture
{
	public:
	int x, y;
  
  //Implementing full global coordinates to make life easier for now.
  // This datatype should be able to hold coordinates for any reasonably-sized world.
  long unsigned int fullX, fullY;
  
  
    /* TEMPORARY WORKAROUND TO ALLOW CHARACTERS TO MOVE ACROSS MAPS.
      CURRENTLY ONLY USED FOR CHARACTERS
      In future World_Local should distinguish between actors and objects */
  int worldX, worldY;
	
	bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	double weight; /* in grams */
	double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
  
  /* True if line of sight cannot pass this object.
    Might be expanded in future. For example maybe a
    crate will block view of people who are sneaking. */
  bool blocksView;
  
  /* True if characters and creatures cannot walk into this tile. */
  bool blocksMovement;
	
	WorldObject();
	virtual ~WorldObject() {}
  
  
	virtual std::string getName();
	virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		int distanceFrom(WorldObject*); /* Alias. */
    
  int distanceTo(int /* _x */, int /* _y */); /* Same, using raw coordinates */
    

	virtual Texture* currentTexture();
};

#endif
