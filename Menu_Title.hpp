#pragma once
#ifndef WORLDSIM_MENU_TITLE_HPP
#define WORLDSIM_MENU_TITLE_HPP

/* WorldSim: Menu_Title
	#include "Menu_Title.hpp"
	
	The title menu. This is the main menu, appearing when the application is launched,
	and also when the player calls the main menu from within the game. It displays some
	fancy opening cutscene, or some nice picture, and the basic options for starting
	and configuring your game.
*/


#include "Menu_WorldGenerator.hpp"
#include "Menu_Options.hpp"
#include "Menu_LoadGame.hpp"

class Menu_Title: public GUI_Interface, public LogicTickInterface
{
	public:

	/* Submenus. */
	/* New game -> Goes to world generator. */
	Menu_WorldGenerator menuWorldGenerator;
	/* Options. */
	//Menu_Options menuOptions;
	/* Load Game. */
	Menu_LoadGame menuLoadGame;

	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	/* Button new game. */
	GUI_Button buttonNewGame;
	/* Button: Load game. */
	GUI_Button buttonLoadGame;
	/* Button: Options. */
	GUI_Button buttonOptions;
	/* Button: Quit */
	GUI_Button buttonQuit;

	
	/* Button: For testing random things. */
	GUI_Button buttonTestSomething;
	
	/* GUI manager. Manages all GUI controls for this menu. */
	GUI_Manager guiManager;
	
	/* Background image */
	Texture* backgroundTexture;

	Menu_Title()
	{
		active=false;
		panelX1=0; panelY1=0; panelX2=0; panelY2=0;
		font=0;
		
		menuWorldGenerator.active=false;
		active=true;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		//menuOptions.setFont(_font);
		menuLoadGame.setFont(_font);
		menuWorldGenerator.setFont(_font);
		guiManager.setFont(_font);
	}

	void eventResize()
	{
		/* Update control positions. */
		buttonNewGame.setPanel(panelCenterX-60, panelY2-20, panelCenterX+60, panelY2-40);
		buttonLoadGame.setPanel(panelCenterX-60, panelY2-42, panelCenterX+60, panelY2-62);
		buttonOptions.setPanel(panelCenterX-60, panelY2-64, panelCenterX+60, panelY2-84);
		buttonQuit.setPanel(panelCenterX-60, panelY2-86, panelCenterX+60, panelY2-106);
		buttonTestSomething.setPanel(panelCenterX-60, panelY2-108, panelCenterX+60, panelY2-128);

		/* Pass resize down to submenus. */
		menuWorldGenerator.setPanel(panelX1,panelY1,panelX2,panelY2);
		//menuOptions.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuLoadGame.setPanel(panelX1,panelY1,panelX2,panelY2);

	}
	
	void init()
	{
		cNormal.set(200,200,200);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(170,170,170);
	
		buttonNewGame.text="1. New Game";
		buttonNewGame.setColours(&cNormal,&cHighlight,0);
		
		buttonLoadGame.text="Load Game";
		buttonLoadGame.setColours(&cNormal,&cHighlight,0);
		
		buttonOptions.text="Options";
		buttonOptions.setColours(&cNormal,&cHighlight,0);

		buttonQuit.text="Quit";
		buttonQuit.setColours(&cNormal,&cHighlight,0);
		
		buttonTestSomething.text="Test map";
		buttonTestSomething.setColours(&cNormal,&cHighlight,0);
		buttonTestSomething.active=true;
		
		/* Update GUI positions. */
		eventResize();
		
		guiManager.addControl(&buttonNewGame);
		guiManager.addControl(&buttonLoadGame);
		guiManager.addControl(&buttonOptions);
		guiManager.addControl(&buttonQuit);
		guiManager.addControl(&buttonTestSomething);
		
		setFont(font);
		
		//menuOptions.init();
		menuLoadGame.init();
		menuWorldGenerator.init();

		menuWorldGenerator.active = false;
		active=true;
		
		if ( QUICKSTART )
		{
			menuWorldGenerator.active=true;
		}

	
	}
	/* DisplayInterface:: */
	void render()
	{
		if(menuWorldGenerator.active)
		{
			menuWorldGenerator.render();
		}
		else if(active)
		{
		
			//if(menuOptions.active)
			//{
				//menuOptions.render();
			//}
      if(menuLoadGame.active)
			{
				menuLoadGame.render();
			}
			else
			{
				/* Background image. Stretch to fit, preserve aspect ratio. */
				Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
				guiManager.render();
			}
			
		}
	}
  
	/* MouseInterface:: */
	bool mouseEvent (Mouse* _mouse)
	{
		if(menuWorldGenerator.active)
		{
			menuWorldGenerator.mouseEvent(_mouse);
		}
		else if(active)
		{
		
			//if(menuOptions.active)
			//{
				//menuOptions.mouseEvent(_mouse);
			//}
      if(menuLoadGame.active)
			{
				menuLoadGame.mouseEvent(_mouse);
			}
			else
			{
				/* If the guiManager did something with the mouse event. */
				if(guiManager.mouseEvent(_mouse)==true)
				{
					if(buttonNewGame.clicked==true)
					{
						buttonNewGame.unclick();
						menuWorldGenerator.active=true;
						return true;
					}
					if(buttonQuit.clicked==true)
					{
						QUIT_FLAG=true;
						buttonQuit.clicked=false;
					}
					if(buttonOptions.clicked==true)
					{
						buttonOptions.clicked=false;
            activeMenu = MENU_OPTIONS;
						//menuOptions.active=true;
					}
					if(buttonLoadGame.clicked==true)
					{
						buttonLoadGame.clicked=false;
						menuLoadGame.active=true;
					}
          if ( buttonTestSomething.clicked == true )
          {
            buttonTestSomething.clicked = false;
            buttonTestSomething.text="Yep it works";
          }

				}
			}
		}
		return false;
	}
	
	void logicTick()
	{
		if(menuWorldGenerator.active==true)
		{
			menuWorldGenerator.logicTick();
		}
	}
	
	bool keyboardEvent(Keyboard* _keyboard)
	{
		if (menuWorldGenerator.active == true )
		{
			menuWorldGenerator.keyboardEvent(_keyboard);
		}
		else if ( guiManager.keyboardEvent(_keyboard) )
		{
			return true;
		}
		
		if (_keyboard->isPressed(Keyboard::ONE) )
    {
      _keyboard->unpress(Keyboard::ONE);
      menuWorldGenerator.active=true;
      return true;
    }
		
		return false;
	}
	
	bool stealKeyboard()
	{
		return guiManager.stealKeyboard();
	}
	
};

#endif