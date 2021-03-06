#pragma once
#ifndef WORLDSIM_MENU_WORLD_HPP
#define WORLDSIM_MENU_WORLD_HPP

/* WorldSim: Menu_World.hpp

	This menu shows a database of all tribes in the world. The player can click a tribe to get a detailed view of the tribe.
*/

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
		guiManager.add(&guiTableLandmass);

		eventResize();
	}

	void render()
	{
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("World Info",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
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
		if ( active )
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
	}
	
};


#endif