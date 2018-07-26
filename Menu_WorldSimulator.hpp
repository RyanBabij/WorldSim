#pragma once
#ifndef WORLDSIM_MENU_WORLDSIMULATOR_HPP
#define WORLDSIM_MENU_WORLDSIMULATOR_HPP

#include "World_Viewer.hpp"

#include "Menu_Tribes.hpp"
#include "Menu_Civs.hpp"
#include "Menu_World.hpp"
#include "Menu_Character.hpp"
#include "Menu_Biome.hpp"

/* World simulator menu. The player can simulate the world to build up civilisation, before jumping in. The player can then select on of the inhabitants to play as. */
class Menu_WorldSimulator: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
	
	//GUI_TextBox menuHeading;
	
	/* Texture to draw in the background of this menu. */
	Texture* backgroundTexture;
	
	/* Button: Resume or commence simulation of the world map. */
	GUI_Button buttonStartSimulation;
	/* Button: Pause the simulation so the player can look around the world as it currently exists. */
	GUI_Button buttonPauseSimulation;
	/* Button: Step the simulation forward by a certain amount of time. (Day/Month/Year). */
	
	/* Number entry: Simulation speed. */
	GUI_CycleButton cycleSimulationSpeed;
	/* Textbox: Caption for simulation speed. */
	GUI_TextBox textSimulationSpeed;
	
	GUI_Button buttonIncrementDay;
	GUI_Button buttonIncrementMonth;
	GUI_Button buttonIncrementYear;
	GUI_Button buttonIncrementDecade;
	
		/* Menu for investigating an individual tribe */
	GUI_Button buttonTribeMenu;
	
		/* Menu for seeing info about the world. */
	GUI_Button buttonWorldMenu;
	
		/* Menu for a list of all biomes. */
	GUI_Button buttonBiomeMenu;
	
		/* List of all characters in the game. */
	GUI_Button buttonCharacterMenu;
	
		/* Toggle tribal territory visibility */
	GUI_Button buttonTerritoryView;
  
		/* List of all Civs in the game */
	GUI_Button buttonCivMenu;
  
		/* Switch to tileset view */
	GUI_Button buttonToggleTileset;
  
		/* Switch to subterranean view on local map */
	GUI_Button buttonSubterraneanView;
	
	/* Button: Switch between 1/4 or fullscreen view of world map. */
	GUI_Button buttonExpandMap;

	bool simulateWorld;
	
	bool fullScreenWorldView;
	
	/* Use this worldViewer instead of a global one. Just make sure to attach the world pointer. */
	//WorldViewer worldViewer;
	//World* world;
	
	/* Submenus */
	Menu_Tribes menuTribes;
	Menu_Civs menuCivs;
	Menu_World menuWorld;
	Menu_Characters menuCharacter;
	Menu_Biome menuBiome;
	
	
	Menu_WorldSimulator()
	{	
		backgroundTexture=&TEX_NEW_GAME_BACKGROUND;

		worldViewer.world = &world;
		worldViewer.active = true;
		simulateWorld=false;
		
		buttonExpandMap.text="Expand";
		buttonExpandMap.setColours(&cNormal,&cHighlight,0);
		//buttonExpandMap.setPanel(0,0,32,32);
		buttonExpandMap.texture = &TEX_GUI_EXPAND;
		

		fullScreenWorldView=true;
		
		menuTribes.active=false;
		menuWorld.active=false;
		menuCharacter.active=false;
		menuBiome.active=false;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
	
	void init()
	{
	
		/* Initialise theme. */
		cNormal.set(200,200,200);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(170,170,170);
		
		cycleSimulationSpeed.wrap=false;
		cycleSimulationSpeed.currentOption=0;
		cycleSimulationSpeed.addOption("PAUSED");
		cycleSimulationSpeed.addOption("1x");
		cycleSimulationSpeed.addOption("60x");
		cycleSimulationSpeed.addOption("100x");
		cycleSimulationSpeed.addOption("1,000x");
		cycleSimulationSpeed.addOption("10,000x");
		cycleSimulationSpeed.addOption("100,000x");
		cycleSimulationSpeed.addOption("1,000,000x");
		cycleSimulationSpeed.addOption("10,000,000x");
		cycleSimulationSpeed.addOption("100,000,000x");
		cycleSimulationSpeed.texCycleButton=&TEX_GUI_DECREMENT;
		cycleSimulationSpeed.setColours(&cNormal,&cHighlight,0);
		
		textSimulationSpeed.text="Speed:";
		textSimulationSpeed.setColours(&cNormal);
		
		buttonIncrementDay.text = "+D";
		buttonIncrementDay.setColours(&cNormal,&cHighlight,0);
		buttonIncrementMonth.text = "+M";
		buttonIncrementMonth.setColours(&cNormal,&cHighlight,0);
		buttonIncrementYear.text = "+Y";
		buttonIncrementYear.setColours(&cNormal,&cHighlight,0);
		buttonIncrementDecade.text = "+D";
		buttonIncrementDecade.setColours(&cNormal,&cHighlight,0);
		
		
		
		buttonTribeMenu.text="T";
		buttonTribeMenu.setColours(&cNormal,&cHighlight,0);
		
		buttonWorldMenu.text="W";
		buttonWorldMenu.setColours(&cNormal,&cHighlight,0);
		
		buttonBiomeMenu.text="B";
		buttonBiomeMenu.setColours(&cNormal,&cHighlight,0);
		
		
		buttonCharacterMenu.text="C";
		buttonCharacterMenu.setColours(&cNormal,&cHighlight,0);
    
		buttonCivMenu.text="Civ";
		buttonCivMenu.setColours(&cNormal,&cHighlight,0);
    
		buttonToggleTileset.text="V";
		buttonToggleTileset.setColours(&cNormal,&cHighlight,0);
    
    buttonSubterraneanView.text = "S";
    buttonSubterraneanView.setColours(&cNormal,&cHighlight,0);
		
		buttonTerritoryView.text="T2";
		buttonTerritoryView.setColours(&cNormal,&cHighlight,0);
		
		guiManager.add(&textSimulationSpeed);
		guiManager.add(&cycleSimulationSpeed);
		guiManager.add(&buttonIncrementDay);
		guiManager.add(&buttonIncrementMonth);
		guiManager.add(&buttonIncrementYear);
		guiManager.add(&buttonIncrementDecade);
		
		guiManager.add(&buttonTribeMenu);
		guiManager.add(&buttonWorldMenu);
		guiManager.add(&buttonBiomeMenu);
		guiManager.add(&buttonCharacterMenu);
		guiManager.add(&buttonCivMenu);
		guiManager.add(&buttonTerritoryView);
		guiManager.add(&buttonToggleTileset);
		guiManager.add(&buttonSubterraneanView);
		
		/* Submenus */
		guiManager.add(&menuTribes);
		guiManager.add(&menuCivs);
		guiManager.add(&menuWorld);
		guiManager.add(&menuCharacter);
		guiManager.add(&menuBiome);


		textSimulationSpeed.active=true;
		cycleSimulationSpeed.active=true;

		buttonIncrementDay.active=true;
		buttonIncrementMonth.active=true;
		buttonIncrementYear.active=true;
		buttonIncrementDecade.active=true;
		
		buttonTribeMenu.active=true;
		buttonWorldMenu.active=true;
		buttonBiomeMenu.active=true;
		buttonCharacterMenu.active=true;
		buttonCivMenu.active=true;
		buttonTerritoryView.active=true;
		buttonToggleTileset.active=true;
		buttonSubterraneanView.active=true;
		
		buttonExpandMap.active = false;
		buttonStartSimulation.active = false;
		buttonPauseSimulation.active = false;
		
		menuTribes.init();
		menuCivs.init();
		menuWorld.init();
		menuCharacter.init();
		menuBiome.init();
		
		worldViewer.centerView();
	
		eventResize();
	}
	
	void render()
	{
		/* Draw background texture. */
		if ( fullScreenWorldView==true)
		{
			Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
		}
		else
		{
			Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
		}
		
		
		/* Preview world view. */
	
		worldViewer.render();


		
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
    
		buttonExpandMap.render();

		// DATE
		Renderer::placeColour4a(150,150,150,255,panelX2-208,panelY2-20,panelX2,panelY2-10);
		std::string strDate = world.calendar.toString();
		font8x8.drawText("DATE: "+strDate,panelX2-208,panelY2-20,panelX2,panelY2-10, false, true);
		
		worldPop=0;
		for (int i=0;i<world.vTribe.size();++i)
		{
			worldPop+=world.vTribe(i)->vCharacter.size();
		}
		
    //Population
    Renderer::placeColour4a(150,150,150,255,panelX2-380,panelY2-20,panelX2-208,panelY2-10);
    font8x8.drawText("Pop: "+DataTools::toString(worldPop),panelX2-380,panelY2-20,panelX2-208,panelY2-10, false, true);
    
		guiManager.render();
			
	}
	
	void logicTick()
	{
		//std::cout<<"WorldSim: LogicTick().\n";
	
		//out("WorldSim: LogicTick()");
		
		//textDate.text = world.date.toString("-");
		
		if(active)
		{
			//std::cout<<"Simulating world.\n";
			
			//std::cout<<"Sim speed: "<<cycleSimulationSpeed.getCurrentOption()<<".\n";
			
			if ( cycleSimulationSpeed.getCurrentOption() == "PAUSED" )
			{
				//PAUSE_LOGIC=true;
				//std::cout<<"Simulation paused.\n";
				TIME_SCALE=0;
			}
			if ( cycleSimulationSpeed.getCurrentOption() == "1x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 1;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "60x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 60;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "100x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 100;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "1,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 1000;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "10,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 10000;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "100,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 100000;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "1,000,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 1000000;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "10,000,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 10000000;
			}
			else if ( cycleSimulationSpeed.getCurrentOption() == "100,000,000x" )
			{
				//PAUSE_LOGIC=false;
				TIME_SCALE = 100000000;
			}
			
		}
	}
	
	bool keyboardEvent (Keyboard* _keyboard)
	{
			// ESCAPE - Close all submenus and go back to main game.
			// If all submenus are already closed, bring up main menu.
		if(_keyboard->isPressed(Keyboard::ESCAPE)) /* Flush console. */
		{

			menuTribes.active = false;
			menuCivs.active = false;
			menuWorld.active = false;
			menuCharacter.active = false;
			menuBiome.active = false;
			
			world.ticksBacklog=0;
			
			_keyboard->keyUp(Keyboard::ESCAPE);	
		}

		guiManager.keyboardEvent(_keyboard);
		
		worldViewer.keyboardEvent(_keyboard);
		return false;
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
		
		worldViewer.mouseEvent(_mouse);
		buttonExpandMap.mouseEvent(_mouse);
		
		if(buttonExpandMap.clicked==true)
		{
			fullScreenWorldView=!fullScreenWorldView;
			
			if (fullScreenWorldView==true)
			{
				worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
				buttonExpandMap.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
			}
			else
			{
				worldViewer.setPanel(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX2,panelY2);
				buttonExpandMap.setPanel(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX1+(panelX2/2)+16,panelY1+(panelY2/2)+16);
			}
			
			buttonExpandMap.unclick();
			eventResize();
		}

		if  (buttonIncrementDay.clicked==true)
		{
			//std::cout<<"Increment day.\n";
			buttonIncrementDay.unclick();
			
			world.incrementTicks(86400);
		}
		if  (buttonIncrementMonth.clicked==true)
		{
			//std::cout<<"Increment month.\n";
			buttonIncrementMonth.unclick();
			
			world.incrementTicks(2592000);
		}
		if  (buttonIncrementYear.clicked==true)
		{
			//std::cout<<"Increment year.\n";
			buttonIncrementYear.unclick();
			//world.incrementTicks(31104000);
			world.ticksBacklog+=31104000;
		}
		if  (buttonIncrementDecade.clicked==true)
		{
			//std::cout<<"Increment decade.\n";
			buttonIncrementDecade.unclick();
			world.ticksBacklog+=311040000;
		}
		
		if  (buttonTribeMenu.clicked==true)
		{
			menuTribes.init();
			menuTribes.active=true;
			buttonTribeMenu.unclick();
		}
		
		if  (buttonWorldMenu.clicked==true)
		{
			std::cout<<"Worldmenu\n";
			menuWorld.active=true;
			buttonWorldMenu.unclick();
		}
		
		if  (buttonCharacterMenu.clicked==true)
		{
			std::cout<<"Character menu\n";
			menuCharacter.init();
			menuCharacter.active=true;
			buttonCharacterMenu.unclick();
		}
    
		if (buttonCivMenu.clicked==true)
		{
			std::cout<<"Civ menu\n";
			menuCivs.init();
			menuCivs.active=true;
			buttonCivMenu.unclick();
		}

		if  (buttonTerritoryView.clicked==true)
		{
			std::cout<<"Toggle territory visibility.\n";
			worldViewer.territoryView=!worldViewer.territoryView;
			buttonTerritoryView.unclick();
		}
		
		if  (buttonBiomeMenu.clicked==true)
		{
			std::cout<<"Biome menu\n";
			menuBiome.init();
			menuBiome.active=true;
			buttonBiomeMenu.unclick();
		}
		
		if  (buttonToggleTileset.clicked==true)
		{
			std::cout<<"Toggle tileset view\n";
      worldViewer.tilesetMode = !worldViewer.tilesetMode;
			buttonToggleTileset.unclick();
		}
		
		if  (buttonSubterraneanView.clicked==true)
		{
			std::cout<<"Toggle subterranean mode\n";
      worldViewer.subterraneanMode = !worldViewer.subterraneanMode;
			buttonSubterraneanView.unclick();
		}
		
		
			/* If the guiManager did something with the mouse event. */
		if(guiManager.mouseEvent(_mouse)==true)
		{
			
		}
		
		if (menuTribes.lastRowClicked != -1 )
		{
			if ( world.vTribe.isSafe(menuTribes.lastRowClicked) )
			{
				Tribe* t = world.vTribe(menuTribes.lastRowClicked);
				worldViewer.setCenterTile(t->worldX,t->worldY);
			}
			
			
			menuTribes.lastRowClicked=-1;
		}
		
		if (menuWorld.lastRowClicked != -1 )
		{
			if ( world.vLandmass.isSafe(menuWorld.lastRowClicked) )
			{
				World_Landmass* l = world.vLandmass(menuWorld.lastRowClicked);
				worldViewer.setCenterTile(l->averageX,l->averageY);
			}
			
			
			menuWorld.lastRowClicked=-1;
		}
		
		return false;
	}
	
	void eventResize()
	{
	
	/* World preview takes top-right quarter of screen. */

		int panelSizeX = panelX2-panelX1;
		int panelSizeY = panelY2-panelY1;
		
		int worldPreviewX1=panelX1+(panelSizeX/2);
		int worldPreviewY1=panelY1+(panelSizeY/2);
				
		/* Left column, centered. */
		const int leftColumnCenterX = panelX1+(panelNX/4);
				
		Renderer::placeColour4(0,0,0,worldPreviewX1,worldPreviewY1,panelX2,panelY2);
		worldViewer.setPanel(worldPreviewX1,worldPreviewY1,panelX2,panelY2);

		textSimulationSpeed.setPanel(panelX2-200, panelY2-30, panelX2-100, panelY2-20);
		cycleSimulationSpeed.setPanel(panelX2-150, panelY2-30, panelX2, panelY2-20);
		
		buttonIncrementDay.setPanel(panelX2-150, panelY2-40, panelX2-120, panelY2-30);
		buttonIncrementMonth.setPanel(panelX2-120, panelY2-40, panelX2-90, panelY2-30);
		buttonIncrementYear.setPanel(panelX2-90, panelY2-40, panelX2-60, panelY2-30);
		buttonIncrementDecade.setPanel(panelX2-60, panelY2-40, panelX2-30, panelY2-30);
		
		buttonTribeMenu.setPanel(panelX2-180, panelY2-40, panelX2-150, panelY2-30);
		buttonWorldMenu.setPanel(panelX2-210, panelY2-40, panelX2-180, panelY2-30);
		buttonCharacterMenu.setPanel(panelX2-240, panelY2-40, panelX2-210, panelY2-30);
		buttonBiomeMenu.setPanel(panelX2-270, panelY2-40, panelX2-240, panelY2-30);
		buttonTerritoryView.setPanel(panelX2-300, panelY2-40, panelX2-270, panelY2-30);
		buttonCivMenu.setPanel(panelX2-340, panelY2-40, panelX2-300, panelY2-30);
		buttonToggleTileset.setPanel(panelX2-370, panelY2-40, panelX2-340, panelY2-30);
		buttonSubterraneanView.setPanel(panelX2-400, panelY2-40, panelX2-370, panelY2-30);

		menuTribes.setPanel(panelX1+20,panelY1+20,panelX2-20,panelY2-20);
		menuCivs.setPanel(panelX1+20,panelY1+20,panelX2-20,panelY2-20);
		menuWorld.setPanel(panelX1+20,panelY1+20,panelX2-20,panelY2-20);
		menuCharacter.setPanel(panelX1+20,panelY1+20,panelX2-20,panelY2-20);
		menuBiome.setPanel(panelX1+20,panelY1+20,panelX2-20,panelY2-20);
		
		if (fullScreenWorldView==true)
		{
			worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
			buttonExpandMap.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
		}
		else
		{
			worldViewer.setPanel(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX2,panelY2);
			buttonExpandMap.setPanel(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX1+(panelX2/2)+16,panelY1+(panelY2/2)+16);
		}
	
	}
	
};

#endif
