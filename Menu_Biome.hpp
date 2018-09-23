#pragma once
#ifndef WORLDSIM_MENU_BIOME_HPP
#define WORLDSIM_MENU_BIOME_HPP

/* WorldSim: Menu_Biome.hpp
	#include "Menu_Biome.hpp"

	fff
*/

#include <GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include <Data/DataTools.hpp>

class Menu_Biome: public GUI_Interface
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
	
		// TABLE FOR BIOMES
	Table2 tBiome;
	GUI_Table guiTableBiome;
	
	
	Menu_Biome()
	{	

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
		guiManager.add(&guiTableBiome);
	
		eventResize();
	}
	
	void render()
	{
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Biome Info",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
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
		}
		return false;
	}
	
	void eventResize()
	{
	
		guiTableBiome.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}
	
};


#endif