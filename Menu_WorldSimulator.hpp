#pragma once
#ifndef WORLDSIM_MENU_WORLDSIMULATOR_HPP
#define WORLDSIM_MENU_WORLDSIMULATOR_HPP

/* WorldSim: Menu_WorldSimulator.hpp
#include "Menu_WorldSimulator.hpp" */

/** @brief World simulator menu.
The player can simulate the world to build up civilisation, before jumping in.
The player can then select one of the inhabitants to play as.
*/

#include "World_Viewer.hpp"
#include "World_Viewer_Minimap.hpp"

#include "Menu_Tribes.hpp"
#include "Menu_Civs.hpp"
#include "Menu_Settlements.hpp"
#include "Menu_World.hpp"
#include "Menu_Character.hpp"
#include "Menu_Biome.hpp"
#include "Menu_Flora.hpp"
#include "Menu_Creatures.hpp"


class Menu_WorldSimulator: public GUI_Interface
{
private:
	GUI_Manager guiManager;
	Wildcat::Font* font;
	/* Texture to draw in the background of this menu. */
	Texture* backgroundTexture;

	/* Colours / theme. */
	ColourRGB <unsigned char> cNormal;
	ColourRGB <unsigned char> cSelected;
	ColourRGB <unsigned char> cDropPanel;
	ColourRGB <unsigned char> cHighlight;

	/* Button: Resume or commence simulation of the world map. */
	GUI_Button buttonStartSimulation;
	/* Button: Pause the simulation so the player can look around the world as it currently exists. */
	GUI_Button buttonPauseSimulation;
	/* Number entry: Simulation speed. */
	GUI_CycleButton cycleSimulationSpeed;
	/* Textbox: Caption for simulation speed. */
	GUI_TextBox textSimulationSpeed;
	/* Button: Step the simulation forward by a certain amount of time. (Day/Month/Year). */
	GUI_Button buttonIncrementDay;
	GUI_Button buttonIncrementMonth;
	GUI_Button buttonIncrementYear;
	GUI_Button buttonIncrementContinuous;
	
	//GUI_Button buttonIncrementContinuous;
	
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
	/* Switch to subterranean view on local map */
	GUI_Button buttonSubterraneanView;
	/* Button: Switch between 1/4 or fullscreen view of world map. */
	GUI_Button buttonExpandMap;
	
	/* Button to view list of all Flora in World */
	GUI_Button buttonFlora;
	/* Button to view list of all Creatures in the World */
	GUI_Button buttonCreaturesMenu;
	/* Button to view list of all Settlements in the World */
	GUI_Button buttonSettlementsMenu;
	
		// Base world view modes
		// Toggle Textures
	GUI_Button buttonMinimapBaseTexture;
		// Toggle landmass/biome view
	GUI_Button buttonMinimapBaseLand;

	bool simulateWorld;

	bool fullScreenWorldView;

	/* Submenus */
	Menu_Tribes menuTribes;
	Menu_Civs menuCivs;
	Menu_World menuWorld;
	Menu_Characters menuCharacter;
	Menu_Biome menuBiome;
	Menu_Flora menuFlora;
	Menu_Creatures menuCreatures;
	Menu_Settlements menuSettlements;
	
public:


