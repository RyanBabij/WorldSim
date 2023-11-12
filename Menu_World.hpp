#pragma once
#ifndef WORLDSIM_MENU_WORLD_HPP
#define WORLDSIM_MENU_WORLD_HPP

/* WorldSim: Menu_World.hpp

	This menu shows a database of all tribes in the world. The player can click a tribe to get a detailed view of the tribe.
	
	This will be changed to legends. Could also be called annals.
*/

#include "Menu_CharacterDetails.cpp"
#include "Menu_Events.cpp"
#include "Menu_Items.cpp"
#include "Menu_Mythology.cpp"
#include "Menu_Races.cpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include <Data/DataTools.hpp>

class Menu_World: public GUI_Interface
{
	public:
	GUI_Manager guiManager;

	/* Colours / theme. */
	ColourRGB <unsigned char> cNormal;
	ColourRGB <unsigned char> cSelected;
	ColourRGB <unsigned char> cDropPanel;
	ColourRGB <unsigned char> cHighlight;

	Wildcat::Font* font;

	/* Menu for investigating an individual tribe */
	GUI_Button buttonClose;

	// View notable events
	GUI_Button buttonEvents;
	// View notable characters living and deceased.
	GUI_Button buttonCharacters;
	// View notable Tribes
	// View notable Civilizations
	GUI_Button buttonCivilizations;
	// View notable Cities
	// View notable Artifacts
	GUI_Button buttonItems;
	// View notable locations
	// View Mythologies
	GUI_Button buttonMythology;
	// View races
	GUI_Button buttonRaces;

	//Submenu for events
	Menu_Events menuEvents;
	//Submenu for items
	Menu_Items menuItems;
	//Submenu for mythologies
	Menu_Mythology menuMythologies;
	//Submenu for races
	Menu_Mythology menuRaces;

	// TABLE FOR LANDMASSES
	Table2 tLandmass;
	GUI_Table guiTableLandmass;

	World_Landmass* selectedLandmass;
	int lastRowClicked;
	
	
	Menu_World()
	{	
		selectedLandmass=0;
		lastRowClicked=-1;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuEvents.setFont(_font);
		menuItems.setFont(_font);
		menuMythologies.setFont(_font);
		menuRaces.setFont(_font);
	}
	
	void init()
	{
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;
		
		buttonEvents.text="Events";
		buttonEvents.setColours(cNormal,cHighlight,0);
		buttonEvents.active=true;
		
		buttonItems.text="Items";
		buttonItems.setColours(cNormal,cHighlight,0);
		buttonItems.active=true;
		
		buttonCivilizations.text="Civilizations";
		buttonCivilizations.setColours(cNormal,cHighlight,0);
		buttonCivilizations.active=true;
		
		buttonMythology.text="Mythologies";
		buttonMythology.setColours(cNormal,cHighlight,0);
		buttonMythology.active=true;
		
		buttonRaces.text="Races";
		buttonRaces.setColours(cNormal,cHighlight,0);
		buttonRaces.active=true;

		active = false;


		guiTableLandmass.clear();
		guiTableLandmass.table = &tLandmass;
		guiTableLandmass.alpha=0;
		guiTableLandmass.active=true;
		guiTableLandmass.addColumn("Name","name",120);
		guiTableLandmass.addColumn("Size","size",80);

		tLandmass.clear();
		for (int i=0;i<world.vLandmass.size();++i)
		{
			tLandmass.addRow(world.vLandmass(i));
		}

		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonEvents);
		guiManager.add(&buttonItems);
		guiManager.add(&buttonCivilizations);
		guiManager.add(&buttonMythology);
		guiManager.add(&buttonRaces);
		//guiManager.add(&guiTableLandmass);
		
		menuEvents.init();
		menuEvents.active=false;
		menuItems.init();
		menuItems.active=false;
		menuMythologies.init();
		menuMythologies.active=false;
		menuRaces.init();
		menuRaces.active=false;
		
