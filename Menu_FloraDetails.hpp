#pragma once
#ifndef WORLDSIM_MENU_FLORA_DETAILS_CPP
#define WORLDSIM_MENU_FLORA_DETAILS_CPP

/* WorldSim: Menu_FloraDetails.hpp
	#include "Menu_FloraDetails.hpp"

	Submenu with specific details for this Flora. Should contain:
	* Name
	* Description (It is X with Y Z).
	* Location found (It can be found in the X biome)
	* Rarity
	* Harvestable ingredient (cooking, alchemy, crafting)
	* Ingredient effects (Max 4?)

*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_FloraDetails: public GUI_Interface
{
	public:
	GUI_Manager guiManager;

	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;

	Wildcat::Font* font;

	GUI_Button buttonClose;

	Flora* selectedFlora;

	Menu_FloraDetails()
	{	
		selectedFlora=0;
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

	void init(Flora* _flora)
	{
		if ( _flora != 0 || selectedFlora== 0)
		{
			selectedFlora=_flora;
		}

		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);

		buttonClose.text="X";
		buttonClose.setColours(&cNormal,&cHighlight,0);
		buttonClose.active=true;

		guiManager.add(&buttonClose);
		guiManager.setFont(font);
		eventResize();
	}

	void render()
	{
		if (selectedFlora==0)
		{ return; }

		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Flora details",panelX1,panelY2-20,panelX2,panelY2-5, true, true);

			int yOffset=35;
			int vSpacing=12;

			// std::string characterName = "Name: " + selectedFlora->getFullName() + ".";
			// font8x8.drawText(characterName,panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
			// yOffset+=vSpacing;
			// font8x8.drawText("Age: "+DataTools::toString(selectedFlora->age),panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
			// yOffset+=vSpacing;
			// std::string strSex;
			// if ( selectedFlora->isMale ) { strSex="Male"; }
			// else { strSex="Female"; }
			// font8x8.drawText("Sex: "+strSex,panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
			// yOffset+=vSpacing;


			// yOffset+=vSpacing;

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
		}
		return false;
	}

	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
	}

};


#endif