	Menu_WorldSimulator()
	{	
		backgroundTexture=&TEX_NEW_GAME_BACKGROUND;

		worldViewer.world = &world;
		worldViewer.active = true;
		
		minimap.world = &world;
		minimap.active=true;
		
		simulateWorld=false;

		buttonExpandMap.text="Expand";
		buttonExpandMap.setColours(cNormal,cHighlight,0);
		//buttonExpandMap.setPanel(0,0,32,32);
		buttonExpandMap.texture = &TEX_GUI_EXPAND;

		fullScreenWorldView=true;

		menuTribes.active=false;
		menuWorld.active=false;
		menuCharacter.active=false;
		menuBiome.active=false;
		menuFlora.active=false;
		menuCreatures.active=false;
		menuSettlements.active=false;
		minimap.active=true;
		
		// Link MenuSettlementDetails and MenuCharacterDetails until I can implement a MenuManager
		menuSettlements.menuSettlementDetails.menuCharacterDetails = &menuCharacter.menuCharacterDetails;
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
		cycleSimulationSpeed.setColours(cNormal,cHighlight,0);

		textSimulationSpeed.text="Speed:";
		textSimulationSpeed.setColours(cNormal);

		buttonIncrementDay.text = "+D";
		buttonIncrementDay.setColours(cNormal,cHighlight,0);
		buttonIncrementMonth.text = "+M";
		buttonIncrementMonth.setColours(cNormal,cHighlight,0);
		buttonIncrementYear.text = "+Y";
		buttonIncrementYear.setColours(cNormal,cHighlight,0);
		buttonIncrementContinuous.text = "+++";
		buttonIncrementContinuous.setColours(cNormal,cHighlight,0);
		buttonTribeMenu.text="Tri";
		buttonTribeMenu.setColours(cNormal,cHighlight,0);
		buttonWorldMenu.text="Leg";
		buttonWorldMenu.setColours(cNormal,cHighlight,0);
		buttonBiomeMenu.text="Bio";
		buttonBiomeMenu.setColours(cNormal,cHighlight,0);
		buttonCharacterMenu.text="Cha";
		buttonCharacterMenu.setColours(cNormal,cHighlight,0);
		buttonCivMenu.text="Civ";
		buttonCivMenu.setColours(cNormal,cHighlight,0);
		buttonSubterraneanView.text = "Sub";
		buttonSubterraneanView.setColours(cNormal,cHighlight,0);
		buttonTerritoryView.text="Ter";
		buttonTerritoryView.setColours(cNormal,cHighlight,0);
		buttonFlora.text="F";
		buttonFlora.setColours(cNormal,cHighlight,0);
		buttonCreaturesMenu.text="Crt";
		buttonCreaturesMenu.setColours(cNormal,cHighlight,0);
		buttonSettlementsMenu.text="Cit";
		buttonSettlementsMenu.setColours(cNormal,cHighlight,0);
		
		
		buttonMinimapBaseTexture.init("", &TEX_GUI_MINIMAP_BASE_TEXTUREMODE, cNormal, cHighlight, 0, true);
		buttonMinimapBaseLand.init("", &TEX_GUI_MINIMAP_BASE_LANDMODE, cNormal, cHighlight, 0, true);
		
		guiManager.add(&minimap);
		
		guiManager.add(&textSimulationSpeed);
		guiManager.add(&cycleSimulationSpeed);
		guiManager.add(&buttonIncrementDay);
		guiManager.add(&buttonIncrementMonth);
		guiManager.add(&buttonIncrementYear);
		guiManager.add(&buttonIncrementContinuous);

		guiManager.add(&buttonTribeMenu);
		guiManager.add(&buttonWorldMenu);
		guiManager.add(&buttonBiomeMenu);
		guiManager.add(&buttonCharacterMenu);
		guiManager.add(&buttonCivMenu);
		guiManager.add(&buttonFlora);
		guiManager.add(&buttonCreaturesMenu);
		guiManager.add(&buttonSettlementsMenu);
		guiManager.add(&buttonTerritoryView);
		guiManager.add(&buttonSubterraneanView);
		
		/* GUI Minimap Buttons */
		guiManager.add(&buttonMinimapBaseTexture);
		guiManager.add(&buttonMinimapBaseLand);

		/* Submenus */
		guiManager.add(&menuTribes);
		guiManager.add(&menuCivs);
		guiManager.add(&menuWorld);
		guiManager.add(&menuCharacter);
		guiManager.add(&menuBiome);
		guiManager.add(&menuFlora);
		guiManager.add(&menuCreatures);
		guiManager.add(&menuSettlements);

		textSimulationSpeed.active=true;
		cycleSimulationSpeed.active=true;

		buttonIncrementDay.active=true;
		buttonIncrementMonth.active=true;
		buttonIncrementYear.active=true;
		buttonIncrementContinuous.active=true;

		buttonTribeMenu.active=true;
		buttonWorldMenu.active=true;
		buttonBiomeMenu.active=true;
		buttonCharacterMenu.active=true;
		buttonCivMenu.active=true;
		buttonFlora.active=true;
		buttonCreaturesMenu.active=true;
		buttonSettlementsMenu.active=true;
		buttonTerritoryView.active=true;
		buttonSubterraneanView.active=true;

		buttonExpandMap.active = false;
		buttonStartSimulation.active = false;
		buttonPauseSimulation.active = false;

		menuTribes.init();
		menuCivs.init();
		menuWorld.init();
		menuCharacter.init();
		menuBiome.init();
		menuFlora.init();
		menuCreatures.init();
		menuSettlements.init();
		//minimap.init();

		worldViewer.centerView();
		eventResize();
	}

