#pragma once
#ifndef WORLDSIM_MENU_ITEMS_CPP
#define WORLDSIM_MENU_ITEMS_CPP

/* WorldSim: Menu_Items.cpp
	#include "Menu_Items.cpp"

	Lists notable items in the world.
	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_Items: public GUI_Interface
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

	GUI_Button buttonItemDetails;
	
	Item* selectedItem;
	
	// Table of items
	Table2 tItems;
	GUI_Table guiTableItems;
	
	Menu_Items()
	{	
		selectedItem=0;
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
	
	void init(Item* _item)
	{
		// init and populate table
		guiTableItems.clear();
		guiTableItems.table = &tItems;
		guiTableItems.alpha=0;
		guiTableItems.active=true;
		guiTableItems.addColumn("Date","date",120);
		guiTableItems.addColumn("Quality","quality",120);
		guiTableItems.addColumn("Type","name",120);
		guiTableItems.addColumn("Description","description",240);
		tItems.clear();
		
		for (int i=0;i<world.vCiv.size();++i)
		{
			for (int j=0;j<world.vCiv(i)->vSettlement.size();++j)
			{
				Settlement * settlement = world.vCiv(i)->vSettlement(j);
				
				for (int k=0;k<settlement->vItem.size();++k)
				{
					tItems.addRow(settlement->vItem(k));
				}
			}
		}
		

		if ( _item != 0 || selectedItem== 0)
		{
			selectedItem=_item;
		}
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;

		buttonItemDetails.text="Item Details";
		buttonItemDetails.setColours(cNormal,cHighlight,0);
		buttonItemDetails.active=true;
		
		guiManager.add(&buttonClose);
		guiManager.add(&buttonItemDetails);
		guiManager.add(&guiTableItems);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
		//if (selectedItem==0) { return; }
		
		if ( active )
		{
			font8x8.drawText("Items",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
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

			if (buttonClose.clicked==true)
			{
				active=false;
				buttonClose.unclick();
			}
      
			if (buttonItemDetails.clicked==true)
			{
				std::cout<<"Put item details menu here\n";
				active=false;
				buttonItemDetails.unclick();
			}
		}
		
		return false;
	}
	
	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonItemDetails.setPanel(panelX2-140, panelY1+40, panelX2-20, panelY1+20);
		guiTableItems.setPanel(panelX1,panelY1,panelX2,panelY2-120);
	}
	
};


#endif