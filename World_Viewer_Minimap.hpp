#pragma once
#ifndef WORLDSIM_WORLD_VIEWER_MINIMAP_HPP
#define WORLDSIM_WORLD_VIEWER_MINIMAP_HPP

/* WorldSim: Menu_WorldSimulator.hpp
#include "Menu_WorldSimulator.hpp" */

/** @brief World minimap menu
Displays a minimap of the entire world. Allows player to see where they are in the world, and also click on it to
quickly view other locations. Has a box which shows the current view area.
*/

#include "World_Viewer.hpp"

#include <Graphics/PixelScreen/PixelScreen.hpp>
#include <Graphics/GUI/GUI_Interface.hpp>


class WorldViewer_Minimap: public GUI_Interface
{
private:

	PixelScreen minimap;

public:
	World* world;

	WorldViewer_Minimap(): minimap(1,1)
	{
		world = 0;
	}
	
	void init()
	{
		if (world==0)
		{ return; }
		
		// load up the minimap pixels here
		minimap.init(world->nX,world->nY);
		minimap.fill(0,0,255,255);
		
		
		
		for (int y=0;y<world->nY;++y)
		{
			for (int x=0;x<world->nX;++x)
			{
				Texture * tex = world->aWorldTile(x,y).currentTexture();
				if ( tex )
				{
					minimap.setPixel(x,y,tex->averageRed,tex->averageGreen,tex->averageBlue);
				}
			}
		}
				
		eventResize();
	}
	
	void render()
	{
		if (world==0)
		{ return; }
	
		// Render the viewbox as a grey overlay on the minimap
		// Like many things this ought to be moved out of the render loop.
		// X axis
		double leftMostTile = worldViewer.centerTileX - (worldViewer.getTilesNX()/2);
		if (leftMostTile<0) {leftMostTile=0;}
		if (leftMostTile>world->nX) {leftMostTile=world->nX;}
		
		// +0.5 adjustment seems to improve the precision of the edge, not sure why.
		double rightMostTile = worldViewer.centerTileX + (worldViewer.getTilesNX()/2)+0.5;
		if (rightMostTile<0) {rightMostTile=0;}
		if (rightMostTile>world->nX) {rightMostTile=world->nX;}
		
		double rightMostTilePercent = rightMostTile / world->nX;
		double leftMostTilePercent = leftMostTile / world->nX;
		
		int minimapViewboxX2 = panelX1+(panelNX*rightMostTilePercent);
		int minimapViewboxX1 = panelX1+(panelNX*leftMostTilePercent);

		// Y axis
		double upMostTile = worldViewer.centerTileY + (worldViewer.getTilesNY()/2)+0.5;
		if (upMostTile<0) {upMostTile=0;}
		if (upMostTile>world->nY) {upMostTile=world->nY;}
		
		double downMostTile = worldViewer.centerTileY - (worldViewer.getTilesNY()/2);
		if (downMostTile<0) {downMostTile=0;}
		if (downMostTile>world->nY) {downMostTile=world->nY;}
		
		double upMostTilePercent = upMostTile / world->nY;
		double downMostTilePercent = downMostTile / world->nY;
		
		int minimapViewboxY2 = panelY1+(panelNY*upMostTilePercent);
		int minimapViewboxY1 = panelY1+(panelNY*downMostTilePercent);

		minimap.render();
		
		// render viewbox over minimap
		Renderer::placeColour4a(255,255,255,60,minimapViewboxX1,minimapViewboxY1,minimapViewboxX2,minimapViewboxY2);
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
		if (world==0)
		{ return false; }
		
		if (_mouse->x < panelX1 || _mouse->x>panelX2 || _mouse->y < panelY1 || _mouse->y>panelY2)
		{
			return false;
		}
		
		// center the worldViewer on the tile we clicked in the minimap.
		if (_mouse->isLeftClick)
		{
			float xMousePos = _mouse->x - panelX1;
			float xPercent = xMousePos / panelNX;
			int tileX = xPercent * world->nX;
			
			float yMousePos = _mouse->y - panelY1;
			float yPercent = yMousePos / panelNY;
			int tileY = yPercent * world->nY;
			
			
			worldViewer.setCenterTile(tileX,tileY);
			//_mouse->isLeftClick=false;
			return true;
		}
		

		
		return false;
	}

	void eventResize()
	{
		//std::cout<<"MINIMAP RESIZED: "<<panelX1<<", "<<panelY1<<", "<<panelX2<<", "<<panelY2<<".\n";
		int panelSizeX = panelX2-panelX1;
		int panelSizeY = panelY2-panelY1;
		
		minimap.setPanel(panelX1,panelY1,panelX2,panelY2);
	}

};
WorldViewer_Minimap minimap;

#endif
