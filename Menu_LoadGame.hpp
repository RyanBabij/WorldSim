#pragma once
#ifndef WORLDSIM_MENU_LOADGAME_HPP
#define WORLDSIM_MENU_LOADGAME_HPP

class Menu_LoadGame: public GUI_Interface
{
	public:
	Texture* backgroundTexture;
	/* GUI manager. Manages all GUI controls. */
	GUI_Manager guiManager;
	/* Button: Back to main menu. */
	GUI_Button buttonBack;
	/* Colours / theme. */
	ColourRGB <unsigned char> cNormal;
	ColourRGB <unsigned char> cSelected;
	ColourRGB <unsigned char> cDropPanel;
	ColourRGB <unsigned char> cHighlight;
  
  Menu_LoadGame()
  {
  }
  ~Menu_LoadGame()
  {
  }
	
	void render()
	{
		/* Render background image. */
		Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
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
		
		/* Update control positions. */
		buttonBack.setPanel(panelCenterX-60, panelY2-20, panelCenterX+60, panelY2-40);
		buttonBack.text="Back";
		buttonBack.setColours(cNormal,cHighlight,0);
		
		
		guiManager.addControl(&buttonBack);
		
		guiManager.setFont(font);
	}
	
	void eventResize()
	{
		/* Update control positions. */
		buttonBack.setPanel(panelCenterX-60, panelY2-20, panelCenterX+60, panelY2-40);
	}
  
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
	
	bool mouseEvent (Mouse* _mouse)
	{
    /* If the guiManager did something with the mouse event. */
    if(guiManager.mouseEvent(_mouse)==true)
    {
    
      if(buttonBack.clicked==true)
      {
        buttonBack.unclick();
        activeMenu = MENU_TITLE;
      }
    }

		return false;
	}
	
	
};

#endif
