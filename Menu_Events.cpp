#pragma once
#ifndef WORLDSIM_MENU_EVENTS_CPP
#define WORLDSIM_MENU_EVENTS_CPP

/* WorldSim: Menu_Events.cpp
	#include "Menu_Events.cpp"

	Lists notable events in the world.
	
*/

#include "World_Events.cpp"

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

	Character* selectedCharacter;
	
	GUI_Link textFatherLink;
	GUI_Link textMotherLink;
	GUI_Link textSpouseLink;
	Vector <GUI_Link*> textChildLink;
  
	Vector <GUI_Link*> textKillLink;
	
	// Table of Events
	Table2 tEvents;
	GUI_Table guiTableEvents;
	
	Menu_Events()
	{	
		selectedCharacter=0;
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
	
	void init(Character* _character)
	{
		// init and populate table
		guiTableEvents.clear();
		guiTableEvents.table = &tEvents;
		guiTableEvents.alpha=0;
		guiTableEvents.active=true;
		guiTableEvents.addColumn("Date","date",120);
		guiTableEvents.addColumn("Type","type",120);
		guiTableEvents.addColumn("Description","description",240);
		tEvents.clear();
		
		for (int i=0;i<world.events.size();++i)
		{
			tEvents.addRow(world.events.get(i));
		}
		

		if ( _character != 0 || selectedCharacter== 0)
		{
			selectedCharacter=_character;
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

		textFatherLink.text = "Test";
		textMotherLink.text = "Test";
		textSpouseLink.text = "Test";
		
		//std::cout<<"Clearing children vector\n";
		for (int i=0;i<textChildLink.size();++i)
		{
			delete textChildLink(i);
		}
		textChildLink.clear();
		
		//std::cout<<"Clearing children vector2\n";
		
		guiManager.clear();
		if ( selectedCharacter != 0)
		{
			for (int i=0;i<selectedCharacter->vChildren.size();++i)
			{
				GUI_Link * childLink = new GUI_Link;
				childLink->text = selectedCharacter->vChildren(i)->getFullName();
				textChildLink.push(childLink);
				guiManager.add(childLink);
			}
		}
//std::cout<<"Clearing children vector 3\n";

		guiManager.add(&buttonClose);
		guiManager.add(&buttonEventDetails);
		guiManager.add(&textFatherLink);
		guiManager.add(&textMotherLink);
		guiManager.add(&textSpouseLink);
		guiManager.add(&guiTableEvents);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
		//if (selectedCharacter==0) { return; }
		
		if ( active )
		{
			font8x8.drawText("Events",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
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
      
			if (buttonEventDetails.clicked==true)
			{
				std::cout<<"Put event details menu here\n";
				active=false;
				buttonEventDetails.unclick();
			}
			
			if (textFatherLink.clicked==true)
			{
				std::cout<<"Clicked father\n";
				textFatherLink.unclick();
				
				if ( selectedCharacter->father != 0)
				{
					selectedCharacter=selectedCharacter->father;
					init();
					_mouse->isLeftClick=false;
				}
			}
			if (textMotherLink.clicked==true)
			{
				std::cout<<"Clicked mother\n";
				textMotherLink.unclick();
				
				if ( selectedCharacter->mother != 0)
				{
					selectedCharacter=selectedCharacter->mother;
					init();
					_mouse->isLeftClick=false;
				}
			}
			if (textSpouseLink.clicked==true)
			{
				std::cout<<"Clicked spouse\n";
				textSpouseLink.unclick();
				
				if ( selectedCharacter->spouse != 0)
				{
					selectedCharacter=selectedCharacter->spouse;
					init();
					_mouse->isLeftClick=false;
				}
			}
			for (int i=0;i<textChildLink.size();++i)
			{
				if ( textChildLink(i)->clicked==true)
				{
					selectedCharacter = selectedCharacter->vChildren(i);
					init();
					_mouse->isLeftClick=false;
				}
			}
		
		}
		
		return false;
	}
	
	void eventResize()
	{
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonEventDetails.setPanel(panelX2-140, panelY1+40, panelX2-20, panelY1+20);
		guiTableEvents.setPanel(panelX1,panelY1,panelX2,panelY2-120);
	}
	
};


#endif