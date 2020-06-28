#pragma once
#ifndef WORLDSIM_WORLD_VIEWER_HPP
#define WORLDSIM_WORLD_VIEWER_HPP

/* WorldSim: WorldViewer
   #include "World_Viewer.hpp"

   Code for rendering the player's view of the world. It is able to render the global
   (world map) view, and additionally any local maps which have been loaded in.

   To do this it makes use of doubles. The main number tracks the global rendering
   coordinates, and the mantissa tracks the local tile rendering coordinates.

   The World Viewer also takes mouse and keyboard input to control panning and zooming.

   It provides coordinates (both local and absolute) for the tile the mouse
   is hovering over.

   It needs some cleanup and optimization. Various techniques could improve performance
   significantly. Also a lot of calculations should be moved out of the render function.
   
   This class could technically be moved into Wildcat but it's not likely
   to be used for much else so it's not a big deal for now.
   
   We should be able to significantly improve render efficiency by building an array or vector of textures to draw.
   Then we draw each texture, only binding them once. Texture binding seems to be expensive so I think this will save
   a lot of render time.
	
*/

#include <Device/Display/DisplayInterface.hpp>
#include <Device/Mouse/MouseInterface.hpp>
#include <Graphics/Render/Renderer.hpp>
#include <Game/WorldGenerator/Biome.hpp> // For rendering biomes. Needs to be fixed
#include "WorldObjectGlobal.hpp"
#include "Creature.hpp"
#include "Tribe.hpp"

// Raindrops that are rendered over screen to indicate rain.
// Should start at top of screen and travel down. Should not render on sheltered tiles
class RainDrop
{
   public:
   int x, y;

   RainDrop()
   {
      x=0;
      y=0;
   }
   void drop(int amount)
   {
      y-=amount;
   }
   void render()
   {
      Renderer::placeColour4a(60,60,255,255,x,y,x+3,y+4);
   }
};

class RainManager
{
   private:
   RandomNonStatic rand;
   
   Vector <RainDrop*> vRainDrop;

   World* world;

   int x1, x2;
   int y1, y2;
   int MAX_RAIN;
   int RAIN_PER_FRAME;

   public:

   RainManager() { }
  
   void init (int _x1, int _y1, int _x2, int _y2, World* _world, int _maxRain=10000, double _rainPercentOfX=0.005)
   {
      x1=_x1;
      y1=_y1;
      x2=_x2;
      y2=_y2;
      MAX_RAIN = _maxRain;
      RAIN_PER_FRAME = _rainPercentOfX*(x2-x1);

      world = _world;

      rand.seed();
   }

   void updateRain()
   {
      // This should be moved out of the render call and threaded.
      if ( world->isRaining &&  vRainDrop.size() < MAX_RAIN )
      {
         for (int i=0;i<RAIN_PER_FRAME;++i)
         {
            RainDrop* rain = new RainDrop;
            rain->y = y2-rand.randomInt(20);


            rain->x = rand.randomInt(x2-x1);
            rain->x+=x1;
            vRainDrop.push(rain);
         }
      }
      for (int i=0;i<vRainDrop.size();++i)
      {
         vRainDrop(i)->drop(rand.randomInt(3)+18);
         if (vRainDrop(i)->y < y1)
         {
            delete vRainDrop(i);
            vRainDrop.removeSlot(i);
         }
      }
   }
  void render();

};

class WorldViewer: public DisplayInterface, public MouseInterface
{
   private:

   /* Rendering coordinates.
	 These should be updated to use the standard GUI panel system */
   int mainViewX1, mainViewX2, mainViewY1, mainViewY2;
   int mainViewNX, mainViewNY;

   double localTileMantissa;

   // Here is what we should really be drawing, rather than deciding during the render call.
   // Ideally this should also be running in a thread, locking when render is called.
   ArrayS2 <Vector <Texture* > > aScene;

   public:
   //RAIN
   RainManager rainManager;

   bool active;

   int tileSize; /* How big to render the tiles, in pixels. */

