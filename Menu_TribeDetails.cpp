#pragma once
#ifndef WORLDSIM_MENU_TRIBE_DETAILS_CPP
#define WORLDSIM_MENU_TRIBE_DETAILS_CPP

/* WorldSim: Menu_TribeDetails.cpp
	#include "Menu_TribeDetails.cpp"
	
*/

#include <GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_TribeDetails: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
	
	GUI_Button buttonClose;
	
		/* TextEntry: Filter names */
	GUI_TextEntry textFilterTable;
	
		// TABLE FOR TRIBES
	Table2 tCharacters;
	GUI_Table guiTableTribeCharacters;
	
	int lastRowClicked;
	Tribe* selectedTribe;
	
	Menu_TribeDetails()
	{	
		lastRowClicked=-1;
		selectedTribe=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
	
	void init()
	{
		lastRowClicked=-1;
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(&cNormal,&cHighlight,0);
		buttonClose.active=true;
		
		
		guiTableTribeCharacters.clear();
		guiTableTribeCharacters.table = &tCharacters;
		guiTableTribeCharacters.alpha=0;
		guiTableTribeCharacters.active=true;
		guiTableTribeCharacters.addColumn("First name","firstname",120);
		guiTableTribeCharacters.addColumn("Last name","lastname",120);
		//guiTableTribeCharacters.addColumn("Food","food",120);
		//guiTableTribeCharacters.addColumn("Population","population",120);
		//guiTableTribeCharacters.addColumn("Coordinates","coordinates",120);
		//guiTableTribeCharacters.addColumn("Territory","territory",120);
		
		tCharacters.clear();
		
		if ( selectedTribe != 0 )
		{
			for (int i=0;i<selectedTribe->vCharacter.size();++i)
			{
				//std::cout<<"Adding: "<<selectedTribe->vCharacter(i)->getFullName()<<".\n";
				tCharacters.addRow(selectedTribe->vCharacter(i));
			}
		}

		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&guiTableTribeCharacters);
		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("TRIBE DETAILS MENU",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			std::string tribeName = "Tribe name: " + selectedTribe->name + ".";
			font8x8.drawText(tribeName,panelX1,panelY2-35,panelX2,panelY2-20, false, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
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
				std::cout<<"CLOSE\n";
				active=false;
				buttonClose.unclick();
			}
		
			// if ( guiTableTribeCharacters.lastClickedIndex != -1 )
			// {
				// //world->worldV
				// //TableInterface
				// //worldViewer.setCenterTile();
				// lastRowClicked=guiTableTribeCharacters.lastClickedIndex;
				// if (world.vTribe.isSafe(lastRowClicked))
				// {
					// selectedTribe=world.vTribe(lastRowClicked);
				// }
				// guiTableTribeCharacters.lastClickedIndex = -1;
			// }
		
		}
		
		return false;
	}
	
	void eventResize()
	{
		
		guiTableTribeCharacters.setPanel(panelX1,panelY2-50,panelX2,panelY1);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}
	
};


#endif