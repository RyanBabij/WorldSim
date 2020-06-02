#pragma once
#ifndef WORLDSIM_WORLDOBJECT_SIGN_HPP
#define WORLDSIM_WORLDOBJECT_SIGN_HPP

/* WorldObject_Sign.hpp
	#include"WorldObject_Sign.hpp"
	
  
*/

class WorldObject_Sign: public WorldObject
{
	public:
  
  std::string signText;

  
	
	WorldObject_Sign();
	virtual ~WorldObject_Sign() {}
  
  
	virtual std::string getName();
	// virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	// int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		// int distanceFrom(WorldObject*); /* Alias. */

	virtual Texture* currentTexture();
};

#endif
