#pragma once

#ifndef GUILD_WORLD_VIEWER_HPP
#define GUILD_WORLD_VIEWER_HPP

#include <Device/Display/DisplayInterface.hpp>
#include <Device/Mouse/MouseInterface.hpp>
#include <Render/Renderer.hpp>

/* #include "World_Viewer.hpp"
	Code for rendering the player's view of the world. There's no class here, just functions and vars.
	
	I'm thinking that this could be a seperate class with a pointer to the world. However, this will probably reduce the efficiency.
	
*/


/* #include "World_Viewer.hpp"

	Needs an updated description.
	
*/

/* Made it a GUI class, since it now functions in GUIs. */
class WorldViewer: public DisplayInterface, public MouseInterface
{
	public:
	
	bool active;
	
	int tileSize; /* How big to render the tiles, in pixels. */
	float centerTileX, centerTileY; /* The tile at the center of the screen. */
	
		/* Store the last position of the mouse, so we can figure out which tile the mouse is on. */
	short int lastMouseX, lastMouseY;
	
	World* world;
	World_Local* worldLocal;
	
	/* Rendering coordinates. */
	int mainViewX1, mainViewX2, mainViewY1, mainViewY2;
	int mainViewNX, mainViewNY;
	
	bool panning;
	float panX, panY; /* Stores the last mouse position when panning. Needs to be a double. */

	bool alternateTile;
	
		/* The current tile that the mouse is hovering over. Set to -1 if the mouse is not hovering over a tile. */
	int hoveredXTile, hoveredYTile;
	
	int tilesToSkip;
	
	bool territoryView;
	
		// Temp: Local tile to render.
	int localX, localY;
	
	WorldViewer()
	{
		tileSize=8;
		centerTileX=0; centerTileY=0;
		world=0;
		mainViewX1=0; mainViewX2=0;
		mainViewY1=0; mainViewY2=0;
		mainViewNX=0; mainViewNY=0;
		active=false;
		
		panX=0; panY=0;
		
		panning=false;
		alternateTile=0;
		
		lastMouseX=0;
		lastMouseY=0;
		
		hoveredXTile=0;
		hoveredYTile=0;
		
		tilesToSkip=0;
		
		territoryView = false;
		
		localX=-1;
		localY=-1;
	}
	
