#pragma once
#ifndef WORLDSIM_MENU_EVENTS_CPP
#define WORLDSIM_MENU_EVENTS_CPP

/* WorldSim: Menu_Events.cpp
	#include "Menu_Events.cpp"

	Lists notable events in the world.
	
*/

#include "World_Events.cpp"
#include "Menu_EventDetails.hpp"

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_Events: public GUI_Interface
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

	GUI_Button buttonEventDetails;

	Event* selectedEvent;
	
	// Table of Events
	Table2 tEvents;
	GUI_Table guiTableEvents;
	
	int lastRowClicked;
	
	Menu_EventDetails menuEventDetails;
	
	Menu_Events()
	{	
		selectedEvent=0;
		lastRowClicked=-1;
	}

	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuEventDetails.setFont(_font);
	}
  
  
	// This is some bad overloading.
	void init()
	{
		init(0);
	}
	
	void init(Event* _event)
	{
		// init and populate table
		guiTableEvents.clear();
		guiTableEvents.table = &tEvents;
		guiTableEvents.alpha=0;
		guiTableEvents.active=true;
		guiTableEvents.addColumn("Date","date",100);
		guiTableEvents.addColumn("Type","type",160);
		guiTableEvents.addColumn("Description","description",480);
		tEvents.clear();
		
		for (int i=0;i<world.eventManager.size();++i)
		{
			tEvents.addRow(world.eventManager.get(i));
		}
		

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

		buttonEventDetails.text="Event Details";
		buttonEventDetails.setColours(cNormal,cHighlight,0);
		buttonEventDetails.active=true;

		guiManager.clear();

		guiManager.add(&buttonClose);
		guiManager.add(&buttonEventDetails);
		guiManager.add(&guiTableEvents);

		guiManager.setFont(font);
	
		menuEventDetails.init();
		menuEventDetails.active=false;
		
		eventResize();
		menuEventDetails.eventResize();
	}
	
	void render()
	{
		if ( menuEventDetails.active )
		{
			menuEventDetails.render();
		}
		else if ( active )
		{
			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			//if (selectedCharacter==0) { return; }
			font8x8.drawText("Events",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuEventDetails.active )
		{
			return menuEventDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		return false;
	}


	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuEventDetails.active )
		{
			menuEventDetails.mouseEvent(_mouse);
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
      
			if (buttonEventDetails.clicked==true)
			{
				if ( selectedEvent != 0 )
				{
					std::cout<<"Event details\n";
					menuEventDetails.init(selectedEvent);
					menuEventDetails.active=true;
					//active=false;
				}
				else
				{
					std::cout<<"Select an event first.\n";
				}

				buttonEventDetails.unclick();
			}
			
			
			if ( guiTableEvents.lastClickedIndex != -1 )
			{
				lastRowClicked=guiTableEvents.lastClickedIndex;

				int totalIndex = 0;
				for (int i=0;i<world.eventManager.vEvent.size();++i)
				{
					if ( lastRowClicked == i )
					{
						selectedEvent = world.eventManager.vEvent(i);
						
						std::cout<<"Event selected: "<<selectedEvent->eventDescription<<"\n";
					}

				}
		
				guiTableEvents.lastClickedIndex = -1;
			}
		
		}
		
		return false;
	}
	
	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonEventDetails.setPanel(panelX2-140, panelY1+40, panelX2-20, panelY1+20);
		guiTableEvents.setPanel(panelX1,panelY1,panelX2,panelY2-120);
		menuEventDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuEventDetails.eventResize();
	}
	
};


#endif