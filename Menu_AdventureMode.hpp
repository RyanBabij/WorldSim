#pragma once
#ifndef WORLDSIM_MENU_ADVENTUREMODE_HPP
#define WORLDSIM_MENU_ADVENTUREMODE_HPP

#include "World_Viewer.hpp"

/* Adventure mode menu. The player can select a character and play as them. */

/* Menu_AdventureMode.cpp
	#include "Menu_AdventureMode.hpp"

  Adventure mode menu. The player can select a character and play as them.
  This class contains the code for the Adventure Mode GUI.
*/

class Menu_AdventureMode: public GUI_Interface
{
  private:
  	GUI_Manager guiManager;
  
  public:
  

	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
  
	Menu_AdventureMode()
	{	
		// backgroundTexture=&TEX_NEW_GAME_BACKGROUND;

		// worldViewer.world = &world;
		// worldViewer.active = true;
		// simulateWorld=false;
		
		// buttonExpandMap.text="Expand";
		// buttonExpandMap.setColours(&cNormal,&cHighlight,0);
		// //buttonExpandMap.setPanel(0,0,32,32);
		// buttonExpandMap.texture = &TEX_GUI_EXPAND;
		

		// fullScreenWorldView=true;
		
		// menuTribes.active=false;
		// menuWorld.active=false;
		// menuCharacter.active=false;
		// menuBiome.active=false;
	}
  
  
};

#endif
