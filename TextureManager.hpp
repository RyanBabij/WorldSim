#pragma once
#ifndef WORLDSIM_TEXTURE_MANAGER_HPP
#define WORLDSIM_TEXTURE_MANAGER_HPP

/* WorldSim: TextureManager.hpp
	#include "TextureManager.hpp"
	
	Class to manage custom Texture objects. Note that these should not be linked during render because constant lookups
	will be slow. Rather they should be linked during startup.
*/

#include <Graphics/Texture/Texture.hpp>

class TextureManager
{
	public:
	
	//Vector <Texture*>
	Texture* tex404;
	
	void add (Texture& texture)
	{
	}
	
		// Get texture for linking with object
	Texture * get (std::string name)
	{
		return 0;
	}
	
	void loadRaw(std::string strRaw)
	{
	}
};

#endif
