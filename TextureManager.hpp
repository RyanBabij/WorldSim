#pragma once
#ifndef WORLDSIM_TEXTURE_MANAGER_HPP
#define WORLDSIM_TEXTURE_MANAGER_HPP

/* WorldSim: TextureManager.hpp
	#include "TextureManager.hpp"
	
	Class to manage custom Texture objects. Note that these should not be linked during render because constant lookups
	will be slow. Rather they should be linked during startup.
*/

#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Texture/TextureLoader.hpp>

#include <File/WTFManager.hpp> // loading raws

class TextureManager
{
	public:
	
	Vector <Texture*> vTexture;
	Vector <std::string> vTexName;
	Texture* tex404; // use this if we can't find the texture we need.
	
	void add (Texture& texture)
	{
	}
	
		// Get texture for linking with object
	Texture * get (std::string name)
	{
		return 0;
	}
	
	void loadTextureVerbose(const std::string _path, Texture* _texture)
	{
		std::cout<<"Loading: "<<_path<<".\n";

		if(loadTextureNearestNeighbour(_path,_texture)==false)
		{ std::cout<<"Error loading "<<_path<<".\n"; }
	}
	
	void loadRaw(std::string strRaw)
	{
		std::cout<<"Texture manager: Getting raw:\n"<<strRaw<<"\n";
		WTFManager wtfManager;
		std::cout<<"Parsing...\n";
		wtfManager.parse(strRaw);
		std::cout<<"End parsing\n";
		
		// get all texture paths and texture names
		Vector <WTFNode*>* vNode = wtfManager.getAllSub("TEXTURE");
		
		if (vNode == 0)
		{
			std::cout<<"Error null ptr\n";
		}
		for (int i=0;i<vNode->size();++i)
		{
			// load texture
			// store texture name
			std::string texName = (*vNode)(i)->getValue("NAME");
			std::string texPath = (*vNode)(i)->getValue("PATH");
			
			Texture * tex = new Texture();
			loadTextureVerbose(texPath,tex);
			vTexName.push(texName);
			vTexture.push(tex);
			
			
			std::cout<<"Loaded tex: "<<texName<<", "<<texPath<<"\n";
		}
	}
	
	Texture * getTextureByName(std::string _texName)
	{
		for (int i=0;i<vTexName.size();++i)
		{
			if (vTexName(i) == _texName)
			{
				if (vTexture.isSafe(i))
				{
					return vTexture(i);
				}
			}
		}
		return 0;
	}
};

#endif
