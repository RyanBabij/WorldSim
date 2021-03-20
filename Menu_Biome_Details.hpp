#pragma once
#ifndef WORLDSIM_MENU_BIOME_DETAILS_HPP
#define WORLDSIM_MENU_BIOME_DETAILS_HPP

/* WorldSim: Menu_Biome_Details.hpp
	#include "Menu_Biome_Details.hpp"

	Menu showing details of a particular selected biome, stuff like Flora and Creatures which can be found here.
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>
#include <Data/DataTools.hpp>

class Menu_Biome_Details: public GUI_Interface
{
	public:
	GUI_Manager guiManager;
	
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	
	Wildcat::Font* font;
	
		/* Menu for investigating an individual biome */
	GUI_Button buttonClose;
	
	World_Biome * selectedBiome;
	
	Menu_Biome_Details()
	{
		selectedBiome=0;
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

		guiManager.clear();
		guiManager.add(&buttonClose);

		eventResize();
	}
	
	void render()
	{
		if ( active )
		{
			if (selectedBiome==0) { return; }
			
			// if (selectedBiome->isGenerated == false)
			// {
				// std::cout<<"Not generated\n";
				// selectedBiome->generate();
			// }
			
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Biome Details Menu",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			std::cout<<"nflora: "<<selectedBiome->vFlora.size()<<"\n";
			std::string strFlora = "Flora:\n";
			for (int i=0;i<selectedBiome->vFlora.size();++i)
			{
				strFlora+=selectedBiome->vFlora(i)->name;
			}
			
			Vector <Flora*> * vFlora2 = selectedBiome->getAllFloraTypes();
			std::cout<<"nflora2: "<<vFlora2->size()<<"\n";
			
			for (int i=0;i<vFlora2->size();++i)
			{
				strFlora+=(*vFlora2)(i)->name+"\n";
			}
			delete vFlora2;
			
			font8x8.drawText("Menu text.\n"+strFlora,panelX1,panelY1,panelX2,panelY2-25, false, false);
			
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
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}
	
};


#endif