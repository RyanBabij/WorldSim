#pragma once
#ifndef WORLDSIM_MENU_SETTLEMENTS_HPP
#define WORLDSIM_MENU_SETTLEMENTS_HPP

/* WorldSim: Menu_Settlements.hpp

	This menu shows a database of all settlements in the world.
	The player can click a civ to get more detailed information of that particular settlement.
*/

#include "Menu_SettlementDetails.cpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>


class Menu_Settlements: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	ColourRGB <unsigned char> cNormal;
	ColourRGB <unsigned char> cSelected;
	ColourRGB <unsigned char> cDropPanel;
	ColourRGB <unsigned char> cHighlight;
	
	Wildcat::Font* font;
  
	GUI_Button buttonClose;
	
		/* Menu for investigating an individual civ */
	GUI_Button buttonSettlementDetails;
	
		// TABLE FOR CIVS
	Table2 tSettlements;
	GUI_Table guiTableSettlements;
	
	int lastRowClicked;
	Settlement* selectedSettlement;
	
	Menu_SettlementDetails menuSettlementDetails;
	
	Menu_Settlements()
	{	
		lastRowClicked=-1;
		selectedSettlement=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuSettlementDetails.setFont(_font);
	}
	
	void init()
	{
		lastRowClicked=-1;
		selectedSettlement=0;
		active = false;

		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;

		buttonSettlementDetails.text="Details";
		buttonSettlementDetails.setColours(cNormal,cHighlight,0);
		buttonSettlementDetails.active=true;
		buttonSettlementDetails.clicked=false;
		
		guiTableSettlements.clear();
		guiTableSettlements.table = &tSettlements;
		guiTableSettlements.alpha=0;
		guiTableSettlements.active=true;
		guiTableSettlements.addColumn("Name","name",240);
		guiTableSettlements.addColumn("Race","race",120);
		guiTableSettlements.addColumn("Population","population",120);
		guiTableSettlements.addColumn("Civilization","civilization",120);
		//guiTableSettlements.addColumn("Coordinates","coordinates",120);
		//guiTableSettlements.addColumn("Territory","territory",120);

		tSettlements.clear();
		
		
		for (int i=0;i<world.vCiv.size();++i)
		{
			for (int j=0;j<world.vCiv(i)->vSettlement.size();++j)
			{
				tSettlements.addRow(world.vCiv(i)->vSettlement(j));
			}
		}


		guiManager.add(&buttonClose);
		guiManager.add(&buttonSettlementDetails);
		guiManager.add(&guiTableSettlements);

		menuSettlementDetails.init();
		menuSettlementDetails.active=false;

		eventResize();
		menuSettlementDetails.eventResize();
	}
	
	void render()
	{
		int nSettlements = 0;
		
		for (int i=0;i<world.vCiv.size();++i)
		{
			for (int j=0;j<world.vCiv(i)->vSettlement.size();++j)
			{
				++nSettlements;
				break;
			}
		}
		
		if (nSettlements==0)
		{
			active=false;
			return;
		}
		
		
		if ( menuSettlementDetails.active )
		{
			menuSettlementDetails.render();
		}
		else if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("SETTLEMENT LIST MENU",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuSettlementDetails.active )
		{
			return menuSettlementDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		
		
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuSettlementDetails.active )
		{
			menuSettlementDetails.mouseEvent(_mouse);
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
			
			if  (buttonSettlementDetails.clicked==true)
			{
				if ( selectedSettlement != 0 )
				{
					std::cout<<"Settlement details\n";
					active=false;
					menuSettlementDetails.selectedSettlement=selectedSettlement;
					menuSettlementDetails.init(selectedSettlement);
					menuSettlementDetails.active=true;
					guiTableSettlements.active=false;
				}
				else
				{
					std::cout<<"Select a civ first.\n";
				}

				buttonSettlementDetails.unclick();
			}
		
		
		
			if (guiTableSettlements.lastClickedIndex != -1)
			{
				lastRowClicked = guiTableSettlements.lastClickedIndex;

				int index = 0;
				bool found = false; // Added flag to indicate when the settlement is found

				for (int i = 0; i < world.vCiv.size() && !found; ++i)
				{
					for (int j = 0; j < world.vCiv(i)->vSettlement.size() && !found; ++j)
					{
						if (index == lastRowClicked)
						{
							selectedSettlement = world.vCiv(i)->vSettlement(j);
							worldViewer.setCenterTile(selectedSettlement->worldX, selectedSettlement->worldY);
							found = true; // Set the flag to true when the settlement is found
							break; // Break out of the inner loop
						}
						++index;
					}
					// No need for an explicit break here; the condition in the for-loop handles it
				}
				guiTableSettlements.lastClickedIndex = -1;
			}

		
		}
		
		return false;
	}
	
	void eventResize()
	{
	
		guiTableSettlements.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonSettlementDetails.setPanel(panelX2-100, panelY1+40, panelX2-20, panelY1+20);
		menuSettlementDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuSettlementDetails.eventResize();
	}
	
};


#endif