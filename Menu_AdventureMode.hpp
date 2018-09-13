#pragma once
#ifndef WORLDSIM_MENU_ADVENTUREMODE_HPP
#define WORLDSIM_MENU_ADVENTUREMODE_HPP

#include "World_Viewer.hpp"

/* Menu_AdventureMode.hpp
	#include "Menu_AdventureMode.hpp"

  Adventure mode menu. The player can select a character and play as them.
  This class contains the code for the Adventure Mode GUI.
*/

class Menu_AdventureMode: public GUI_Interface
{
  private:
  	GUI_Manager guiManager;
    
    /* Colours / theme. */
    Colour cNormal;
    Colour cSelected;
    Colour cDropPanel;
    Colour cHighlight;
    
    Wildcat::Font* font;
    
    /* Texture to draw in the background of this menu. */
    Texture* backgroundTexture;
    
  
  public:
  

	

	Menu_AdventureMode()
	{	
		backgroundTexture=&TEX_NEW_GAME_BACKGROUND;
	}
  
  void init()
  {
		/* Initialise theme. */
		cNormal.set(200,200,200);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(170,170,170);
    eventResize();
  }
  
  
  void render()
  {
    if ( active == false )
    { return; }
  
		// MESSAGE CONSOLE		
		const int nY = panelY2-panelY1;
		Renderer::placeColour4a(200,200,250,125,panelX1,panelY1+120,panelX1+220,panelY2);
		int currentY = panelY2-2;
		int iMessage = vConsoleMessage.size()-1;
    
    int linesDrawn = 0;
    
		while (currentY>panelY1+120 && iMessage>=0 && iMessage<vConsoleMessage.size() )
		{
			linesDrawn = font8x8.drawText(vConsoleMessage(iMessage),panelX1,currentY,panelX1+220,currentY-38,false,false);
			--iMessage;
			currentY-=linesDrawn*9;
      currentY-=2;
		}

    Renderer::placeColour4a(200,200,250,250,panelX1,panelY1,panelX1+220,panelY1+120);
  
  
    guiManager.render();
  }
  
	void logicTick()
	{
    if ( active == false )
    { return; }
	}
  
	bool keyboardEvent (Keyboard* _keyboard)
	{
    if ( active == false )
    { return false; }
  
			// ESCAPE - Close all submenus and go back to main game.
			// If all submenus are already closed, bring up main menu.
		if(_keyboard->isPressed(Keyboard::ESCAPE)) /* Flush console. */
		{
			_keyboard->keyUp(Keyboard::ESCAPE);	
		}

		guiManager.keyboardEvent(_keyboard);
		worldViewer.keyboardEvent(_keyboard);
		return false;
	}
  
	bool mouseEvent (Mouse* _mouse)
	{
    if ( active == false )
    { return false; }
		worldViewer.mouseEvent(_mouse);

		return false;
	}
  
	void eventResize()
	{
		worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);

	}
	
  
};

#endif
