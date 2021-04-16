#pragma once
#ifndef WORLDSIM_MENU_CIVDETAILS_HPP
#define WORLDSIM_MENU_CIVDETAILS_HPP

/* WorldSim: Menu_CivDetails.cpp
  #include "Menu_CivDetails.hpp"

	This menu shows details of a specific civ.
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_CivDetails: public GUI_Interface
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
	GUI_Button buttonCivDetails;
	
		// TABLE FOR CIVS
	Table2 tSettlements;
	GUI_Table guiTableCivs;
	
	int lastRowClicked;
	Civ* selectedCiv;
	
	//Menu_TribeDetails menuTribeDetails;
	
	Menu_CivDetails()
	{	
		lastRowClicked=-1;
		selectedCiv=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		//menuTribeDetails.setFont(_font);
	}
  
  
    // This is some bad overloading.
  void init()
  {
    init(0);
  }
	
	void init(Civ* _selectedCiv)
	{
    selectedCiv = _selectedCiv;
    
		lastRowClicked=-1;
		//selectedCiv=0;
    active = false;
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;
		
		buttonCivDetails.text="Details";
		buttonCivDetails.setColours(cNormal,cHighlight,0);
		buttonCivDetails.active=true;
		
		guiTableCivs.clear();
		guiTableCivs.table = &tSettlements;
		guiTableCivs.alpha=0;
		guiTableCivs.active=true;
		guiTableCivs.addColumn("Name","name",180);
		guiTableCivs.addColumn("Race","race",120);
		guiTableCivs.addColumn("Population","population",120);
		guiTableCivs.addColumn("Metal","metal",120);
		//guiTableCivs.addColumn("Coordinates","coordinates",120);
		//guiTableCivs.addColumn("Territory","territory",120);
		
		tSettlements.clear();
    if ( selectedCiv != 0 )
    {
      for (int i=0;i<selectedCiv->vSettlement.size();++i)
      {
        tSettlements.addRow(selectedCiv->vSettlement(i));
      }
		}
		
		guiManager.add(&buttonClose);
		guiManager.add(&buttonCivDetails);
		guiManager.add(&guiTableCivs);
		
		//menuCivDetails.init();
		//menuCivDetails.active=false;
		
		eventResize();
		//menuCivDetails.eventResize();
	}
	
	void render()
	{
		// if ( menuCivDetails.active )
		// {
			// menuCivDetails.render();
		// }
		//else if ( active )
    if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("CIV DETAILS MENU",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		// if ( menuCivDetails.active )
		// {
			// return menuCivDetails.keyboardEvent(_keyboard);
		// }
    if ( active )
		//else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		
		
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		// if ( menuCivDetails.active )
		// {
			// menuCivDetails.mouseEvent(_mouse);
		// }
		//else if ( active )
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
			
			if  (buttonCivDetails.clicked==true)
			{
				if ( selectedCiv != 0 )
				{
					std::cout<<"Civ details\n";
					//active=false;
            //menuCivDetails.selectedTribe=selectedTribe;
            //menuCivDetails.init();
            //menuCivDetails.active=true;
					//guiTableTribes.active=false;
				}
				else
				{
					std::cout<<"Select a tribe first.\n";
				}

				buttonCivDetails.unclick();
			}
		
		
		
			if ( guiTableCivs.lastClickedIndex != -1 )
			{
				//world->worldV
				//TableInterface
				//worldViewer.setCenterTile();
				lastRowClicked=guiTableCivs.lastClickedIndex;
				if (world.vCiv.isSafe(lastRowClicked))
				{
					selectedCiv=world.vCiv(lastRowClicked);
				}
				guiTableCivs.lastClickedIndex = -1;
			}
		
		}
		
		return false;
	}
	
	void eventResize()
	{
	
		guiTableCivs.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonCivDetails.setPanel(panelX2-100, panelY1+40, panelX2-20, panelY1+20);
		//menuTribeDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		//menuTribeDetails.eventResize();
	}
	
};


#endif