	void render()
	{
		// if ( active == false )
		// {
		// if ( menuAdventureMode.active )
		// {
		// menuAdventureMode.render();
		// return;
		// }

		// return;
		// }
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



		// Minimap
		// I think we can assume it'll be a square, although in future a rectangle may be more realistic.
		Renderer::placeColour4a(250,200,250,250,panelX1,panelY1,panelX1+220,panelY1+220);
		

		// Tile info panel
		Renderer::placeColour4a(200,250,250,250,panelX1,panelY1+236,panelX1+220,panelY1+356);

		//if (world.queryWorldX != -1 && world.queryWorldY != -1)
		if ( world.isSafe(world.queryWorldX,world.queryWorldY) )
		{
			World_Local * tile = world.getTile(world.queryWorldX,world.queryWorldY);

			if ( tile != 0 )
			{
				// Tile stats
				font8x8.drawText
				(Stream() << tile->getTerrainName() << " ("<<world.queryWorldX<<", "<<world.queryWorldY<<")",
				panelX1,panelY1+310,panelX1+220,panelY1+320,false,true);
				font8x8.drawText
				(Stream() << "Landmass: "<< world.getLandmassName(world.queryWorldX,world.queryWorldY),
				panelX1,panelY1+300,panelX1+220,panelY1+310,false,true);
				font8x8.drawText
				(Stream() << "Biome: "<< world.getBiomeName(world.queryWorldX,world.queryWorldY),
				panelX1,panelY1+290,panelX1+220,panelY1+300,false,true);
				font8x8.drawText(Stream() << tile->baseMetal << " metal",
				panelX1,panelY1+280,panelX1+220,panelY1+290,false,true);
				// Elevation
				font8x8.drawText
				(Stream() << "Elevation: "<< tile->centerHeight << "m",
				panelX1,panelY1+270,panelX1+220,panelY1+280,false,true);
				// Tile image
				Renderer::setTextureMode();
				Renderer::placeTexture4(panelX1, panelY1+236, panelX1+32, panelY1+268, tile->currentTexture(), false);
			}
		}
		
		// Map info panel
		Renderer::placeColour4a(250,200,250,250,panelX1,panelY1+356,panelX1+220,panelY1+476);
		if ( world.isSafe(world.queryWorldX,world.queryWorldY) )
		{
			World_Local * tile = world.getTile(world.queryWorldX,world.queryWorldY);
			
			if ( tile != 0 )
			{
				if (tile->data && tile->data->aLocalTile.isSafe(world.queryWorldXLocal,world.queryWorldYLocal))
				{
					
					if ( tile->data->aLocalTile(world.queryWorldXLocal,world.queryWorldYLocal).objStatic )
					{
						const std::string sName =
						tile->data->aLocalTile(world.queryWorldXLocal,world.queryWorldYLocal).objStatic->getName();
						font8x8.drawText
						(Stream() << sName,
						panelX1,panelY1+418,panelX1+220,panelY1+428,false,true);
					}
					
					font8x8.drawText
					(Stream() << tile->data->aLocalTile
					(world.queryWorldXLocal,world.queryWorldYLocal).getName()
					<< " ("<<world.queryWorldXLocal<<", "<<world.queryWorldYLocal<<")",
					panelX1,panelY1+408,panelX1+220,panelY1+418,false,true);
					

					
					font8x8.drawText
					(Stream() << tile->getTerrainName() << " ("<<world.queryWorldXLocal<<", "<<world.queryWorldYLocal<<")",
					panelX1,panelY1+398,panelX1+220,panelY1+408,false,true);
					font8x8.drawText
					(Stream() << "Elevation: "<< tile->data->aLocalTile
					(world.queryWorldXLocal,world.queryWorldYLocal).height << "m",
					panelX1,panelY1+388,panelX1+220,panelY1+398,false,true);
				}
				// Tile image
				Renderer::setTextureMode();
				Renderer::placeTexture4(panelX1, panelY1+356, panelX1+32, panelY1+388, tile->currentTexture(), false);
			}
		}

		buttonExpandMap.render();

		// DATE
		Renderer::placeColour4a(150,150,150,255,panelX2-208,panelY2-20,panelX2,panelY2-10);
		std::string strDate = globalCalendar.toString();
		font8x8.drawText("DATE: "+strDate,panelX2-208,panelY2-20,panelX2,panelY2-10, false, true);

		worldPop=0;
		for (int i=0;i<world.vTribe.size();++i)
		{
			worldPop+=world.vTribe(i)->vCharacter.size();
		}
		
		for (int i=0;i<world.vCiv.size();++i)
		{
			//Todo: Remove dead civ members from vector.
			worldPop+=world.vCiv(i)->vCharacter.size();
		}

		//Population
		Renderer::placeColour4a(150,150,150,255,panelX2-380,panelY2-20,panelX2-208,panelY2-10);
		font8x8.drawText("Pop: "+DataTools::toString(worldPop),panelX2-380,panelY2-20,panelX2-208,panelY2-10, false, true);

		guiManager.render();
	}

