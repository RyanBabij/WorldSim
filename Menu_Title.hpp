#pragma once
#ifndef WORLDSIM_MENU_TITLE_HPP
#define WORLDSIM_MENU_TITLE_HPP

/*
	#include "Menu_Title.hpp"
	
	The title menu. This is the main menu, appearing when the application is launched,
	and also when the player calls the main menu from within the game. It displays some
	fancy opening thingy, or some nice picture, and the basic options for starting
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
	Menu_Options menuOptions;
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
		//buttonNewGame.clicked = AUTO_NEW_SIMULATION;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		menuOptions.setFont(_font);
		menuLoadGame.setFont(_font);
		menuWorldGenerator.setFont(_font);
		guiManager.setFont(_font);
	}
	
	/* This is used for init, and also called when the panel is resized. */
	// void setPanel(const int _panelX1, const int _panelY1, const int _panelX2, const int _panelY2)
	// {
	
		// /* Sort the x and y coords. */
		// if(_panelX1<=_panelX2)
		// { panelX1=_panelX1; panelX2=_panelX2; }
		// else
		// { panelX2=_panelX1; panelX1=_panelX2; }
		// if(_panelY1<=_panelY2)
		// { panelY1=_panelY1; panelY2=_panelY2; }
		// else
		// { panelY2=_panelY1; panelY1=_panelY2; }
		
		// menuNewGame.setPanel(panelX1,panelY1,panelX2,panelY2);
		// menuOptions.setPanel(panelX1,panelY1,panelX2,panelY2);
		// menuLoadGame.setPanel(panelX1,panelY1,panelX2,panelY2);
		
		// /* Resize the GUI controls. */
		
	// }
	
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
		menuOptions.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuLoadGame.setPanel(panelX1,panelY1,panelX2,panelY2);

	}
	
	void init()
	{
		//std::cout<<"Menu title init\n";
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
		
		buttonTestSomething.text="Test something";
		buttonTestSomething.setColours(&cNormal,&cHighlight,0);
		buttonTestSomething.active=true;
		
		/* Update GUI positions. */
		eventResize();
		
		//guiManager.addControl(&menuWorldGenerator);
		
		guiManager.addControl(&buttonNewGame);
		guiManager.addControl(&buttonLoadGame);
		guiManager.addControl(&buttonOptions);
		guiManager.addControl(&buttonQuit);
		guiManager.addControl(&buttonTestSomething);
		
		setFont(font);
		
		menuOptions.init();
		menuLoadGame.init();
		menuWorldGenerator.init();
		
		// if (QUICKSTART==true)
		// {
			// menuNewGame.active=true;
		// }
		menuWorldGenerator.active = false;
		active=true;
		
		if ( QUICKSTART )
		{
			menuWorldGenerator.active=true;
			//active = false;
		}
		
		
	
	}
	/* DisplayInterface:: */
	void render()
	{
		//std::cout<<"menu title render call\n";
		if(menuWorldGenerator.active)
		{
			//std::cout<<"Worldgen active\n";
			menuWorldGenerator.render();
		}
		else if(active)
		{
			//std::cout<<"title active\n";
			
			// Currently a bad design but not important to fix right now.
		
			if(menuOptions.active)
			{
				menuOptions.render();
			}
			else if(menuLoadGame.active)
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

		
		//if(menuNewGame.active)
		//{
			//menuNewGame.render();
		//}
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
		
			if(menuOptions.active)
			{
				menuOptions.mouseEvent(_mouse);
			}
			else if(menuLoadGame.active)
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
						//active=false;
					
						//std::cout<<"New game\n";
						//buttonNewGame.clicked=false;
						//menuNewGame.active=true;
						//menuNewGame.setPanel(panelX1,panelX2,panelY1,panelY2);
						
						//deactivate self.
						//active=false;
						
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
						menuOptions.active=true;
					}
					if(buttonLoadGame.clicked==true)
					{
						buttonLoadGame.clicked=false;
						menuLoadGame.active=true;
						std::cout<<"Load game.\n";
						
					}
					if (buttonTestSomething.clicked == true)
					{
						std::cout<<"Uh... 1+1=2. Yep it works.\n";
            buttonTestSomething.unclick();
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