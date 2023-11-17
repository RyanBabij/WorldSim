#pragma once
#ifndef WORLDSIM_MENU_SETTLEMENT_DETAILS_CPP
#define WORLDSIM_MENU_SETTLEMENT_DETAILS_CPP

/* WorldSim: Menu_SettlementDetails.cpp
	#include "Menu_SettlementDetails.cpp"

	
*/

#include <Graphics/GUI/GUI_Table.hpp>
#include <Container/Table/Table.hpp>

class Menu_SettlementDetails: public GUI_Interface
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

	// GUI_Button buttonFavourite;
	// GUI_Button buttonPossess;

	Settlement* selectedSettlement;
	
	GUI_Link textLeaderLink;
	// GUI_Link textMotherLink;
	// GUI_Link textSpouseLink;
	// Vector <GUI_Link*> textChildLink;
  
	// Vector <GUI_Link*> textKillLink;
	
	Menu_SettlementDetails()
	{	
		selectedSettlement=0;
		
		textLeaderLink.setRGB(255,255,0);
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
	
	void init(Settlement* _selectedSettlement)
	{
		if ( _selectedSettlement != 0 || selectedSettlement== 0)
		{
			selectedSettlement=_selectedSettlement;
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
		
		// if ( selectedCharacter != 0)
		// {
			// for (int i=0;i<selectedCharacter->vChildren.size();++i)
			// {
				// GUI_Link * childLink = new GUI_Link;
				// childLink->text = selectedCharacter->vChildren(i)->getFullName();
				// textChildLink.push(childLink);
				// guiManager.add(childLink);
			// }
		// }

		guiManager.add(&buttonClose);
		// guiManager.add(&buttonFavourite);
		// guiManager.add(&buttonPossess);
		guiManager.add(&textLeaderLink);
		// guiManager.add(&textMotherLink);
		// guiManager.add(&textSpouseLink);

		guiManager.setFont(font);
	
		eventResize();
	}
	
	void render()
	{
		if (selectedSettlement==0) { return; }
		
		if ( active )
		{
			Renderer::placeColour4a(150,150,150,220,panelX1,panelY1,panelX2,panelY2);
			font8x8.drawText("Settlement details",panelX1,panelY2-20,panelX2,panelY2-5, true, true);
			
			int yOffset=35;
			int vSpacing=12;
			int leftMargin=5;
			
			std::string settlementName = "Name: " + selectedSettlement->getName() + ".";
			font8x8.drawText
			(settlementName,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			yOffset+=vSpacing;
		
			font8x8.drawText
			("Population: "+DataTools::toString(selectedSettlement->vCharacter.size())
			,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			yOffset+=vSpacing;
			
			Character * leader = selectedSettlement->government.leader.character;
			
			textLeaderLink.setFont(&font8x8White);
			textLeaderLink.setRGB(255,255,0);
			
			if ( leader != 0 )
			{
				textLeaderLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textLeaderLink.text = "Leader: " + leader->getFullName() + ".";
			}
			else
			{
				textLeaderLink.setPanel(panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
				textLeaderLink.text = "No leader.";
			}
			
			// if ( leader != 0 )
			// {
				// font8x8.drawText
				// ("Leader: "+leader->getFullName() ,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			// }
			// else
			// {
				// font8x8.drawText
				// ("Leader: None" ,panelX1+leftMargin,panelY2-yOffset,panelX2,panelY2-yOffset+vSpacing);
			// }
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
			
			if (textLeaderLink.clicked==true)
			{
				std::cout<<"Clicked leader\n";
				textLeaderLink.unclick();
				
				Character * leader = selectedSettlement->government.leader.character;
				
				
				if ( leader != 0)
				{
					// selectedCharacter=selectedCharacter->father;
					// init();
					_mouse->isLeftClick=false;
				}
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