	void logicTick()
	{
	//textDate.text = world.date.toString("-");

	if(active)
	{
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
		// If time is progressing, pause it.
		if(_keyboard->isPressed(Keyboard::ESCAPE))
		{
			menuTribes.active = false;
			menuCivs.active = false;
			menuFlora.active = false;
			menuFlora.menuFloraDetails.active = false;
			menuCreatures.active = false;
			menuCreatures.menuCreatureDetails.active = false;
			menuSettlements.active = false;
			menuWorld.active = false;
			menuCharacter.active = false;
			menuBiome.active = false;
			menuBiome.menuBiomeDetails.active = false;
			minimap.active = true;


			//Clear the backlog so time stops progressing.
			world.ticksBacklog=0;
			buttonIncrementContinuous.text = "+++";
			world.incrementContinuous=false;

			_keyboard->keyUp(Keyboard::ESCAPE);
		}
		// Increment world by 1 tick by pressing period
		if(_keyboard->isPressed(Keyboard::PERIOD))
		{
			world.incrementTicks(1);

			_keyboard->keyUp(Keyboard::PERIOD);
		}
		

		// TAB will switch between adventure mode and god mode.
		if(_keyboard->isPressed(Keyboard::TAB))
		{
			if (playerCharacter == 0 )
			{
				std::cout<<"No character selected, selecting random character.\n";
				//Automatically select a random character so we can go straight into adventure mode.
				playerCharacter = world.getRandomCharacter();
			}

			if ( playerCharacter != 0 )
			{
				std::cout<<"Entering adventure mode.\n";

				if ( world.prepareAdventureMode(playerCharacter) )
				{
					activeMenu = MENU_ADVENTUREMODE;

					menuTribes.active = false;
					menuCivs.active = false;
					menuFlora.active = false;
					menuCreatures.active = false;
					menuSettlements.active = false;
					menuWorld.active = false;
					menuCharacter.active = false;
					menuBiome.active = false;
					minimap.active = false;
					//active = false;

					worldViewer.subterraneanMode=playerCharacter->isUnderground;
				}
				else
				{
					std::cout<<"Error: world.prepareAdventureMode(~) failed.\n";
				}

				_keyboard->keyUp(Keyboard::TAB);
			}
			else
			{
				std::cout<<"Error: No player pointer.\n";
			}
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
				buttonExpandMap.setPanel
				(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX1+(panelX2/2)+16,panelY1+(panelY2/2)+16);
			}

			buttonExpandMap.unclick();
			eventResize();
		}

		if (buttonIncrementDay.clicked==true)
		{
			buttonIncrementDay.unclick();
			world.incrementTicks(TICKS_PER_DAY);
		}
		if (buttonIncrementMonth.clicked==true)
		{
			buttonIncrementMonth.unclick();
			world.incrementTicks(TICKS_PER_MONTH);
		}
		if (buttonIncrementYear.clicked==true)
		{
			buttonIncrementYear.unclick();
			world.ticksBacklog+=TICKS_PER_YEAR;
		}
		if (buttonIncrementContinuous.clicked==true)
		{
			buttonIncrementContinuous.unclick();
			//world.ticksBacklog+=311040000;
			world.incrementContinuous=!world.incrementContinuous;
			
			if (world.incrementContinuous)
			{
				buttonIncrementContinuous.text = "---";
			}
			else
			{
				buttonIncrementContinuous.text = "+++";
			}
			
			std::cout<<"Incrementing continuously: "<<world.incrementContinuous<<".\n";
		}

		if (buttonTribeMenu.clicked==true)
		{
			menuTribes.init();
			menuTribes.active=true;
			buttonTribeMenu.unclick();
		}

		if (buttonWorldMenu.clicked==true)
		{
			std::cout<<"Worldmenu\n";
			menuWorld.init();
			menuWorld.active=true;
			buttonWorldMenu.unclick();
		}

		if (buttonCharacterMenu.clicked==true)
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
		
		if (buttonFlora.clicked==true)
		{
			std::cout<<"Flora menu\n";
			menuFlora.init();
			menuFlora.active=true;
			buttonFlora.unclick();
		}
		
		if (buttonCreaturesMenu.clicked==true)
		{
			std::cout<<"Creatures menu\n";
			menuCreatures.init();
			menuCreatures.active=true;
			buttonCreaturesMenu.unclick();
		}
		
		if (buttonSettlementsMenu.clicked==true)
		{
			std::cout<<"Settlments menu\n";
			menuSettlements.init();
			menuSettlements.active=true;
			buttonSettlementsMenu.unclick();
		}
		
		



		if (buttonBiomeMenu.clicked==true)
		{
			std::cout<<"Biome menu\n";
			menuBiome.init();
			menuBiome.active=true;
			buttonBiomeMenu.unclick();
		}

		if (buttonMinimapBaseTexture.clicked==true)
		{
			std::cout<<"Toggle texture/colour mode\n";
			worldViewer.tilesetMode = !worldViewer.tilesetMode;
			buttonMinimapBaseTexture.unclick();
		}
		if (buttonMinimapBaseLand.clicked==true)
		{
			std::cout<<"Toggle texture/colour mode\n";
			worldViewer.landMode = !worldViewer.landMode;
			buttonMinimapBaseLand.unclick();
		}
		if (buttonTerritoryView.clicked==true)
		{
			std::cout<<"Toggle territory visibility.\n";
			worldViewer.territoryView=!worldViewer.territoryView;
			buttonTerritoryView.unclick();
		}
		if (buttonSubterraneanView.clicked==true)
		{
			std::cout<<"Toggle subterranean mode\n";
			worldViewer.subterraneanMode = !worldViewer.subterraneanMode;
			buttonSubterraneanView.unclick();
		}
		
		//buttonMinimapBaseTexture.setPanel(panelX1,panelY1,panelX1+32,panelY1+16);
		//buttonMinimapBaseColour.setPanel(panelX1+32,panelY1,panelX1+64,panelY1+16);
		//buttonMinimapBaseLand.setPanel(panelX1+64,panelY1,panelX1+96,panelY1+16);

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

		// maybe some kind of EVENT class would work better for this kind of thing?
		if (menuWorld.lastRowClicked != -1 )
		{
			if ( world.vLandmass.isSafe(menuWorld.lastRowClicked) )
			{
				World_Landmass* l = menuWorld.selectedLandmass;
				if ( l != 0 )
				{
					worldViewer.setCenterTile(l->centerX,l->centerY);
				}
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

		buttonIncrementDay.setPanel(panelX2-120, panelY2-40, panelX2-160, panelY2-30);
		buttonIncrementMonth.setPanel(panelX2-80, panelY2-40, panelX2-120, panelY2-30);
		buttonIncrementYear.setPanel(panelX2-40, panelY2-40, panelX2-80, panelY2-30);
		buttonIncrementContinuous.setPanel(panelX2, panelY2-40, panelX2-40, panelY2-30);

		buttonTribeMenu.setPanel(panelX2-180, panelY2-40, panelX2-220, panelY2-30);
		buttonWorldMenu.setPanel(panelX2-220, panelY2-40, panelX2-260, panelY2-30);
		buttonCharacterMenu.setPanel(panelX2-260, panelY2-40, panelX2-300, panelY2-30);
		buttonBiomeMenu.setPanel(panelX2-300, panelY2-40, panelX2-340, panelY2-30);
		buttonTerritoryView.setPanel(panelX2-340, panelY2-40, panelX2-380, panelY2-30);
		buttonCivMenu.setPanel(panelX2-380, panelY2-40, panelX2-420, panelY2-30);
		buttonSubterraneanView.setPanel(panelX2-420, panelY2-40, panelX2-460, panelY2-30);
		
		buttonFlora.setPanel(panelX2-460, panelY2-40, panelX2-500, panelY2-30);
		buttonCreaturesMenu.setPanel(panelX2-500, panelY2-40, panelX2-540, panelY2-30);
		buttonSettlementsMenu.setPanel(panelX2-540, panelY2-40, panelX2-580, panelY2-30);
		
		buttonMinimapBaseTexture.setPanel(panelX1,panelY1,panelX1+32,panelY1+16);
		buttonMinimapBaseLand.setPanel(panelX1+32,panelY1,panelX1+64,panelY1+16);

		menuTribes.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuCivs.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuWorld.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuCharacter.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuBiome.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuFlora.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuCreatures.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		menuSettlements.setPanel(panelX1,panelY1+5,panelX2,panelY2-40);
		minimap.setPanel(panelX1,panelY1+16,panelX1+220,panelY1+236);

		if (fullScreenWorldView==true)
		{
			worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
			buttonExpandMap.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
		}
		else
		{
			worldViewer.setPanel(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX2,panelY2);
			buttonExpandMap.setPanel
			(panelX1+(panelX2/2),panelY1+(panelY2/2),panelX1+(panelX2/2)+16,panelY1+(panelY2/2)+16);
		}
	}

};

#endif