		// Center the view on the middle of the world. Good for initializing.
	void centerView()
	{
		if (world==0)
		{
			return;
		}
		centerTileX = world->nX/2;
		centerTileY = world->nY/2;
	}
	
bool keyboardEvent( Keyboard* _keyboard )
{
	//std::cout<<"Keypress: "<<(int)_keyboard->lastKey<<".\n";

	/* Pass keyboard event, instead of handling individual keypressed here. */
	
	// if ( menuUnitConstructor.active==true )
	// {
		// if (keyboard->keyWasPressed)
		// { menuUnitConstructor.keyboardEvent(keyboard); }
		// return;
	// }
	// else if (unitComponentConstructorMenu.active==true)
	// {
		// if (keyboard->keyWasPressed)
		// { unitComponentConstructorMenu.keyboardEvent(keyboard); }
		// return;
	// }
	// else if (vehicleConstructorMenu.active==true)
	// {
		// if (keyboard->keyWasPressed)
		// { vehicleConstructorMenu.keyboardEvent(keyboard); }
		// return;
	// }

	/* Keyboard handling for main view. */
	
	//if(keyboard->keyWasPressed&&playerControl)
	// if(keyboard->keyWasPressed)
	// {
		// if(keyboard->isPressed(Keyboard::RIGHT))
		// {
		// }
		// else if(keyboard->isPressed(Keyboard::LEFT))
		// {
		// }
		// else if(keyboard->isPressed(Keyboard::UP))
		// {
		// }
		// else if(keyboard->isPressed(Keyboard::DOWN))
		// {
		// }
		/* Zoom the main map in by one step. */
		if(_keyboard->isPressed(Keyboard::EQUALS))
		{
			if (_keyboard->keyWasPressed)
			{
				zoomIn();
			}
		}
		/* Zoom the main map out by one step. */
		if(_keyboard->isPressed(Keyboard::MINUS))
		{
			if (_keyboard->keyWasPressed)
			{
				zoomOut();
			}
		}
		// if(keyboard->isPressed(Keyboard::SPACEBAR))
		// {
		// }
		// if (keyboard->isPressed(Keyboard::Q)||keyboard->isPressed(Keyboard::q))
		// {
			// std::cout<<"I'm afraid I can't let you do that.\n";
		// }
		// if(keyboard->isPressed(Keyboard::ESCAPE))
		// {
			// queryMode=false;
			// buildSubMenu=false;
			// buildSubMenuChoice=0;
		// }
		if(_keyboard->isPressed(Keyboard::D)||_keyboard->isPressed(Keyboard::d)) /* Debug info. */
		{
			std::cout<<"Debug Info:\n";
			
			if (world==0)
			{
				std::cout<<"ERROR: World doesn't exist.\n";
			}
			else
			{
			
				std::cout<<"Num tribes: "<<world->vTribe.size()<<"\n";
				
				for (int i=0;i<world->vTribe.size();++i)
				{
					Tribe* t = world->vTribe(i);
					std::cout<<"All living members of tribe "<<t->name<<".\n";
					
					for(int j=0;j<t->vCharacter.size();++j)
					{
						Character* c = t->vCharacter(j);
						if(c->isAlive)
						{
						std::cout<<c->id<<": "<<c->getFullName()<<", age: "<<c->age<<". ";
						if(c->isMale==true)
						{ std::cout<<"Male.\n"; }
						else
						{ std::cout<<"Female.\n";
						}
						}
					}
					
				}
				std::cout<<"World population: "<<world->getPopulation()<<".\n";
			}
			
			std::cout.flush();
			
			_keyboard->keyUp(Keyboard::D);
			_keyboard->keyUp(Keyboard::d);
		}
		
		
		if(_keyboard->isPressed(Keyboard::F)||_keyboard->isPressed(Keyboard::f)) /* Flush console. */
		{

			std::cout.flush();
			
			_keyboard->keyUp(Keyboard::F);
			_keyboard->keyUp(Keyboard::f);
		}

	return false;
}

void zoomIn()
{
	if ( tilesToSkip > 0 ) { --tilesToSkip; }
	else
	{
	//tilesToSkip=0;
	tileSize*=2;
	if(tileSize>400000)
	{ tileSize=400000; }
	}
	std::cout<<"Tilesize: "<<tileSize<<".\n";
	// if (tileSize < 32 && world->isSafe(hoveredXTile, hoveredYTile))
	// {
		// centerTileX=hoveredXTile;
		// centerTileY=hoveredYTile;
		// glutWarpPointer(mainViewX1+(mainViewNX/2),mainViewY1+(mainViewNY/2));
	// }
}
void zoomOut()
{
	tileSize/=2;
	if(tileSize<1)
	{ tileSize=1; ++tilesToSkip; }


}

void switchTarget(World_Local* _worldLocal)
{
	worldLocal=_worldLocal;
}

	
		/* Keep the map visible on the screen so the player can't accidentally move it off screen and lose it. */
	void normaliseCoordinates()
	{
		if ( centerTileX < 0 )
		{
			centerTileX=0;
		}
		if ( centerTileY < 0 )
		{
			centerTileY=0;
		}
		if ( centerTileX > world->nX )
		{
			centerTileX = world->nX;
		}
		if ( centerTileY > world->nY )
		{
			centerTileY = world->nY;
		}
	}
	
	bool mouseEvent ( Mouse* mouse )
	{
		if ( world==0 ) { return false; }
	
		lastMouseX=mouse->x;
		lastMouseY=mouse->y;
		
		if(panning==true)
		{
			/* The view can be panned by holding down the middle mouse button
				and moving the mouse to drag the map. */
			centerTileX+=(panX-mouse->x)/((float)tileSize/(tilesToSkip+1));
			centerTileY+=(panY-mouse->y)/((float)tileSize/(tilesToSkip+1));
			panX=mouse->x;
			panY=mouse->y;
		}
		
		if(mouse->isMiddleClick==false)
		{
			panning=false;
		}
		if ( mouse->isMiddleClick)
		{
			//std::cout<<"Middle click.\n";
			panning=true;
			//panning=!panning;
			
			/* Set initial pan coordinates. */
			panX=mouse->x;
			panY=mouse->y;
			
			//mouse->isMiddleClick=false;
		}
		
		if(mouse->isWheelDown)
		{
			zoomOut();

			mouse->isWheelDown=false;
		}
		if(mouse->isWheelUp)
		{
			zoomIn();
			
			mouse->isWheelUp=false;
		}
		
		normaliseCoordinates();
		
		if ( mouse->isLeftClick == true )
		{
			//if ( isSafe(hover
			//std::cout<<"Clicked on: "<<hoveredXTile<<", "<<hoveredYTile<<".\n";
			
			world->queryTile(hoveredXTile,hoveredYTile);
		}
		
		if (mouse->isRightClick)
		{
			std::cout<<"Adding tile: "<<hoveredXTile<<", "<<hoveredYTile<<" to render.\n";
			world->generateLocal(hoveredXTile,hoveredYTile);
			localX=hoveredXTile;
			localY=hoveredYTile;
		}
		
		return false;
	}
	
