#pragma once
#ifndef WORLDSIM_MENU_CHARACTER_HPP
#define WORLDSIM_MENU_CHARACTER_HPP

/* Menu_Tribes.cpp
	WorldSim
	Project code name: WorldSim.

	This menu shows a database of all tribes in the world. The player can click a tribe to get a detailed view of the tribe.
*/

#include <GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

#include "Menu_CharacterDetails.cpp"

class Menu_Characters: public GUI_Interface
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
	
		/* Menu for investigating an individual tribe */
	GUI_Button buttonCharacterDetails;
	
		// TABLE FOR CHARACTERS
	Table2 tCharacters;
	GUI_Table guiTableCharacters;
	
	int lastRowClicked;
	//Tribe* selectedTribe;
	Character* selectedCharacter;
	
	Menu_CharacterDetails menuCharacterDetails;
	
	Menu_Characters()
	{	
		lastRowClicked=-1;
		selectedCharacter=0;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
		menuCharacterDetails.setFont(_font);
	}
	
	void init()
	{
		lastRowClicked=-1;
		selectedCharacter=0;
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonClose.text="X";
		buttonClose.setColours(&cNormal,&cHighlight,0);
		buttonClose.active=true;
		
		buttonCharacterDetails.text="Character Sheet";
		buttonCharacterDetails.setColours(&cNormal,&cHighlight,0);
		buttonCharacterDetails.active=true;
		
		guiTableCharacters.clear();
		guiTableCharacters.table = &tCharacters;
		guiTableCharacters.alpha=0;
		guiTableCharacters.active=true;
		guiTableCharacters.addColumn("First name","firstname",120);
		guiTableCharacters.addColumn("Last name","lastname",120);
		guiTableCharacters.addColumn("Age","age",60);
		guiTableCharacters.addColumn("Sex","sex",60);
		guiTableCharacters.addColumn("Strength","strength",100);
		guiTableCharacters.addColumn("Kills","kills",100);
		//guiTableTribes.addColumn("Food","food",120);
		//guiTableTribes.addColumn("Population","population",120);
		//guiTableTribes.addColumn("Coordinates","coordinates",120);
		//guiTableTribes.addColumn("Territory","territory",120);
		
		tCharacters.clear();
		for (int i=0;i<world.vTribe.size();++i)
		{
			for (int i2=0;i2<world.vTribe(i)->vCharacter.size();++i2)
			{
				tCharacters.addRow(world.vTribe(i)->vCharacter(i2));
			}
			//tTribes.addRow(world.vTribe(i));
		}
		
		guiManager.clear();
		guiManager.add(&buttonClose);
		guiManager.add(&buttonCharacterDetails);
		guiManager.add(&guiTableCharacters);
		
		menuCharacterDetails.init();
		menuCharacterDetails.active=false;
		
		eventResize();
		menuCharacterDetails.eventResize();
	}
	
	void render()
	{
		if ( menuCharacterDetails.active )
		{
			menuCharacterDetails.render();
		}
		else if ( active )
		{

			Renderer::placeColour4a(150,150,150,200,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Character List Menu",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			guiManager.render();
		}
	}

	bool keyboardEvent (Keyboard* _keyboard)
	{
		if ( menuCharacterDetails.active )
		{
			return menuCharacterDetails.keyboardEvent(_keyboard);
		}
		else if ( active )
		{
			return guiManager.keyboardEvent(_keyboard);
		}
		
		
		return false;
	}

	bool mouseEvent (Mouse* _mouse)
	{
		if ( menuCharacterDetails.active )
		{
			menuCharacterDetails.mouseEvent(_mouse);
		}
		else if ( active )
		{
				/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
				
			}

			if  (buttonClose.clicked==true)
			{
				std::cout<<"CLOSE\n";
				active=false;
				buttonClose.unclick();
			}
			
			if  (buttonCharacterDetails.clicked==true)
			{
				if ( selectedCharacter != 0 )
				{
					std::cout<<"Character details\n";
					//active=false;
						//menuCharacterDetails.selectedTribe=selectedTribe;
						menuCharacterDetails.init(selectedCharacter);
						
						menuCharacterDetails.active=true;
					//guiTableTribes.active=false;
				}
				else
				{
					std::cout<<"Select a character first.\n";
				}

				buttonCharacterDetails.unclick();
			}
		
		
		
			if ( guiTableCharacters.lastClickedIndex != -1 )
			{
				lastRowClicked=guiTableCharacters.lastClickedIndex;

				int totalIndex = 0;
				for (int i=0;i<world.vTribe.size();++i)
				{
					for (int i2=0;i2<world.vTribe(i)->vCharacter.size();++i2)
					{
						
						if ( totalIndex == lastRowClicked )
						{
							selectedCharacter = world.vTribe(i)->vCharacter(i2);
						}
						++totalIndex;
					}
				}
		
				guiTableCharacters.lastClickedIndex = -1;
			}
		
		}
		
		return false;
	}
	
	void eventResize()
	{
		guiTableCharacters.setPanel(panelX1,panelY1,panelX2,panelY2-30);
		buttonClose.setPanel(panelX2-40, panelY2-40, panelX2-20, panelY2-20);
		buttonCharacterDetails.setPanel(panelX2-150, panelY1+40, panelX2-20, panelY1+20);
		menuCharacterDetails.setPanel(panelX1,panelY1,panelX2,panelY2);
		menuCharacterDetails.eventResize();
	}
	
};


#endif