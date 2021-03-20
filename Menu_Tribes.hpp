#pragma once
#ifndef WORLDSIM_MENU_TRIBES_HPP
#define WORLDSIM_MENU_TRIBES_HPP

/* WorldSim: Menu_Tribes.cpp

	This menu shows a database of all tribes in the world. The player can click a tribe to get a detailed view of the tribe.
*/

#include "Menu_TribeDetails.cpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_Tribes: public GUI_Interface
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
	
		/* Menu for investigating an individual tribe */
	GUI_Button buttonTribeDetails;
	
		// TABLE FOR TRIBES
	Table2 tTribes;
	GUI_Table guiTableTribes;
	
	int lastRowClicked;
	Tribe* selectedTribe;
	
	Menu_TribeDetails menuTribeDetails;
	
	Menu_Tribes()
	{	
		lastRowClicked=-1;
		selectedTribe=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuTribeDetails.setFont(_font);
	}
	
	void init()
	{
		lastRowClicked=-1;
		selectedTribe=0;
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(&cNormal,&cHighlight,0);
		buttonClose.active=true;
		
		buttonTribeDetails.text="Details";
		buttonTribeDetails.setColours(&cNormal,&cHighlight,0);
		buttonTribeDetails.active=true;
		
		guiTableTribes.clear();
		guiTableTribes.table = &tTribes;
		guiTableTribes.alpha=0;
		guiTableTribes.active=true;
		guiTableTribes.addColumn("Name","name",120);
		guiTableTribes.addColumn("Race","race",120);
		guiTableTribes.addColumn("Food","food",120);
		guiTableTribes.addColumn("Population","population",120);
		guiTableTribes.addColumn("Coordinates","coordinates",120);
		guiTableTribes.addColumn("Territory","territory",120);
		
		tTribes.clear();
		for (int i=0;i<world.vTribe.size();++i)
		{
			tTribes.addRow(world.vTribe(i));
		}
		
		
		guiManager.add(&buttonClose);
		guiManager.add(&buttonTribeDetails);
		guiManager.add(&guiTableTribes);
		
		menuTribeDetails.init();
		menuTribeDetails.active=false;
		
		eventResize();
		menuTribeDetails.eventResize();
	}
	
	void render()
	{
		if ( menuTribeDetails.active )
		{
			menuTribeDetails.render();
		}
		else if ( active )
		{

			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("TRIBE LIST MENU",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuTribeDetails.active )
		{
			return menuTribeDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		
		
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuTribeDetails.active )
		{
			menuTribeDetails.mouseEvent(_mouse);
		}
		else if ( active )
		{
				/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
				
			}

			if  (buttonClose.clicked==true)
			{
				//std::cout<<"CLOSE\n";
				active=false;
				buttonClose.unclick();
			}
			
			if  (buttonTribeDetails.clicked==true)
			{
				if ( selectedTribe != 0 )
				{
					//std::cout<<"Tribe details\n";
					//active=false;
					menuTribeDetails.selectedTribe=selectedTribe;
					menuTribeDetails.init();
					menuTribeDetails.active=true;
					//guiTableTribes.active=false;
				}
				else
				{
					std::cout<<"Select a tribe first.\n";
				}

				buttonTribeDetails.unclick();
			}
		
		
		
			if ( guiTableTribes.lastClickedIndex != -1 )
			{
				//world->worldV
				//TableInterface
				//worldViewer.setCenterTile();
				lastRowClicked=guiTableTribes.lastClickedIndex;
				if (world.vTribe.isSafe(lastRowClicked))
				{
					selectedTribe=world.vTribe(lastRowClicked);
				}
				guiTableTribes.lastClickedIndex = -1;
			}
		
		}
		
		return false;
	}
	
	void eventResize()
	{
	
		guiTableTribes.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonTribeDetails.setPanel(panelX2-100, panelY1+40, panelX2-20, panelY1+20);
		menuTribeDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuTribeDetails.eventResize();
	}
	
};


#endif