   /* The tile at the center of the screen.
   Note that the mantissa is used to center on local tiles.
   This must be a double due to the required level of precision.
   */
   double centerTileX, centerTileY;

   /* The first tile rendered, and the pixel coordinates where it is rendered */
   /* We store this for other functions to use. */
   int firstTileX, firstTileY;
   unsigned long int firstTileAbsX, firstTileAbsY; /* (0,0) absolute of the above */
   int firstPixelX, firstPixelY;

   // Temp: Local tile to render.
   int centerLocalX, centerLocalY;
   double pixelsPerLocalTile;


   /* Store the last position of the mouse, so we can figure out which tile the mouse is on. */
   short int lastMouseX, lastMouseY;

   World* world;
   World_Local* worldLocal;

   bool panning;
   float panX, panY; /* Stores the last mouse position when panning. Needs to be a double. */

   bool alternateTile;

   /* The current tile that the mouse is hovering over. Set to -1 if the mouse is not hovering over a tile. */
   int hoveredXTile, hoveredYTile;
   int hoveredXTileLocal, hoveredYTileLocal;
   unsigned long int hoveredAbsoluteX, hoveredAbsoluteY;
   bool showHoveredTile;

   int tilesToSkip;

   bool territoryView;
   bool tilesetMode;
   bool subterraneanMode;
	bool landMode; // Show only landmasses

   // If true, game will randomly scroll around.
   // In the future, the title screen will probably be scrolling around a map.
   bool demoMode;
   double demoScroll;

   WorldViewer()
   {
      tileSize=8;
      tilesToSkip=0;
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);
      localTileMantissa = (double)1/(double)LOCAL_MAP_SIZE;
      
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
      hoveredXTileLocal=0;
      hoveredYTileLocal=0;
      hoveredAbsoluteX=ABSOLUTE_COORDINATE_NULL;
      hoveredAbsoluteY=ABSOLUTE_COORDINATE_NULL;

      showHoveredTile=false;

      centerLocalX=LOCAL_MAP_SIZE/2;
      centerLocalY=LOCAL_MAP_SIZE/2;

      firstTileX=0;
      firstTileY=0;
      firstPixelX=0;
      firstPixelY=0;
      firstTileAbsX=0;
      firstTileAbsY=0;

      territoryView = false;
      tilesetMode = true;
      landMode = false;
      subterraneanMode = false;

      demoMode = false;
      demoScroll = 0.1;

