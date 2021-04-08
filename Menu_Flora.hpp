#pragma once
#ifndef WORLDSIM_MENU_FLORA_HPP
#define WORLDSIM_MENU_FLORA_HPP

/* WorldSim: Menu_Flora.hpp
	#include "Menu_Flora.hpp"

	Table of all Flora in World, with option to view submenu for a specific Flora.
*/

#include "Menu_FloraDetails.hpp" //Flora Details Submenu

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>
#include <Data/DataTools.hpp>



class Menu_Flora: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
	
		/*  Button to close the menu */
	GUI_Button buttonClose;
	
		/* Button to open Flora Details menu */
	GUI_Button buttonFloraDetails;
	
		// TABLE FOR BIOMES
		// why are there 2 tables here?
	Table2 tFlora;
	GUI_Table guiTableFlora;
	
	Menu_FloraDetails menuFloraDetails;
	
	int lastRowClicked;
	Flora * selectedFlora; // The Flora the user has selected in the menu.
	
	Menu_Flora()
	{
		lastRowClicked=-1;
		selectedFlora=0;
		menuFloraDetails.active=false;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuFloraDetails.setFont(_font);
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
		
		buttonFloraDetails.text="Flora Details";
		buttonFloraDetails.setColours(&cNormal,&cHighlight,0);
		buttonFloraDetails.active=true;
		
		active = false;
		
		guiTableFlora.clear();
		guiTableFlora.table = &tFlora;
		guiTableFlora.alpha=0;
		guiTableFlora.active=true;
		guiTableFlora.addColumn("Name","name",300);

		tFlora.clear();
		for (int i=0;i<world.vBiome.size();++i)
		{
			World_Biome * biome = world.vBiome(i);
			Vector <Flora*> * vFloraTypes = biome->getAllFloraTypes();
			
			if ( vFloraTypes == 0 )
			{
				// error null vector ptr
				return;
			}
			
			for (int j=0;j<vFloraTypes->size();++j)
			{
				tFlora.addRow( (*vFloraTypes)(j) );
			}
		}
		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonFloraDetails);
		guiManager.add(&guiTableFlora);
	
		eventResize();
	}
	
	void render()
	{
		if (menuFloraDetails.active)
		{
			menuFloraDetails.render();
		}
		else if (active)
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Flora Info",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if (menuFloraDetails.active)
		{
			menuFloraDetails.mouseEvent(_mouse);
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
			if  (buttonFloraDetails.clicked==true)
			{
				std::cout<<"Clicked flora details.\n";
				if ( selectedFlora != 0 )
				{
					std::cout<<"Flora details\n";
					menuFloraDetails.init(selectedFlora);
					menuFloraDetails.active=true;
				}
				else
				{
					std::cout<<"Select a Flora first.\n";
				}

				buttonFloraDetails.unclick();
			}
			if ( guiTableFlora.lastClickedIndex != -1 )
			{
				selectedFlora=0;
				lastRowClicked=guiTableFlora.lastClickedIndex;
				std::cout<<"You clicked index: "<<lastRowClicked<<"\n";
				
				// get the flora of this index.
				// (in future an ID system would be useful)]
				int floraIndex=0;
				
				for (int i=0;i<world.vBiome.size();++i)
				{
					World_Biome * biome = world.vBiome(i);
					Vector <Flora*> * vFloraTypes = biome->getAllFloraTypes();
					
					if ( vFloraTypes == 0 )
					{
						// error null vector ptr
						return false;
					}
					
					for (int j=0;j<vFloraTypes->size();++j)
					{
						
						if ( floraIndex == lastRowClicked )
						{
							// we found the matching index.
							selectedFlora=(*vFloraTypes)(j);
							break;
						}
						++floraIndex;
					}
				}
				guiTableFlora.lastClickedIndex = -1;
			}
		}
		return false;
	}
	
	void eventResize()
	{
		guiTableFlora.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonFloraDetails.setPanel(panelX2-150, panelY1+40, panelX2-20, panelY1+20);
		menuFloraDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuFloraDetails.eventResize();
	}
	
};


#endif