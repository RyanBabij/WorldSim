#pragma once
#ifndef GUILD_MENU_OPTIONS_HPP
#define GUILD_MENU_OPTIONS_HPP

class Menu_Options: public GUI_Interface
{
	public:
	//GUI_TextBox menuHeading;
	Texture* backgroundTexture;
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
	/* Button: Back to main menu. */
	GUI_Button buttonBack;
	/* GUI manager. Manages all GUI controls. */
	GUI_Manager guiManager;
  
  Menu_Options()
  {
  }
  ~Menu_Options()
  {
  }
	
	void render()
	{
		/* Render background image. */
		Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
		
		guiManager.render();
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
  
	void eventResize()
	{
		/* Update control positions. */
		buttonBack.setPanel(panelCenterX-60, panelY2-20, panelCenterX+60, panelY2-40);


	}
	
	void init()
	{
		backgroundTexture=&TEX_OPTIONS_BACKGROUND;
		active=false;
		
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		/* Update control positions. */
		buttonBack.setPanel(panelCenterX-60, panelY2-20, panelCenterX+60, panelY2-40);
		buttonBack.text="Back";
		buttonBack.setColours(&cNormal,&cHighlight,0);
		
		
		guiManager.add(&buttonBack);
		
		guiManager.setFont(font);
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
			/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
			
				if(buttonBack.clicked==true)
				{
					buttonBack.unclick();
					//std::cout<<"Back.\n";
					//active=false;
          activeMenu = MENU_TITLE;
				}
			}

		return false;
	}
	
};

#endif