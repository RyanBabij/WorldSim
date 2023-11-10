#pragma once
#ifndef WORLDSIM_MENU_MYTHOLOGY_CPP
#define WORLDSIM_MENU_MYTHOLOGY_CPP

/* WorldSim: Menu_Mythology.cpp
	#include "Menu_Mythology.cpp"

	Lists notable myuthologies in the world.
	
*/

#include "Mythology.hpp"
#include "Menu_MythologyDetails.cpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>


class Menu_Mythology: public GUI_Interface
{
	Menu_MythologyDetails menuMythologyDetails;
	
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
	
	Mythology* selectedMythology;
	
	// Table of mythologies
	Table2 tMythologies;
	GUI_Table guiTableMythologies;
	
	int lastRowClicked;
	
	Menu_Mythology()
	{	
		selectedMythology=0;
		lastRowClicked=-1;
	}

	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuMythologyDetails.setFont(_font);
	}
  
  
	// This is some bad overloading.
	void init()
	{
		init(0);
	}
	
	void init(Mythology* _selectedMythology)
	{
		// init and populate table
		guiTableMythologies.clear();
		guiTableMythologies.table = &tMythologies;
		guiTableMythologies.alpha=0;
		guiTableMythologies.active=true;
		guiTableMythologies.addColumn("Date","date",100);
		guiTableMythologies.addColumn("Type","type",160);
		guiTableMythologies.addColumn("Description","description",480);
		tMythologies.clear();
		
		for (int i=0;i<world.mythologyManager.size();++i)
		{
			tMythologies.addRow(world.mythologyManager.get(i));
		}


		if ( _selectedMythology != 0 || selectedMythology== 0)
		{
			selectedMythology=_selectedMythology;
		}
		
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
		
		guiManager.clear();

		guiManager.add(&buttonClose);
		guiManager.add(&buttonMythologyDetails);
		guiManager.add(&guiTableMythologies);

		guiManager.setFont(font);
		
		menuMythologyDetails.init();
		menuMythologyDetails.active=false;
	
		eventResize();
		menuMythologyDetails.eventResize();
	}
	
	void render()
	{
		if ( menuMythologyDetails.active )
		{
			menuMythologyDetails.render();
		}
		else if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Mythologies",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}
	
	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuMythologyDetails.active )
		{
			return menuMythologyDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuMythologyDetails.active )
		{
			menuMythologyDetails.mouseEvent(_mouse);
		}
		else if ( active )
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
				if ( selectedMythology != 0 )
				{
					std::cout<<"Mythology details\n";
					menuMythologyDetails.init(selectedMythology);
					menuMythologyDetails.active=true;
					// Keep the parent menu active so we go back to it when closing submenu
					//active=false;
				}
				else
				{
					std::cout<<"Select a mythology first.\n";
				}

				buttonMythologyDetails.unclick();
			}
			
			if ( guiTableMythologies.lastClickedIndex != -1 )
			{
				lastRowClicked=guiTableMythologies.lastClickedIndex;

				int totalIndex = 0;
				for (int i=0;i<world.mythologyManager.vMythology.size();++i)
				{
					if ( lastRowClicked == i )
					{
						selectedMythology = world.mythologyManager.vMythology(i);
						
						std::cout<<"Mythology selected: "<<selectedMythology->name<<"\n";
					}

				}
		
				guiTableMythologies.lastClickedIndex = -1;
			}
			
			
			
			
		}
		
		return false;
	}
	
	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonMythologyDetails.setPanel(panelX2-140, panelY1+40, panelX2-20, panelY1+20);
		guiTableMythologies.setPanel(panelX1,panelY1,panelX2,panelY2-120);
		menuMythologyDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuMythologyDetails.eventResize();
	}
	
};


#endif