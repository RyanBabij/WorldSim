#pragma once
#ifndef WORLDSIM_MENU_BIOME_HPP
#define WORLDSIM_MENU_BIOME_HPP

/* WorldSim: Menu_Biome.hpp
	#include "Menu_Biome.hpp"

	Shows sortable table of all biomes with size and type. Also centers on the biome when you click the entry.
	Leads to a "Biome Detail" menu.
*/

#include "Menu_Biome_Details.hpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include <Data/DataTools.hpp>

class Menu_Biome: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	ColourRGB <unsigned char> cNormal;
	ColourRGB <unsigned char> cSelected;
	ColourRGB <unsigned char> cDropPanel;
	ColourRGB <unsigned char> cHighlight;
	
	Wildcat::Font* font;
	
		/* Menu for investigating an individual biome */
	GUI_Button buttonClose;
	
	GUI_Button buttonBiomeDetail;
	
		// TABLE FOR BIOMES
	Table2 tBiome;
	GUI_Table guiTableBiome;
	
	int lastRowClicked;
	World_Biome * selectedBiome; // The biome the user has selected in the menu.
	
	Menu_Biome_Details menuBiomeDetails;
	
	Menu_Biome()
	{
		lastRowClicked=-1;
		selectedBiome=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuBiomeDetails.setFont(_font);
	}
	
	void init()
	{
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;
		
		buttonBiomeDetail.text="Details";
		buttonBiomeDetail.setColours(cNormal,cHighlight,0);
		buttonBiomeDetail.active=true;
		
		active = false;
		
		
		guiTableBiome.clear();
		guiTableBiome.table = &tBiome;
		guiTableBiome.alpha=0;
		guiTableBiome.active=true;
		guiTableBiome.addColumn("Name","name",150);
		guiTableBiome.addColumn("Size","size",80);
		guiTableBiome.addColumn("Type","type",80);

		tBiome.clear();
		for (int i=0;i<world.vBiome.size();++i)
		{
			tBiome.addRow(world.vBiome(i));
		}
		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonBiomeDetail);
		guiManager.add(&guiTableBiome);
		
		
		menuBiomeDetails.init();
		menuBiomeDetails.active=false;
		
		eventResize();
		menuBiomeDetails.eventResize();
	}
	
	void render()
	{
		if ( menuBiomeDetails.active )
		{
			menuBiomeDetails.render();
		}
		else if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Biome Menu",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			//font8x8.drawText("This menu will display interesting info about the world. For example, did you know that the world has "+DataTools::toString(world.vTribe.size())+" tribes? Fascinating stuff.\n",panelX1,panelY1,panelX2,panelY2-25, false, false);
			
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuBiomeDetails.active )
		{
			return menuBiomeDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuBiomeDetails.active )
		{
			menuBiomeDetails.mouseEvent(_mouse);
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

			if  (buttonBiomeDetail.clicked==true)
			{
				if ( selectedBiome != 0 )
				{
					//we need to quickly generate the flora if it hasn't been already
					// we might need a higher level meta generator for flora/creatures which
					// doesn't need land to be generated.
					//selectedBiome->generate();
					
					// update: It seems there's no need to do this as the flora and creatures are already generated
					// upon entering world simulator
					
					menuBiomeDetails.selectedBiome=selectedBiome;
					menuBiomeDetails.init();
					menuBiomeDetails.active=true;
				}
				else
				{
					std::cout<<"Select a biome first.\n";
				}

				buttonBiomeDetail.unclick();
			}
			
			
			if ( guiTableBiome.lastClickedIndex != -1 )
			{
				lastRowClicked=guiTableBiome.lastClickedIndex;
				
				// find the biome with the correct id
				//for (int i=0;i<world.vBiome
				World_Biome * b = world.getBiome(lastRowClicked);
				
				if (b)
				{
					selectedBiome = b;
					//std::cout<<"Selectedbiome: "<<selectedBiome->name<<"\n";
					
					if ( b != 0 )
					{
						worldViewer.setCenterTile(b->centerX,b->centerY);
					}
				}
				
				guiTableBiome.lastClickedIndex = -1;
			}
		}
		return false;
	}
	
	void eventResize()
	{
		guiTableBiome.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonBiomeDetail.setPanel(panelX2-150, panelY1+40, panelX2-20, panelY1+20);
		menuBiomeDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuBiomeDetails.eventResize();
	}
	
};


#endif