	void setPanel( const int _x1, const int _y1, const int _x2, const int _y2)
	{
		//std::cout<<"Worldviewer has been resized.\n";
		//std::cout<<"Coords: "<<mainViewX1<<","<<mainViewY1<<","<<mainViewX2<<","<<mainViewY2<<".\n";
	//	std::cout<<"MVSP\n";
		mainViewX1 = _x1;
		mainViewY1 = _y1;
		mainViewX2 = _x2;
		mainViewY2 = _y2;
		
		mainViewNX = mainViewX2-mainViewX1;
		mainViewNY = mainViewY2-mainViewY1;
		
	}
	
	inline void setCenterTile (const float _centerTileX, const float _centerTileY)
	{
		centerTileX = _centerTileX;
		centerTileY = _centerTileY;
	}
	
		/* Convert tile coordinates to screen (pixel) coordinates. */
	void toScreenCoords(const int _tileX, const int _tileY, int * _pixelX, int * _pixelY)
	{
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float centerTileXPixels = centerTileXDecimal*tileSize;
		float centerTileYPixels = centerTileYDecimal*tileSize;
	

		//double pixelOffsetX = tileSize*centerTileXDecimal;
		//double pixelOffsetY = tileSize*centerTileYDecimal;

		const int iCenterTileX = centerTileX;
		

		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTileX = mainViewX1+(mainViewNX/2) - centerTileXPixels;
		while(pixelTileX>=mainViewX1)
		{
			--tileX;
			tileX-=tilesToSkip;
			pixelTileX-=tileSize;
			
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		int pixelTileY = mainViewY1 + (mainViewNY/2) - centerTileYPixels;
		while(pixelTileY>=0)
		{
			--tileY;
			tileY-=tilesToSkip;
			pixelTileY-=tileSize;
		}
		
		
		int nExtraXTiles = _tileX - tileX;
		int nExtraYTiles = _tileY - tileY;
		
		if ( nExtraXTiles>=0 && nExtraYTiles>=0 )
		{
			*_pixelX = pixelTileX + (tileSize * nExtraXTiles);
			*_pixelY = pixelTileY + (tileSize * nExtraYTiles);
		}
		else
		{
			*_pixelX = -1000;
			*_pixelY = -1000;
		}
	}
	
	
		/* HERE I RENDER THE GRAPHICAL ICONS WHICH ARE PRESENT ON THE WORLD MAP, FOR EXAMPLE: CITIES AND ARMIES. */
		/* ONLY A SMALL NUMBER OF WORLD TILES WILL HAVE AN ICON ON THEM, SO I WILL USE A DIFFERENT TECHNIQUE. */
	void renderWorldIcons()
	{
		//return;
		//std::cout<<"Rendering world icons.\n";
	
		if (active == false)
		{ return; }
		if(world==0)
		{
			std::cout<<"ABORT: No world to render.\n";
			return;
		}
		
			/* 0223554692 - SO I GUESS THIS PART MODIFIES THE SCREEN SO I CAN SIMPLY DRAW FROM (0,0). */
		Renderer::saveViewPort();
		Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
		
			/* CONFUSING SHIT WHICH IS HARD TO EXPLAIN. */
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float pixelOffsetX = tileSize*centerTileXDecimal;
		float pixelOffsetY = tileSize*centerTileYDecimal;

		const int iCenterTileX = centerTileX;
		
		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTile = mainViewNX/2;
		while(pixelTile>=0)
		{
			--tileX;
			pixelTile-=tileSize;
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		pixelTile = mainViewNY/2;
		while(pixelTile>=0)
		{
			--tileY;
			pixelTile-=tileSize;
		}
		
			/* ALLOWS DRAWING OF TEXTURES. */
		Renderer::setTextureMode();
		
			// USING THE NEW 'WORLDOBJECTGLOBAL' INTERFACE.
		for (int i=0;i<world->vWorldObjectGlobal.size();++i)
		{
			WorldObjectGlobal* wog = world->vWorldObjectGlobal(i);
			int wogPixelX=-1;
			int wogPixelY=-1;

			toScreenCoords(wog->worldX,wog->worldY,&wogPixelX,&wogPixelY);
			
				// I NEED TO DO A CHECK TO SEE IF THE OBJECT IS VISIBLE WITHIN THE CURRENT VIEW.
			
			if ( wogPixelX != -1000 && wogPixelY != -1000 )
			{
				Renderer::placeTexture4(wogPixelX,wogPixelY,wogPixelX+tileSize,wogPixelY+tileSize,wog->currentTexture(),false);
			}
		}
			/* RESTORE THE VIEWPORT TO THE PREVIOUS SETTING (PROBABLY THE WHOLE PANEL). */
		Renderer::restoreViewPort();
		
	}
	
	void render()
	{
		
		if (active == false)
		{ return; }
		if(world==0)
		{
			std::cout<<"ABORT: No world to render.\n";
			return;
		}

			/* 0223554692
				SO I GUESS THIS PART MODIFIES THE SCREEN SO I CAN SIMPLY DRAW FROM (0,0).
				I THINK IT ALSO PREVENTS ACCIDENTALLY RENDERING OVER OTHER PARTS OF THE SCREEN.
			*/
		Renderer::saveViewPort();

		Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
		
		float centerTileXDecimal = centerTileX - (int)centerTileX;
		float centerTileYDecimal = centerTileY - (int)centerTileY;
		
		float centerTileXPixels = centerTileXDecimal*tileSize;
		float centerTileYPixels = centerTileYDecimal*tileSize;
	

		//double pixelOffsetX = tileSize*centerTileXDecimal;
		//double pixelOffsetY = tileSize*centerTileYDecimal;

		const int iCenterTileX = centerTileX;
		

		const int tilesToDrawX = (mainViewNX/tileSize) + 1;
		const int tilesToDrawY = (mainViewNY/tileSize) + 1;
		
		int tileX=centerTileX;
		int pixelTileX = mainViewX1+(mainViewNX/2) - centerTileXPixels;
		while(pixelTileX>=mainViewX1)
		{
			--tileX;
			tileX-=tilesToSkip;
			pixelTileX-=tileSize;
			
		}
		const int revertTileX = tileX;
		
		int tileY=centerTileY;
		int pixelTileY = mainViewY1 + (mainViewNY/2) - centerTileYPixels;
		while(pixelTileY>=0)
		{
			--tileY;
			tileY-=tilesToSkip;
			pixelTileY-=tileSize;
		}

			/* UPDATE HOVERED TILE COORDS. */
			/* MAKE SURE MOUSE IS WITHIN THE RENDERING VIEW */
		if (lastMouseX >= mainViewX1 && lastMouseX <= mainViewX2 && lastMouseY >= mainViewY1 && lastMouseY <= mainViewY2 )
		{
			const int startingPX = pixelTileX;
			const int startingPY = pixelTileY;
			
			int diffX = (lastMouseX - startingPX)/tileSize;
			int diffY = (lastMouseY - startingPY)/tileSize;
			
				/* UPDATE THE COORDS OF THE HOVERED TILE. */
			hoveredXTile = tileX + diffX;
			hoveredYTile = tileY + diffY;
		}
		else
		{
			hoveredXTile=-1;
			hoveredYTile=-1;
		}
		
		if ( tileSize <= 16 )
		{

			/* Pixel coords for leftmost tile. */
			Renderer::setColourMode();

			for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
			{
				//break;
				glBegin(GL_QUAD_STRIP);

				
				/* NOTE: 022-171 added this initial case. The left side of the map wasn't drawing, so I put this here, and moved the other glColor3ub from above the vertexes to below them. This seems to have fixed the rendering. The tiles were always there, they just weren't rendering... */
				glColor3ub(0,0,0);
				unsigned char lastRed=0;
				unsigned char lastGreen=0;
				unsigned char lastBlue=0;
				
				//for (int currentX = -pixelOffsetX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				{
				
					if(world->isSafe(tileX,tileY)==true)
					{
						
						glVertex2s(currentX,currentY);
						glVertex2s(currentX,currentY+tileSize);
						
							// Territory view
						if (territoryView)
						{
							if ( world->aInfluence(tileX,tileY) != 0 )
							{
								Tribe* dominantTribe = world->getDominantInfluence(tileX,tileY);
								if (dominantTribe!=0)
								{
									glColor3ub(dominantTribe->colourRed,dominantTribe->colourGreen,dominantTribe->colourBlue);
								}
								else
								{
									glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
								}
							}
							else
							{
								glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							}
						}
						else
						{
							if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==false)
							{
								//enderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_OCEAN_00, false);
								glColor3ub(TEX_WORLD_TERRAIN_OCEAN_00.averageRed,TEX_WORLD_TERRAIN_OCEAN_00.averageGreen,TEX_WORLD_TERRAIN_OCEAN_00.averageBlue);
							}
							//else if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true)
							//{
							//	glColor3ub(TEX_WORLD_TERRAIN_GRASS_00.averageRed,TEX_WORLD_TERRAIN_GRASS_00.averageGreen,TEX_WORLD_TERRAIN_GRASS_00.averageBlue);
							//}
							
							
							else
							{
								glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							}
							
						}


							// ONLY DRAW STUFF THE THE PLAYER HAS REVEALED.
							// FOG OF WAR GOES HERE.

						
							// Basic landmass view
						if ( world->aIsLand(tileX,tileY) )
						{
							//glColor3ub(50,200,50);
						}
						else
						{
							//glColor3ub(50,50,200);
						}
							//landmass ID view
						// if ( world->aIsLand(tileX,tileY) && world->aLandmassID(tileX,tileY) != -1 )
						// {
							// glColor3ub(50,200,50);
						// }
						// else if ( world->aIsLand(tileX,tileY) )
						// {
							// glColor3ub(200,50,50);
						// }
						// else
						// {
							// glColor3ub(50,50,200);
						// }
						

					}
					
					/* NOTE: 022-171. Final case here, otherwise last x doesn't render. */
					else if(world->isSafe(tileX-1,tileY)==true)
					{
						glVertex2s(currentX,currentY);
						glVertex2s(currentX,currentY+tileSize);
					}

					++tileX;
					tileX+=tilesToSkip;
				}
				glEnd();

				tileX=revertTileX;
				++tileY;
				tileY+=tilesToSkip;
			}
			
		}
		else
		{
			
			Renderer::setTextureMode();

			for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
			{
				//glBegin(GL_QUAD_STRIP);

				
				/* NOTE: 022-171 added this initial case. The left side of the map wasn't drawing, so I put this here, and moved the other glColor3ub from above the vertexes to below them. This seems to have fixed the rendering. The tiles were always there, they just weren't rendering... */
				glColor3ub(255,255,255);
				//unsigned char lastRed=0;
				//unsigned char lastGreen=0;
				//unsigned char lastBlue=0;
				
				for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
				{
						// RENDER THE LOCAL TILE
					if (localX == tileX && localY == tileY && world->isSafe(tileX,tileY))
					{
						float pixelsPerLocalTile = (float)tileSize/LOCAL_MAP_SIZE;
						//std::cout<<"pixelsPerLocalTile: "<<pixelsPerLocalTile<<".\n";
						

						
						float currentSubY = currentY;
						float nextSubY = currentY + pixelsPerLocalTile;
						
						int subTileSize = pixelsPerLocalTile;
						if (subTileSize < 1) { subTileSize = 1; }
						
						Random r1;
						r1.seed (42);
						
						for (int localYTile = 0; localYTile<LOCAL_MAP_SIZE;++localYTile)
						{
							
							if ( nextSubY>=mainViewY1 && currentSubY <= mainViewY2 && floor(currentSubY) != floor(nextSubY) )
							{
							
								float currentPixel = currentX;
								float nextPixel = currentX+pixelsPerLocalTile;
								for (int localXTile = 0; localXTile<LOCAL_MAP_SIZE;++localXTile)
								{
									if ( nextPixel>=mainViewX1 && currentPixel <= mainViewX2 && floor(currentPixel) != floor(nextPixel) )
									{
										//std::cout<<".";
										Renderer::placeTexture4(floor(currentPixel), floor(currentSubY), floor(nextPixel), floor(nextSubY), &TEX_WORLD_TERRAIN_GRASS_00, false);
										
										if ( r1.oneIn(10) )
										{
											Renderer::placeTexture4(floor(currentPixel), floor(currentSubY), floor(nextPixel), floor(nextSubY), &TEX_ALCHEMY, false);
										}
									}
									else
									{
										r1.oneIn(10);
									}
									
									currentPixel=nextPixel;
									nextPixel+=pixelsPerLocalTile;
								}
							}
							else
							{
								for (int localXTile = 0; localXTile<LOCAL_MAP_SIZE;++localXTile)
								{
								r1.oneIn(10);
								}
							}
							currentSubY=nextSubY;
							nextSubY+=pixelsPerLocalTile;
						}
						//std::cout<<"\n";
						//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
					}
					else
					{
						/* Textures are chosen here rather than from tile objects because it is highly dependent on neighboring tiles. It would be possible to delegate texture handling to tile objects, but would take too much memory maintaining pointers to neighbours. In future maybe worldtile can return hardcoded textures chosen by world object. */
						
						// if (world->isSafe(tileX,tileY) )
						// {
							// //Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, world->aWorldTile(tileX,tileY).currentTexture(), false);
						// }
				
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY))
						{
							// //Renderer::pair
							//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
							
							
							if (world->aSeed(tileX,tileY) % 4 == 0)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 1)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_01, false);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 2)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_02, false);
							}
							else if (world->aSeed(tileX,tileY) % 4 == 3)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_03, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_00, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_02, false);
							}
							if ( world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_03, false);
							}
							if ( world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_01, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_00, false);
							}
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_01, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_03, false);
							}
							if ( world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false)
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_CORNER_02, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY+1) && world->isLand(tileX,tileY+1) ==false
							&& world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_PENINSULA_00, false);
							}
							
							if ( world->isSafe(tileX+1,tileY) && world->isLand(tileX+1,tileY) ==false
							&& world->isSafe(tileX,tileY-1) && world->isLand(tileX,tileY-1) ==false
							&& world->isSafe(tileX-1,tileY) && world->isLand(tileX-1,tileY) ==false )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_COAST_PENINSULA_02, false);
							}


							
							if ( world->aSeed(tileX,tileY) % 20 == 0 )
							{
								Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_ALCHEMY, false);
							}
							// glVertex2s(currentX,currentY);
							// glVertex2s(currentX,currentY+tileSize);
							
								// //ONLY DRAW STUFF THE THE PLAYER HAS REVEALED.
								// //FOG OF WAR GOES HERE.
							// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							
							// if ( world->aInfluence(tileX,tileY) != 0 )
							// {
								// Tribe* dominantTribe = world->getDominantInfluence(tileX,tileY);
								// if (dominantTribe!=0)
								// {
									// glColor3ub(dominantTribe->colourRed,dominantTribe->colourGreen,dominantTribe->colourBlue);
								// }
								// else
								// {
									// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
								// }
							// }
							// else
							// {
								// glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
							// }
						}
						else if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==false)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_OCEAN_00, false);
						}
						
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == FOREST)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_FOREST_TREE, false);
						}
						
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == MOUNTAIN)
						{
							//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_MOUNTAIN_00, false);
						}

						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == DESERT)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_DESERT_00, false);
						}
						if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true && world->aTerrain(tileX,tileY) == DESERT)
						{
							Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_DESERT_01, false);
						}
						
						
						
					
					
					}
					//TEX_WORLD_TERRAIN_FOREST_TREE
					
					/* NOTE: 022-171. Final case here, otherwise last x doesn't render. */
					// else if(world->aHeightMap.isSafe(tileX-1,tileY)==true)
					// {
						// glVertex2s(currentX,currentY);
						// glVertex2s(currentX,currentY+tileSize);
					// }

					++tileX;
					tileX+=tilesToSkip;
				}
				//glEnd();

				tileX=revertTileX;
				++tileY;
				tileY+=tilesToSkip;
			}
			
		// Texture mode
		//Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_01, false);
		
	}

		Renderer::restoreViewPort();
		

		
		
		/* Now render the icons on the world map. */
		renderWorldIcons();
			//Renderer::placeTexture4(mainViewX1+(mainViewNX/2)-32, mainViewY1+(mainViewNY/2)-32, mainViewX1+(mainViewNX/2)+32, mainViewY1+(mainViewNY/2)+32, &TEX_WORLD_TEST_00, false);
		normaliseCoordinates();
		
	}

	
};

#endif
