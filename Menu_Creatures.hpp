#pragma once
#ifndef WORLDSIM_MENU_CREATURES_HPP
#define WORLDSIM_MENU_CREATURES_HPP

/* WorldSim: Menu_Creatures.hpp
	#include "Menu_Creatures.hpp"

	Menu of all creatures in World.
*/

#include "Menu_SpeciesDetails.hpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include <Data/DataTools.hpp>

class Menu_Creatures: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
	
		/* Menu for investigating an individual tribe */
	GUI_Button buttonClose;
	
		/* Button to open Creature Details menu */
	GUI_Button buttonCreatureDetails;
	
		// TABLE FOR BIOMES
	Table2 tCreatures;
	GUI_Table guiTableCreatures;
	
	int lastRowClicked;
	Creature_Species * selectedSpecies; // The biome the user has selected in the menu.
	
	Menu_SpeciesDetails menuCreatureDetails;
	
	Menu_Creatures()
	{
		lastRowClicked=-1;
		selectedSpecies=0;
		menuCreatureDetails.active=false;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuCreatureDetails.setFont(_font);
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
		
		buttonCreatureDetails.text="Species Details";
		buttonCreatureDetails.setColours(&cNormal,&cHighlight,0);
		buttonCreatureDetails.active=true;
		
		active = false;
		
		
		guiTableCreatures.clear();
		guiTableCreatures.table = &tCreatures;
		guiTableCreatures.alpha=0;
		guiTableCreatures.active=true;
		guiTableCreatures.addColumn("Name","name",300);
		//guiTableCreatures.addColumn("Size","size",80);
		//guiTableCreatures.addColumn("Type","type",80);

		tCreatures.clear();
		for (int i=0;i<world.vBiome.size();++i)
		{
			World_Biome * biome = world.vBiome(i);
			Vector <Creature_Species*> * vCreatureTypes = biome->getAllCreatureTypes();
			
			if ( vCreatureTypes == 0 )
			{
				// error null vector ptr
				return;
			}
			
			for (int j=0;j<vCreatureTypes->size();++j)
			{
				tCreatures.addRow( (*vCreatureTypes)(j) );
			}
			//tCreatures.addRow(world.vBiome(i));
		}
		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonCreatureDetails);
		guiManager.add(&guiTableCreatures);
	
		eventResize();
	}
	
	void render()
	{
		if (menuCreatureDetails.active)
		{
			menuCreatureDetails.render();
		}
		else if (active)
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Species Info",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if (menuCreatureDetails.active)
		{
			menuCreatureDetails.mouseEvent(_mouse);
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
			if  (buttonCreatureDetails.clicked==true)
			{
				std::cout<<"Clicked creature details.\n";
				if ( selectedSpecies != 0 )
				{
					std::cout<<"Creature details\n";
					menuCreatureDetails.init(selectedSpecies);
					menuCreatureDetails.active=true;
				}
				else
				{
					std::cout<<"Select a Species first.\n";
				}

				buttonCreatureDetails.unclick();
			}
			if ( guiTableCreatures.lastClickedIndex != -1 )
			{
				selectedSpecies=0;
				lastRowClicked=guiTableCreatures.lastClickedIndex;
				std::cout<<"You clicked index: "<<lastRowClicked<<"\n";
				
				// get the species of this index.
				// (in future an ID system would be useful)]
				int creatureIndex=0;
				
				for (int i=0;i<world.vBiome.size()&&selectedSpecies==0;++i)
				{
					World_Biome * biome = world.vBiome(i);
					Vector <Creature_Species*> * vCreatureTypes = biome->getAllCreatureTypes();
					
					if ( vCreatureTypes == 0 )
					{
						// error null vector ptr
						return false;
					}
					
					for (int j=0;j<vCreatureTypes->size()&&selectedSpecies==0;++j)
					{
						
						if ( creatureIndex == lastRowClicked )
						{
							// we found the matching index.
							
							selectedSpecies=(*vCreatureTypes)(j);
							std::cout<<"Found match: "<<selectedSpecies->name<<"\n";
							//break;
						}
						++creatureIndex;
					}
				}
				guiTableCreatures.lastClickedIndex = -1;
			}
		}
		return false;
	}
	
	void eventResize()
	{
		guiTableCreatures.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonCreatureDetails.setPanel(panelX2-150, panelY1+40, panelX2-20, panelY1+20);
		menuCreatureDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuCreatureDetails.eventResize();
	}
	
};


#endif