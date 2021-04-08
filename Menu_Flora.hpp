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
	GUI_Table guiTableBiome;
	
	//Menu_FloraDetails menuFloraDetails;
	
	int lastRowClicked;
	World_Biome * selectedFlora; // The Flora the user has selected in the menu.
	
	Menu_Flora()
	{
		lastRowClicked=-1;
		selectedFlora=0;
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
		
		buttonFloraDetails.text="Flora Details";
		buttonFloraDetails.setColours(&cNormal,&cHighlight,0);
		buttonFloraDetails.active=true;
		
		active = false;
		
		guiTableBiome.clear();
		guiTableBiome.table = &tFlora;
		guiTableBiome.alpha=0;
		guiTableBiome.active=true;
		guiTableBiome.addColumn("Name","name",300);
		//guiTableBiome.addColumn("Size","size",80);
		//guiTableBiome.addColumn("Type","type",80);

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
			//tFlora.addRow(world.vBiome(i));
		}
		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonFloraDetails);
		guiManager.add(&guiTableBiome);
	
		eventResize();
	}
	
	void render()
	{
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Flora Info",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
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
			if  (buttonFloraDetails.clicked==true)
			{
				std::cout<<"Clicked flora details.\n";
				if ( selectedFlora != 0 )
				{
					std::cout<<"Flora details\n";
					//menuFloraDetails.init(selectedFlora);
					//menuFloraDetails.active=true;
				}
				else
				{
					std::cout<<"Select a Flora first.\n";
				}

				buttonFloraDetails.unclick();
			}
			// if ( guiTableBiome.lastClickedIndex != -1 )
			// {
				// lastRowClicked=guiTableBiome.lastClickedIndex;
				
				// // find the biome with the correct id
				// //for (int i=0;i<world.vBiome
				// World_Biome * b = world.getBiome(lastRowClicked);
				
				// if (b)
				// {
					// selectedFlora = b;
				// }
				
				// //if (world.vBiome.isSafe(lastRowClicked))
				// //{
				// //	selectedBiome=world.vBiome(lastRowClicked);
				// //}
				// guiTableBiome.lastClickedIndex = -1;
			// }
		}
		return false;
	}
	
	void eventResize()
	{
		guiTableBiome.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonFloraDetails.setPanel(panelX2-150, panelY1+40, panelX2-20, panelY1+20);
	}
	
};


#endif