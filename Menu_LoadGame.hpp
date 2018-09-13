#pragma once
#ifndef GUILD_MENU_LOADGAME_HPP
#define GUILD_MENU_LOADGAME_HPP

class Menu_LoadGame: public Menu_Interface
{
	public:
	Texture* backgroundTexture;
	/* GUI manager. Manages all GUI controls. */
	GUI_Manager guiManager;
	/* Button: Back to main menu. */
	GUI_Button buttonBack;
	/* Colours / theme. */
	Colour cNormal;
	Colour cSelected;
	Colour cDropPanel;
	Colour cHighlight;
  
  Menu_LoadGame()
  {
    menuID = MENU_LOADGAME;
  }
  ~Menu_LoadGame()
  {
  }
	
	void render()
	{
		/* Render background image. */
		Renderer::placeTexture4(0,0,1024,600,backgroundTexture);
		guiManager.render();
	}
	
	void init()
	{
		backgroundTexture=&TEX_LOAD_GAME_BACKGROUND;
		active=false;
		
		/* Initialise theme. */
		cNormal.set(220,220,220);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(255,160,160);
		
		buttonBack.setPanel((1024/2)-70, 400, (1024/2)+70, 420);
		buttonBack.text="Back";
		buttonBack.setColours(&cNormal,&cHighlight,0);
		
		
		guiManager.addControl(&buttonBack);
		
		guiManager.setFont(font);
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
		if(active)
		{
			/* If the guiManager did something with the mouse event. */
			if(guiManager.mouseEvent(_mouse)==true)
			{
			
				if(buttonBack.clicked==true)
				{
					buttonBack.unclick();

					//std::cout<<"Back.\n";
					active=false;
				}
			}

		}
		return false;
	}
	
	
};

#endif
