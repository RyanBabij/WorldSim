#pragma once
#ifndef WORLDSIM_MENU_CHARACTER_DETAILS_CPP
#define WORLDSIM_MENU_CHARACTER_DETAILS_CPP

/* WorldSim: Menu_CharacterDetails.cpp
	#include "Menu_CharacterDetails.cpp"

	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_CharacterDetails: public GUI_Interface
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

	GUI_Button buttonFavourite;
	GUI_Button buttonPossess;
	GUI_Button buttonSmite;

	Character* selectedCharacter;
	
	GUI_Link textFatherLink;
	GUI_Link textMotherLink;
	GUI_Link textSpouseLink;
	Vector <GUI_Link*> textChildLink;
  
	Vector <GUI_Link*> textKillLink;
	
	Menu_CharacterDetails()
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

		buttonFavourite.text="Favourite";
		buttonFavourite.setColours(cNormal,cHighlight,0);
		buttonFavourite.active=true;

		buttonPossess.text="Control";
		buttonPossess.setColours(cNormal,cHighlight,0);
		buttonPossess.active=true;
		
		buttonSmite.text="Smite";
		buttonSmite.setColours(cNormal,cHighlight,0);
		buttonSmite.active=true;
		
		
		
		
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
		guiManager.add(&buttonFavourite);
		guiManager.add(&buttonPossess);
		guiManager.add(&buttonSmite);
		guiManager.add(&textFatherLink);
		guiManager.add(&textMotherLink);
		guiManager.add(&textSpouseLink);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		if (selectedCharacter==0) { return; }
		
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Character sheet",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			int yOffset=35;
			int vSpacing=12;
			int leftMargin=5;
			
			std::string characterName = "Name: " + selectedCharacter->getFullName() + ".";
			font8x8.drawText
			(characterName,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			yOffset+=vSpacing;
			font8x8.drawText
			("Age: "+DataTools::toString(selectedCharacter->age)
			,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			yOffset+=vSpacing;
			
			std::string strSex;
			if ( selectedCharacter->isMale ) { strSex="Male"; }
			else { strSex="Female"; }
			font8x8.drawText("Sex: "+strSex,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			yOffset+=vSpacing;
			
			if ( selectedCharacter->father != 0 )
			{
				//font8x8.drawText("Father: " + selectedCharacter->father->getFullName() + ".",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textFatherLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textFatherLink.text = "Father: " + selectedCharacter->father->getFullName() + ".";
			}
			else
			{
				//font8x8.drawText("No father.",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textFatherLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textFatherLink.text = "No father.";
			}
			yOffset+=vSpacing;
			
			if ( selectedCharacter->mother != 0 )
			{
				//font8x8.drawText("Father: " + selectedCharacter->father->getFullName() + ".",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textMotherLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textMotherLink.text = "Mother: " + selectedCharacter->mother->getFullName() + ".";
			}
			else
			{
				//font8x8.drawText("No father.",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textMotherLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textMotherLink.text = "No mother.";
			}
			yOffset+=vSpacing;
			
			if ( selectedCharacter->spouse != 0 )
			{
				//font8x8.drawText("Father: " + selectedCharacter->father->getFullName() + ".",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textSpouseLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textSpouseLink.text = "Spouse: " + selectedCharacter->spouse->getFullName() + ".";
			}
			else
			{
				//font8x8.drawText("No father.",panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing, false, true);
				textSpouseLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textSpouseLink.text = "No spouse.";
			}
			yOffset+=vSpacing;
			
			
			
			
			if ( textChildLink.size()==0)
			{
				font8x8.drawText("No children.",panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			}
			else
			{
				font8x8.drawText("Children:",panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			}
			yOffset+=vSpacing;
			
			for (int i=0;i<textChildLink.size();++i)
			{
				textChildLink(i)->setPanel(panelX1,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				yOffset+=vSpacing;
			}
			
			font8x8.drawText(selectedCharacter->baseSkill.toString(),panelX1+leftMargin,panelY2-yOffset,panelX2,panelY1);
			
			yOffset+=vSpacing*8;
			

			font8x8.drawText(selectedCharacter->getBiography(),panelX1+5,panelY2-yOffset,panelX2,panelY1);

			
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
      
			if (buttonFavourite.clicked==true)
			{
				selectedCharacter->isFavourite=true;
				active=false;
				buttonFavourite.unclick();
			}
      
			if (buttonPossess.clicked==true)
			{
				//std::cout<<"Yeah you can't do that yet.\n";
				world.controlCharacter(selectedCharacter);
				Console(Stream() << "Controlling: " << selectedCharacter->getFullName());

				world.prepareAdventureMode(selectedCharacter);
				//Transition into the Adventure mode GUI. Transitioning back to god mode will relinquish control of the character.
				activeMenu = MENU_ADVENTUREMODE;

				active=false;
				buttonPossess.unclick();
			}
			
			if (buttonSmite.clicked==true)
			{
				Console(Stream() << "Smiting: " << selectedCharacter->getFullName());
				selectedCharacter->die(SMITED);

				active=false;
				buttonSmite.unclick();
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

		buttonFavourite.setPanel(panelX2-100, panelY1+40, panelX2-20, panelY1+20);
		buttonPossess.setPanel(panelX2-100, panelY1+65, panelX2-20, panelY1+45);
		buttonSmite.setPanel(panelX2-100, panelY1+90, panelX2-20, panelY1+70);
	}
	
};


#endif