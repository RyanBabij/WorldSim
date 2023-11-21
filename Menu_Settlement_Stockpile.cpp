#pragma once
#ifndef WORLDSIM_MENU_SETTLEMENT_STOCKPILE_CPP
#define WORLDSIM_MENU_SETTLEMENT_STOCKPILE_CPP

/* WorldSim: Menu_Settlement_Stockpile.cpp
	#include "Menu_Settlement_Stockpile.cpp"

	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_Settlement_Stockpile: public GUI_Interface
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

	Settlement* selectedSettlement;

		// TABLE FOR ITEMS
	Table2 tSettlements;
	GUI_Table guiTableSettlements;
	
	Menu_Settlement_Stockpile()
	{	
		selectedSettlement=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
  

	// This is some bad overloading.
	void init()
	{
		init(0);
	}
	
	void init(Settlement* _selectedSettlement)
	{
		if ( _selectedSettlement == 0 )
		{
			return;
		}
		
		if ( _selectedSettlement != 0 || selectedSettlement== 0)
		{
			selectedSettlement=_selectedSettlement;
		}
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;

		guiManager.clear();
		
		guiTableSettlements.clear();
		guiTableSettlements.table = &tSettlements;
		guiTableSettlements.alpha=0;
		guiTableSettlements.active=true;
		guiTableSettlements.addColumn("Name","name",240);
		guiTableSettlements.addColumn("Type","type",120);
		guiTableSettlements.addColumn("Quality","quality",120);
		//guiTableSettlements.addColumn("Civilization","civilization",120);
		//guiTableSettlements.addColumn("Coordinates","coordinates",120);
		//guiTableSettlements.addColumn("Territory","territory",120);

		tSettlements.clear();
		
		
		for (int i=0;i<selectedSettlement->stockpile.vItem.size();++i)
		{
			tSettlements.addRow(selectedSettlement->stockpile.vItem(i));
		}

		guiManager.add(&buttonClose);
		guiManager.add(&guiTableSettlements);
		
		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		if (selectedSettlement==0)
		{
			active=false;
			return;
		}
		
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Settlement stockpile",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if (selectedSettlement==0) { return false; }
		
		if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if (selectedSettlement==0) { return false; }
		
		if ( active )
		{
				/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
				
			}

			if (buttonClose.clicked==true)
			{
				active=false;
				buttonClose.unclick();
				return true;
			}
		}
		
		return false;
	}
	
	void eventResize()
	{
		guiTableSettlements.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}
	
};


#endif // WORLDSIM_MENU_SETTLEMENT_STOCKPILE_CPP