		eventResize();
		menuEvents.eventResize();
		menuItems.eventResize();
		menuMythologies.eventResize();
		menuRaces.eventResize();
	}

	void render()
	{
		if ( menuEvents.active )
		{
			menuEvents.render();
		}
		else if ( menuItems.active )
		{
			menuItems.render();
		}
		else if ( menuMythologies.active )
		{
			menuMythologies.render();
		}
		else if ( menuRaces.active )
		{
			menuRaces.render();
		}
		else if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Legends of "+world.name,panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			std::string strWorldInfo = "The planet "+world.name+" is in the "+world.astronomy.sunName+" system, with "+
			DataTools::toString(world.astronomy.vPlanet.size())+" planets. ";
			
			if ( world.astronomy.vMoon.size() == 0 )
			{
				strWorldInfo += "It has no moons.";
			}
			else if (world.astronomy.vMoon.size() == 1 )
			{
				strWorldInfo += "It has 1 moon called "+world.astronomy.vMoon(0)+".";
			}
			else
			{
				strWorldInfo += "It has " + DataTools::toString(world.astronomy.vMoon.size()) +" moons called ";
				
				
				for (int i=0;i<world.astronomy.vMoon.size()-1;++i)
				{
					strWorldInfo+=world.astronomy.vMoon(i)+", ";
				} strWorldInfo+="and "+world.astronomy.vMoon(world.astronomy.vMoon.size()-1)+".";
				
			}
			
			font8x8.drawText(strWorldInfo, panelX1+2,panelY1,panelX2-2,panelY2-25, false, false);
			
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuEvents.active )
		{
			menuEvents.mouseEvent(_mouse);
		}
		else if ( menuItems.active )
		{
			menuItems.mouseEvent(_mouse);
		}
		else if ( menuMythologies.active )
		{
			menuMythologies.mouseEvent(_mouse);
		}
		else if ( menuRaces.active )
		{
			menuRaces.mouseEvent(_mouse);
		}
		
		else if ( active )
		{
				/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
				
			}

			if  (buttonClose.clicked==true)
			{
				active=false;
				buttonClose.unclick();
			}
			if  (buttonEvents.clicked==true)
			{
				std::cout<<"Events\n";
				active=false;
				buttonEvents.unclick();
				
				//menuEvents here
				menuEvents.init(0);
				menuEvents.active=true;
			}
			if  (buttonItems.clicked==true)
			{
				std::cout<<"Items\n";
				active=false;
				buttonItems.unclick();
				
				//menuEvents here
				menuItems.init(0);
				menuItems.active=true;
			}
			if  (buttonCivilizations.clicked==true)
			{
				std::cout<<"Civilizations\n";
				active=false;
				buttonCivilizations.unclick();
				
				//menuEvents here
				//menuItems.init(0);
				//menuItems.active=true;
			}
			if  (buttonMythology.clicked==true)
			{
				std::cout<<"Mythology\n";
				active=false;
				buttonMythology.unclick();
				menuMythologies.init(0);
				menuMythologies.active=true;
			}
			if  (buttonRaces.clicked==true)
			{
				std::cout<<"Races\n";
				active=false;
				buttonRaces.unclick();
				menuRaces.init(0);
				buttonRaces.active=true;
			}
			
			
			if ( guiTableLandmass.lastClickedIndex != -1 )
			{
				//world->worldV
				//TableInterface
				//worldViewer.setCenterTile();
				lastRowClicked=guiTableLandmass.lastClickedIndex;
				if (world.vLandmass.isSafe(lastRowClicked))
				{
					selectedLandmass=world.vLandmass(lastRowClicked);
				}
				guiTableLandmass.lastClickedIndex = -1;
			}
			
		}
		return false;
	}
	
	void eventResize()
	{
		guiTableLandmass.setPanel(panelX1,panelY1,panelX2,panelY2-80);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);

		int midX = panelNX/2+panelX1;

		buttonEvents.setPanel(midX-80,panelY2-100,midX+80,panelY2-120);
		buttonItems.setPanel(midX-80,panelY2-120,midX+80,panelY2-140);
		buttonCivilizations.setPanel(midX-80,panelY2-140,midX+80,panelY2-160);
		buttonMythology.setPanel(midX-80,panelY2-160,midX+80,panelY2-180);
		buttonRaces.setPanel(midX-80,panelY2-180,midX+80,panelY2-200);

		menuEvents.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuEvents.eventResize();

		menuItems.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuItems.eventResize();
		
		menuMythologies.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuMythologies.eventResize();		
		
		menuRaces.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuRaces.eventResize();
		
	}

};


#endif