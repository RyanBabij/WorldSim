#pragma once
#ifndef TILEVIEWER_HPP
#define TILEVIEWER_HPP

/* TileViewer

Each layer has its own tileset. Each tileset uses a single texture atlas.

Documentation here. */

//#include <Device/Display/OpenGLTools.hpp>
#include <Render/Renderer.hpp>
#include <Container/ArrayS3/ArrayS3.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Container/Vector/Vector.hpp>


class TileViewer
{
	private:
	int tilesPerAtlas;
	
	
	//ArrayS3 <int> textureAtlasPixels;

	public:
	
	
	ArrayS3 <int> aBoard; /* z is layers. Each layer gets it's own texture atlas. Value is texture ID. */
	
	
	int tileSizePixels; /* 64 is the default tile size, but can be modified to another pow2 size. */
	int atlasSizePixels; /* Note that graphics cards have a maximum texture size, so the atlas needs to be small enough for these cards. */
	
	
	
	
	TileViewer()
	{
	
		tileSizePixels=0;
		atlasSizePixels=0;
		tilesPerAtlas=0;
	}
	void init(const int _worldSizeXY, const int _nAtlas, const int _tileSizePixels, const int _atlasSizePixels)
	{
		aBoard.init(_worldSizeXY,_worldSizeXY,_nAtlas,0);
		tileSizePixels=_tileSizePixels;
		atlasSizePixels=_atlasSizePixels;
	}
	
	
	inline int & operator() (const unsigned int x, const unsigned int y, const unsigned int z)
	{
		return aBoard(x,y,z);
	}
	
	// void setDimensions(const int _sizeXY, const int _nLayers)
	// {
		// aWorld.init(_sizeXY,_sizeXY,_nLayers,0);
	// }
	
	
	/* Render the board with the center tiles in the center of the screen, at the given screen coordinates. */
	void render(const int _screenX1, const int _screenY1, const int _screenX2, const int _screenY2, const double _centerTileX, const double _centerTileY)
	{
		//OpenGLTools::placeTexture4(_screenX1,_screenY1,_screenX2,_screenY2,TEX_MENU_MAIN_MENU);
	
	}
	
	/* Loads the texture into the atlas and return the ID. */
	short int pushTexture(Texture* t, short int layer)
	{
	
		
	
		return 0; /* 0 is default value and is a blank texture. */
	}
	
	
	/* Load a pre-made atlas. */
	void loadAtlas(Texture* t, short int layer)
	{
	}
	
	void putTile(const int _x, const int _y, const int _z, const int _tileID)
	{
	}
	
	
};

#endif
