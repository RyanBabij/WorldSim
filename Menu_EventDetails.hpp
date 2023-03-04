#pragma once
#ifndef WORLDSIM_MENU_EVENT_DETAILS_CPP
#define WORLDSIM_MENU_EVENT_DETAILS_CPP

/* WorldSim: Menu_EventDetails.cpp
	#include "Menu_EventDetails.cpp"

	Menu listing the details of the particular event.
	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_EventDetails: public GUI_Interface
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

	Event* selectedEvent;

	
	Menu_EventDetails()
	{	
		selectedEvent=0;
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
	
	void init(Event* _event)
	{
		if ( _event != 0 || selectedEvent== 0)
		{
			selectedEvent=_event;
		}
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(cNormal,cHighlight,0);
		buttonClose.active=true;

		guiManager.clear();

		guiManager.add(&buttonClose);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		if (selectedEvent==0) { return; }
		
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Event details",panelX1,panelY2-20,panelX2,panelY2-5, true, true);

			int yOffset=35;
			int vSpacing=12;
			int xBuffer = 50;

			font8x8.drawText(selectedEvent->getLongDescription(),panelX1+xBuffer,panelY2-yOffset,panelX2-xBuffer,panelY1, false, false);
			yOffset+=vSpacing;

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