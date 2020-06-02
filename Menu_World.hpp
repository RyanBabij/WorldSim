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
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
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
		buttonClose.setColours(&cNormal,&cHighlight,0);
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
			
			//font8x8.drawText("This menu will display interesting info about the world. For example, did you know that the world has "+DataTools::toString(world.vTribe.size())+" tribes? Fascinating stuff.\n",panelX1,panelY1,panelX2,panelY2-25, false, false);
			
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
	
		guiTableLandmass.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}
	
};


#endif