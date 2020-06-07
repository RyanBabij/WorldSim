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
		//Renderer::placeColour4a(250,0,0,100,panelX1,panelY1,panelX2/2,panelY2/2);
		minimap.render();
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
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