      rainManager.init(0,0,0,0,0);
   }
	
   // Center the view on the middle of the world. Good for initializing.
   void centerView()
   {
      if (world==0)
      { return; }
   
      centerTileX = world->nX/2;
      centerTileY = world->nY/2;

      centerTileX+=0.5;
      centerTileY+=0.5;
   }
	
   bool keyboardEvent( Keyboard* _keyboard )
   {
      //zoomIn();
      //return false;
      // Zoom the main map in by one step.  (Can use either top row or numpad)
      if(_keyboard->isPressed(Keyboard::EQUALS) || _keyboard->isPressed(Keyboard::PLUS))
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

      return false;
   }

   void zoomIn()
   {
      if ( tilesToSkip > 0 )
      { --tilesToSkip; }
      else
      {
         tileSize*=2;
         if(tileSize>2000000)
         { tileSize=2000000; }
      }
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);
   }

   void zoomOut()
   {
      tileSize/=2;

      if(tileSize<1)
      { tileSize=1; ++tilesToSkip; }
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);
   }
   
   void setTileSize(const int _tileSize)
   {
      tileSize=_tileSize;
      tilesToSkip=0;
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);
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
         panning=true;
         /* Set initial pan coordinates. */
         panX=mouse->x;
         panY=mouse->y;
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
         world->queryTile(hoveredXTile,hoveredYTile);
         world->queryTileLocal(hoveredXTileLocal,hoveredYTileLocal);
      }

      if (mouse->isRightClick)
      {
         world->generateLocal(hoveredXTile,hoveredYTile);
         DEBUG_X=hoveredXTile;
         DEBUG_Y=hoveredYTile;
      }
      

      return false;
   }

   void setPanel( const int _x1, const int _y1, const int _x2, const int _y2)
   {
		// Todo: Update class to use standard GUI panel system.
      mainViewX1 = _x1;
      mainViewY1 = _y1;
      mainViewX2 = _x2;
      mainViewY2 = _y2;

      mainViewNX = mainViewX2-mainViewX1;
      mainViewNY = mainViewY2-mainViewY1;
      
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);

      rainManager.init(_x1,_y1,_x2,_y2,world);
   }
	
   inline void setCenterTile (const double _centerTileX, const double _centerTileY, const int _subTileX=LOCAL_MAP_SIZE/2, const int _subTileY=LOCAL_MAP_SIZE/2)
   {
      centerTileX = _centerTileX;
      centerTileY = _centerTileY;

      centerTileX += (_subTileX * localTileMantissa)+localTileMantissa/2;
      centerTileY += (_subTileY * localTileMantissa)+localTileMantissa/2;
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
  
   // Take the given pixels and calculate what tile they are. This is in absolute coordinates.
   // I need to add a version which can do relative coords.
   void toTileCoords(int pixelX, int pixelY, unsigned long int * absX, unsigned long int * absY)
   {
      if (absX==0 || absY==0) {return;}
      
      if (pixelX<0 || pixelY<0 || pixelsPerLocalTile == 0)
      {
         *absX=ABSOLUTE_COORDINATE_NULL;
         *absY=ABSOLUTE_COORDINATE_NULL;
      }

      int pixelDistanceX = pixelX - firstPixelX;
      int pixelDistanceY = pixelY - firstPixelY;

      if ( pixelDistanceX < 0 || pixelDistanceY < 0 )
      {
         *absX=0;
         *absY=0;
         return;
      }

      *absX = firstTileAbsX + ((double)pixelDistanceX/pixelsPerLocalTile);
      *absY = firstTileAbsY + ((double)pixelDistanceY/pixelsPerLocalTile);
   }
	
   /* HERE I RENDER THE GRAPHICAL ICONS WHICH ARE PRESENT ON THE WORLD MAP, FOR EXAMPLE: CITIES AND ARMIES. */
   /* ONLY A SMALL NUMBER OF WORLD TILES WILL HAVE AN ICON ON THEM, SO I WILL USE A DIFFERENT TECHNIQUE. */
   void renderWorldIcons()
   {
      if (active == false)
      { return; }
      if(world==0)
      {
         std::cout<<"ABORT: No world to render.\n";
         return;
      }
		
      // set viewport to panel
      Renderer::saveViewPort();
      Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
      
      // some pixel coordinate calculations
      // needs to be cleaned up

      float centerTileXDecimal = centerTileX - (int)centerTileX;
      float centerTileYDecimal = centerTileY - (int)centerTileY;

      float pixelOffsetX = tileSize*centerTileXDecimal;
      float pixelOffsetY = tileSize*centerTileYDecimal;

      const int iCenterTileX = centerTileX;

      const int tilesToDrawX = (mainViewNX/tileSize) + 1;
      const int tilesToDrawY = (mainViewNY/tileSize) + 1;

      int tileX=centerTileX;
      int pixelTile = mainViewNX/2;
      
      // this is bad but I'm too tired to figure out how to fix it rn
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

      Renderer::setTextureMode();

      // Draw global world objects
      // Basically stuff like armies, fleets, tribes
      for (int i=0;i<world->vWorldObjectGlobal.size();++i)
      {
         WorldObjectGlobal* wog = world->vWorldObjectGlobal(i);

         // Check if we're supposed to render a local map on this tile.
         // If so, don't draw world icons over it.
         if ( tileSize > 4 && world->isSafe(tileX,tileY) )
         {
            for ( int i2=0;i2<world->vWorldLocal.size();++i2)
            {
               if ( world->vWorldLocal(i2)->globalX == wog->worldX && world->vWorldLocal(i2)->globalY == wog->worldY )
               {
                  wog = 0;
                  break;
               }
            }
         }
      
         if ( wog == 0 ) { continue; }

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

   // This function should run in the background and basically constantly build and update the array of textures to render.
   // We need code to build the world view, and code to build the local map view.
   // We can start with just the global view because it's less complicated.
   // The scene should not update more than once per frame. Otherwise we're going to be constantly loading and unloading
   // arrays for nothing.
   void updateScene()
   {
   }

	void render()
	{
      if (active == false || world==0)
      { return; }
      
      if ( demoMode )
      {
         if ( centerTileX < 10 && demoScroll < 0 )
         {
            demoScroll = demoScroll * -1;
         }
         else if ( centerTileX > world->nX - 10 && demoScroll > 0 )
         {
            demoScroll = demoScroll * -1;
         }
         centerTileX += demoScroll;
      }
      
      // I don't know why this has to be here but it do
      pixelsPerLocalTile = ((double)tileSize/LOCAL_MAP_SIZE);
      
      hoveredAbsoluteX = ABSOLUTE_COORDINATE_NULL;
      hoveredAbsoluteY = ABSOLUTE_COORDINATE_NULL;

      Renderer::saveViewPort();
      Renderer::resizeViewPort(mainViewX1,mainViewY1,mainViewX2,mainViewY2);
    
      /* The World Viewer is passed centre coordinates because typically the camera needs to be centered on something.
      However from this we must calculate what coordinates to start rendering from in the top-left.
      Therefore we take the centre coordinates and work back to the top-left. This is done for both the tile (array)
      coordinates, and the rendering (pixel) coordinates. */

      float centerTileXDecimal = centerTileX - (int)centerTileX;
      float centerTileYDecimal = centerTileY - (int)centerTileY;
      float centerTileXPixels = centerTileXDecimal*tileSize;
      float centerTileYPixels = centerTileYDecimal*tileSize;

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
      //We need to store the top-left coordinates for other functions to use.
      firstTileX=tileX;
      firstTileY=tileY;
      if (world->isSafe(tileX,tileY))
      {
         firstTileAbsX=tileX*LOCAL_MAP_SIZE;
         firstTileAbsY=tileY*LOCAL_MAP_SIZE;
      }
      else
      {
         firstTileAbsX=ABSOLUTE_COORDINATE_NULL;
         firstTileAbsY=ABSOLUTE_COORDINATE_NULL;
      }
      firstPixelX=pixelTileX;
      firstPixelY=pixelTileY;

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
      
      if ( tilesetMode == false )
      {
         renderColourMode(tileX,tileY,pixelTileX,pixelTileY);
      }
      else // Render in texture mode
      {
         renderTextureMode(tileX,tileY,pixelTileX,pixelTileY);
      }
      if(subterraneanMode==false)
      {
         rainManager.render();
      }

      Renderer::restoreViewPort();
      // Now render the icons on the world map.
      renderWorldIcons();
      normaliseCoordinates();
   }
   
   void renderLocalMap(World_Local* localMap, int currentX, int currentY)
   {
      float currentSubY = currentY;
      float nextSubY = currentY + pixelsPerLocalTile;

      int subTileSize = pixelsPerLocalTile;
      if (subTileSize < 1) { subTileSize = 1; }

      const enumBiome localBaseBiome = localMap->baseBiome;
      
      // this needs to be fixed, we should simply be asking
      // the local map and local tile what to draw.
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
                  if ( subterraneanMode && localMap->dataSubterranean)
                  { // SUBTERRANEAN MODE
               
                     // render order:
                     // terrain
                     // static
                     // objects
                     // fog
                     // overlay
                     
                     glColor4ub(255,255,255,255);
                     //Very basic player line of sight check here (only if we're in Adventure mode)
                     // Unseen tiles
                     if (FOG_OF_WAR && playerCharacter !=0 && activeMenu == MENU_ADVENTUREMODE && playerCharacter->hasSeen(localMap, localXTile,localYTile,true) == 0 )
                     {
                     }
                     //Previously seen tiles
                     else if (FOG_OF_WAR && playerCharacter !=0 && activeMenu == MENU_ADVENTUREMODE && playerCharacter->hasSeen(localMap, localXTile,localYTile,true) == 1 )
                     {
                        //Draw tile very dark to symbolise fog of war
                        LocalTile* localTile = &localMap->dataSubterranean->aSubterranean(localXTile,localYTile);

                        Vector <Texture*> * vText = localTile->currentTextures();
                        if ( vText != 0)
                        {
                           for (int i=0;i<vText->size();++i)
                           {
                              Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), (*vText)(i), false);
                           }
                        }
                        delete vText;
                     }
                     else /* DRAW VISIBLE TILES */
                     {
                        LocalTile* localTile = &localMap->dataSubterranean->aSubterranean(localXTile,localYTile);
                        Vector <Texture*> * vText = localTile->currentTextures();
                        if ( vText != 0)
                        {
                           for (int i=0;i<vText->size();++i)
                           {
                              Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), (*vText)(i), false);
                           }
                        }
                        for(int i=0;i<localMap->dataSubterranean->aSubterranean(localXTile,localYTile).vObject.size();++i)
                        {
                           Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), localMap->dataSubterranean->aSubterranean(localXTile,localYTile).vObject(i)->currentTexture(), false);
                        }
                        delete vText;
                     }
                  }
                  else
                  { // NON-SUBTERRANEAN MODE
                     
                     
                     //Very basic player line of sight check here (only if we're in Adventure mode)
                     // Unseen tiles
                     if (FOG_OF_WAR && playerCharacter !=0 && activeMenu == MENU_ADVENTUREMODE && playerCharacter->hasSeen(localMap, localXTile,localYTile) == 0 )
                     { // If we've never seen this tile
                  
                        // Draw nothing (defaults to black)

                     }
                     else if (FOG_OF_WAR && playerCharacter !=0 && activeMenu == MENU_ADVENTUREMODE && playerCharacter->hasSeen(localMap, localXTile,localYTile) == 1 )
                     { //Previously seen tiles
                        //Draw tile very dark to symbolise fog of war
                        LocalTile* localTile = &localMap->data->aLocalTile(localXTile,localYTile);

                        unsigned char lightValue = 80;
                        glColor3ub(lightValue,lightValue,lightValue);
                        //Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), localTile->currentTexture(), false);

                        Vector <Texture*> * vText = localTile->currentTextures();
                        if ( vText != 0)
                        {
                           for (int i=0;i<vText->size();++i)
                           {
                              Renderer::placeTexture4(currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), (*vText)(i), false);
                           }
                        }
                        delete vText;

                        glColor3ub(255,255,255);
                     }
                     else
                     { // Currently visible tiles
                        LocalTile* localTile = &localMap->data->aLocalTile(localXTile,localYTile);

                        // this belongs in World.
								// Due to increase in height levels we will remove the height shading and replace it with
								// height shading relative to player's height. This obviously will only work when a player
								// exists
                        unsigned char lightValue = localTile->height+127;
								//unsigned char lightValue = 255;

                        int currentSecond = world->calendar.second;
                        int sunsetCounter = currentSecond-50;

                        int currentMinute = world->calendar.minute;
                        int currentHour = world->calendar.hour;

								// This is a bad implementation. We should overlay a colour over the whole playfield instead.
								glColor3ub(lightValue,lightValue,lightValue);
                        // NIGHT
                        if (currentHour < 6 || currentHour > 19)
                        {
                           //glColor3ub(50+lightValue,50+lightValue,50+lightValue);
                           //glColor3ub(lightValue,lightValue,lightValue);
                        }
                        else if (currentHour == 6) // SUNRISE
                        {
                           //glColor3ub(110+lightValue,100+lightValue,120+lightValue);
                        }
                        else if (currentHour == 19) // SUNSET
                        {
                           //glColor3ub(130+lightValue,100+lightValue,100+lightValue);
                        }
                        else
                        {
                           //glColor3ub(180+lightValue,180+lightValue,180+lightValue);
                           //glColor3ub(80+(lightValue/2),80+(lightValue/2),80+(lightValue/2));
                           //if (currentSecond > 50 ) { glColor3ub(80+(sunsetCounter*9)+(lightValue/2),80+(sunsetCounter*9)+(lightValue/2),80+(sunsetCounter*10)+(lightValue/2)); }
                        }

                        // draw all textures for this tile
                        // in future this should be copied to an array for later smart rendering, preferably outside of render function.
                        Vector <Texture*> * vText = localTile->currentTextures();
                        if ( vText != 0)
                        {
                           for (int i=0;i<vText->size();++i)
                           {
                              Renderer::placeTexture4
                              (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), (*vText)(i), false);
                           }
                        }
                        delete vText;

                        glColor3ub(255,255,255);

                        // Draw wall if necessary.
                        // Move this to static
								// this is bad, needs to account for multiple blockers
                        if (localTile->bWall != 0 )
                        {
                           if ( (localTile->bWall & 0b10001000) == 0b10001000) // NORTH
                           {
                              Renderer::placeTexture4RotatedDegrees
                              (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SOUTH, 180);
                           }
                           if ( (localTile->bWall & 0b00010001) == 0b00010001) // WEST
                           {
                              Renderer::placeTexture4RotatedDegrees
                              (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SOUTH, 90);
                           }
                           if ( (localTile->bWall & 0b01000100) == 0b01000100) // EAST
                           {
                              Renderer::placeTexture4RotatedDegrees
                              (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SOUTH, 270);
                           }
                           if ( (localTile->bWall & 0b00100010) == 0b00100010) // SOUTH
                           {
                              Renderer::placeTexture4
                              (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SOUTH, false);
                           }
                           // else if ( localTile->bWall == 0b01100110) // SE
                           // {
                              // Renderer::placeTexture4
                              // (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SE, false);
                           // }
                           // else if ( localTile->bWall == 0b00110011) // SW
                           // {
                              // Renderer::placeTexture4RotatedDegrees
                              // (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SE, 90);
                           // }
                           // else if ( localTile->bWall == 0b10011001) // NW
                           // {
                              // Renderer::placeTexture4RotatedDegrees
                              // (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SE, 180);
                           // }
                           // else if ( localTile->bWall == 0b11001100) // NE
                           // {
                              // Renderer::placeTexture4RotatedDegrees
                              // (currentPixel, currentSubY, ceil(nextPixel), ceil(nextSubY), &TEX_WALL_GREYBRICK_SE, 270);
                           // }
                        }
                     }
                  }

                  // UPDATE LOCAL TILE HOVERED AND ABSOLUTE COORDS
                  // ALSO OPTIONALLY RENDER A SELECTION TEXTURE
                  if ( lastMouseX > currentPixel && lastMouseX < ceil(nextPixel) && lastMouseY > currentSubY && lastMouseY < ceil(nextSubY) )
                  {
                     hoveredXTileLocal = localXTile;
                     hoveredAbsoluteX = hoveredXTile * LOCAL_MAP_SIZE + localXTile;
                     hoveredYTileLocal = localYTile;
                     hoveredAbsoluteY = hoveredYTile * LOCAL_MAP_SIZE + localYTile;

                     if ( showHoveredTile )
                     {
                        //The lines get overwritten if you don't -1. Do not add -1 anywhere else because it causes artifacts.
                        Renderer::placeBorder4(255,0,0,currentPixel, currentSubY, ceil(nextPixel)-1, ceil(nextSubY)-1);
                        Renderer::setTextureMode();
                     }
                  }
               }
               currentPixel=nextPixel;
               nextPixel+=pixelsPerLocalTile;
            }
         }
         currentSubY=nextSubY;
         nextSubY+=pixelsPerLocalTile;
      }
   }
   
   // render the world using the texture system.
   void renderTextureMode(int tileX, int tileY, float pixelTileX, float pixelTileY)
   {
      Renderer::setTextureMode();
      
      const int revertTileX = tileX;

      for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
      {
         //initial case
         glColor3ub(255,255,255);
  
         for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
         {
            // Check if we're supposed to render a local map on this tile.
            World_Local * localMap = 0;
            if ( tileSize >= 12 && world->isSafe(tileX,tileY) )
            {
               for ( int i=0;i<world->vWorldLocal.size();++i)
               {
                  if ( world->vWorldLocal(i)->globalX == tileX && world->vWorldLocal(i)->globalY == tileY )
                  {
                     localMap = world->vWorldLocal(i);
                     break;
                  }
               }
            }

            // RENDER THE LOCAL TILE
            // Should be it's own function
            //if (tileSize > 4 && localX == tileX && localY == tileY && world->isSafe(tileX,tileY))
            if ( localMap != 0 && localMap->data != 0 && localMap->initialized && tileSize > LOCAL_MAP_SIZE*4)
            {
               renderLocalMap(localMap,currentX,currentY);
            }
				else if (landMode && world->isSafe(tileX,tileY) )
				{
					// only render grassland and ocean tiles.
					if(world->isLand(tileX,tileY)==false)
					{
						Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_OCEAN_00, false);
					}
					else if(world->isLand(tileX,tileY)==true)
					{
						Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_GRASS_00, false);
					}
				}
            else if ( world->isSafe(tileX,tileY) )
            { // Render tile on world view
               /* Textures are chosen here rather than from tile objects because it is highly dependent on neighboring tiles. It would be possible to delegate texture handling to tile objects, but would take too much memory maintaining pointers to neighbours. In future maybe worldtile can return hardcoded textures chosen by world object. */
               
               // Todo:
               // tile should just return vector of textures tbh

               //UPDATE: Textures should be assigned to World_Local at generation/modification, to save cycles on render time.
               World_Local * tile = &(world->aWorldTile(tileX,tileY));
               if (tile)
               {
                  Renderer::placeTexture4(currentX, currentY, currentX+tileSize, currentY+tileSize, tile->currentTexture(), false);
               }

               // DRAW IMPROVEMENTS (FOREST, RIVER)
               // Improvements can layer over base terrain, and each other. For example a tile may have a river and forest.
               // This should be handled by the tile
               if(world->isLand(tileX,tileY) && world->aWorldTile(tileX,tileY).baseBiome == FOREST)
               {
                  Renderer::placeTexture4
                  (currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_FOREST_TREES, false);
               }
               else if(world->isLand(tileX,tileY) &&world->aWorldTile(tileX,tileY).baseBiome == JUNGLE)
               {
                  Renderer::placeTexture4
                  (currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_JUNGLE, false);
               }
               if(world->isLand(tileX,tileY) && world->aWorldTile(tileX,tileY).baseBiome == MOUNTAIN)
               {
                  Renderer::placeTexture4
                  (currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_MOUNTAIN_00, false);
               }
               if(world->isLand(tileX,tileY) && world->aWorldTile(tileX,tileY).baseBiome == HILLY)
               {
                  Renderer::placeTexture4
                  (currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_HILL, false);
               }
               if(world->isLand(tileX,tileY) && world->aRiverID(tileX,tileY) != -1)
               {
                  Renderer::placeTexture4
                  (currentX, currentY, currentX+tileSize, currentY+tileSize, &TEX_WORLD_TERRAIN_RIVER_FULL, false);
               }
            }
            ++tileX;
            tileX+=tilesToSkip;
         }
         tileX=revertTileX;
         ++tileY;
         tileY+=tilesToSkip;
      }
   }
   
   // render the world using the old colour system. Generally not used.
   void renderColourMode(int tileX, int tileY, float pixelTileX, float pixelTileY)
   {
      // Pixel coords for leftmost tile.
      Renderer::setColourMode();
      
      const int revertTileX = tileX;

      for (int currentY = pixelTileY; currentY<mainViewNY; currentY+=tileSize)
      {
         glBegin(GL_QUAD_STRIP);

         // initial case
         glColor3ub(0,0,0);

         for (int currentX = pixelTileX; currentX<mainViewNX+tileSize; currentX+=tileSize)
         {
            if(world->isSafe(tileX,tileY)==true)
            {
               glVertex2s(currentX,currentY);
               glVertex2s(currentX,currentY+tileSize);

               // Territory view
               if (territoryView)
               {
                  const Tribe* dominantTribe = world->aWorldTile(tileX,tileY).getDominantInfluence();
                  if (dominantTribe!=0)
                  {
                     glColor3ub(dominantTribe->colourRed,dominantTribe->colourGreen,dominantTribe->colourBlue);
                  }
                  else
                  {
                     glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
                  }
               }
					else if (landMode)
					{
						// only draw land and ocean textures.
                  if(world->isLand(tileX,tileY)==false)
                  {
                     glColor3ub(TEX_WORLD_TERRAIN_OCEAN_00.averageRed,TEX_WORLD_TERRAIN_OCEAN_00.averageGreen,TEX_WORLD_TERRAIN_OCEAN_00.averageBlue);
                  }
                  else if(world->isLand(tileX,tileY)==true)
                  {
                     glColor3ub(TEX_WORLD_TERRAIN_GRASS_00.averageRed,TEX_WORLD_TERRAIN_GRASS_00.averageGreen,TEX_WORLD_TERRAIN_GRASS_00.averageBlue);
                  }
						
					}
               // draw normal colour mode
               else
               {
                  // Cool feature where we keep track of the average colour of a texture and just draw that colour
                  // in colour mode. Could also be adapted for distant rendering.
                  // it allows for much smoother transition between texture and colur mode
                  if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==false)
                  {
                     glColor3ub(TEX_WORLD_TERRAIN_OCEAN_00.averageRed,TEX_WORLD_TERRAIN_OCEAN_00.averageGreen,TEX_WORLD_TERRAIN_OCEAN_00.averageBlue);
                  }
                  else if(world->isSafe(tileX,tileY)==true && world->isLand(tileX,tileY)==true)
                  {
                     //glColor3ub(TEX_WORLD_TERRAIN_GRASS_00.averageRed,TEX_WORLD_TERRAIN_GRASS_00.averageGreen,TEX_WORLD_TERRAIN_GRASS_00.averageBlue);
                     Texture * tex = world->aWorldTile(tileX,tileY).currentTexture();
                     if ( tex )
                     {
                        glColor3ub(tex->averageRed,tex->averageGreen,tex->averageBlue);
                     }
                  }
                  else
                  {
                     //glColor3ub(world->aTopoMap(tileX,tileY,0),world->aTopoMap(tileX,tileY,1),world->aTopoMap(tileX,tileY,2));
                  }
               }
            }

            // final case
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


	// return the maximum number of tiles that fit on the current panel along X axis
	double getTilesNX()
	{
		return mainViewNX/tileSize;
	}
	// return the maximum number of tiles that fit on the current panel along Y axis
	double getTilesNY()
	{
		return mainViewNY/tileSize;
	}

};
WorldViewer worldViewer;


void RainManager::render()
{
   if (world==0 || worldViewer.pixelsPerLocalTile < 1)
   {return;}

   unsigned long int bX,bY;
   worldViewer.toTileCoords(0,0,&bX,&bY);

   Renderer::setColourMode();
   glColor3ub(0,0,220);
   for (int i=0;i<vRainDrop.size();++i)
   {
      int rX = vRainDrop(i)->x;
      int rY = vRainDrop(i)->y;
      unsigned long int aX,aY;

      worldViewer.toTileCoords(rX,rY,&aX,&aY);

      if ( world->isGenerated(aX,aY) )
      {
         if ( (*world)(aX,aY)->hasFloor == 0 )
         {
            vRainDrop(i)->render();
         }
      }
      else
      {
         vRainDrop(i)->render();
      }
   }
   glColor3ub(255,255,255);
   Renderer::setTextureMode();
}

#endif
