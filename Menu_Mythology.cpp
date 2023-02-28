#pragma once
#ifndef WORLDSIM_MENU_MYTHOLOGY_CPP
#define WORLDSIM_MENU_MYTHOLOGY_CPP

/* WorldSim: Menu_Mythology.cpp
	#include "Menu_Mythology.cpp"

	Lists notable myuthologies in the world.
	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include "Mythology.hpp"

class Menu_Mythology: public GUI_Interface
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

	GUI_Button buttonMythologyDetails;
	
	Item* selectedMythology;
	
	// Table of items
	Table2 tItems;
	GUI_Table guiTableMythologies;
	
	Menu_Mythology()
	{	
		selectedMythology=0;
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
	
	void init(Mythology* _selectedMythology)
	{
		std::cout<<"\n**********MYTH INIT*****\n";
		// init and populate table
		guiTableMythologies.clear();
		guiTableMythologies.table = &tItems;
		guiTableMythologies.alpha=0;
		guiTableMythologies.active=true;
		guiTableMythologies.addColumn("AYAYA","creation",120);
		guiTableMythologies.addColumn("Quality","quality",120);
		guiTableMythologies.addColumn("Type","name",120);
		guiTableMythologies.addColumn("Description","description",240);
		tItems.clear();
		
		// for (int i=0;i<world.vCiv.size();++i)
		// {
			// for (int j=0;j<world.vCiv(i)->vSettlement.size();++j)
			// {
				// Settlement * settlement = world.vCiv(i)->vSettlement(j);
				
				// for (int k=0;k<settlement->vItem.size();++k)
				// {
					// tItems.addRow(settlement->vItem(k));
				// }
			// }
		// }
		

		// if ( _selectedMythology != 0 || selectedMythology== 0)
		// {
			// selectedMythology=_selectedMythology;
		// }
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;

		buttonMythologyDetails.text="Mythology Details";
		buttonMythologyDetails.setColours(cNormal,cHighlight,0);
		buttonMythologyDetails.active=true;
		
		guiManager.add(&buttonClose);
		guiManager.add(&buttonMythologyDetails);
		guiManager.add(&guiTableMythologies);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
		//if (selectedMythology==0) { return; }
		
		if ( active )
		{
			font8x8.drawText("Mythologies",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
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
      
			if (buttonMythologyDetails.clicked==true)
			{
				std::cout<<"Put mythology details menu here\n";
				active=false;
				buttonMythologyDetails.unclick();
			}
		}
		
		return false;
	}
	
	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonMythologyDetails.setPanel(panelX2-140, panelY1+40, panelX2-20, panelY1+20);
		guiTableMythologies.setPanel(panelX1,panelY1,panelX2,panelY2-120);
	}
	
};


#endif