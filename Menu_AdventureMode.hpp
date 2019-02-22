#pragma once
#ifndef WORLDSIM_MENU_ADVENTUREMODE_HPP
#define WORLDSIM_MENU_ADVENTUREMODE_HPP

#include "World_Viewer.hpp"

/* Menu_AdventureMode.hpp
	#include "Menu_AdventureMode.hpp"

  Adventure mode menu. The player can select a character and play as them.
  This class contains the code for the Adventure Mode GUI.
*/

class Menu_AdventureMode: public GUI_Interface
{
  private:
  	GUI_Manager guiManager;
    
    /* Colours / theme. */
    Colour cNormal;
    Colour cSelected;
    Colour cDropPanel;
    Colour cHighlight;
    
    Wildcat::Font* font;
    
    /* Texture to draw in the background of this menu. */
    Texture* backgroundTexture;
    
  
      /* Button to center camera on current player */
    GUI_Button buttonCenterCamera;
    
      /* Button to toggle sneaking - SNK */
    GUI_Button buttonSneak;
  
  
      /* Button to view inventory */
    GUI_Button buttonInventory;
    
      /* Button to view manual */
    GUI_Button buttonManual;
    
        //Render very basic manual for now. Future manual should have stuff like bestiary, alchemy instructions, crafting instructions, etc.
    bool manualActive;
    
      // Render conversation menu
    bool conversationActive;
      Character* conversationCharacter;
      
      //Render inventory menu
    bool inventoryActive;
    
  public:
  

	

	Menu_AdventureMode()
	{	
		backgroundTexture=&TEX_NEW_GAME_BACKGROUND;
    font = &font8x8;
	}
  
  void init()
  {
		/* Initialise theme. */
		cNormal.set(200,200,200);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(170,170,170);

		buttonCenterCamera.text="";
		buttonSneak.text="SNK";
    buttonSneak.font = font;
		buttonCenterCamera.setColours(&cNormal,&cHighlight,0);
		buttonSneak.setColours(&cNormal,&cHighlight,0);
		guiManager.add(&buttonCenterCamera);
		guiManager.add(&buttonSneak);
		buttonCenterCamera.active=true;
		buttonSneak.active=true;
    buttonCenterCamera.texture = &TEX_GUI_CENTER_CAMERA;
    
    
    buttonInventory.active = true;
    buttonInventory.text = "INV";
    buttonInventory.font = font;
		buttonInventory.setColours(&cNormal,&cHighlight,0);
		guiManager.add(&buttonInventory);
    
    buttonManual.active = true;
    buttonManual.text = "MAN";
    buttonManual.font = font;
		buttonManual.setColours(&cNormal,&cHighlight,0);
		guiManager.add(&buttonManual);
    
    manualActive=false;
    conversationActive = false;
      conversationCharacter=0;
    
    inventoryActive=false;
    
    eventResize();
  }
  
  
  void render()
  {
		/* Preview world view. */
		worldViewer.render();
    
		// MESSAGE CONSOLE		
		const int nY = panelY2-panelY1;
		Renderer::placeColour4a(200,200,250,125,panelX1,panelY1+120,panelX1+220,panelY2);
		int currentY = panelY2-2;
		int iMessage = vConsoleMessage.size()-1;
    
    int linesDrawn = 0;
    
		while (currentY>panelY1+120 && iMessage>=0 && iMessage<vConsoleMessage.size() )
		{
			linesDrawn = font8x8.drawText(vConsoleMessage(iMessage),panelX1,currentY,panelX1+220,currentY-38,false,false);
			--iMessage;
			currentY-=linesDrawn*9;
      currentY-=2;
		}

    Renderer::placeColour4a(200,200,250,250,panelX1,panelY1,panelX1+220,panelY1+220);
    linesDrawn = font8x8.drawText("Minimap",panelX1,panelY1,panelX1+220,panelY1+220,true,true);
    
    Renderer::placeColour4a(150,150,250,250,panelX1,panelY1+220,panelX1+220,panelY1+320);
    linesDrawn = font8x8.drawText("Action Menu",panelX1,panelY1+220,panelX1+220,panelY1+320,true,true);
  
  
    guiManager.render();
    
		// DATE
		//Renderer::placeColour4a(150,150,150,255,panelX2-208,panelY2-20,panelX2,panelY2-10);
		std::string strDate = world.calendar.toString();
		font8x8.drawText("DATE: "+strDate,panelX1,panelY1+220,panelX1+220,panelY1+230, true, true);
    
    
    // Render manual page
    if (manualActive)
    {
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
      linesDrawn = font8x8.drawText(ADVENTURE_MODE_MANUAL,panelX1+250,panelY1+45,panelX2-25,panelY2-25,false,false);
    }
    
    // Render conversation menu
    if (conversationActive && conversationCharacter != 0)
    {
      Renderer::placeColour4a(30,140,40,250,panelX1+235,panelY1+35,panelX2-15,panelY2-10);
      
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-220);
      linesDrawn = font8x8.drawText(playerCharacter->getFullName()+" talks to "+conversationCharacter->getFullName()+".\n\n["+playerCharacter->getFullName()+"]: COLONEL, WHAT'S A CONVERSATION SYSTEM DOING HERE?\n["+conversationCharacter->getFullName()+"]: Snake, remember what De Gaulle said \"The graveyards are full of indispensable men.\" Snake, you're all alone and surrounded by bad guys. Try to be careful and avoid getting into a fight whenever you can.\n["+playerCharacter->getFullName()+"]: I FEEL ASLEEP.\n\n(Press ESC to exit)",panelX1+250,panelY1+45,panelX2-25,panelY2-225,false,false);
      
      Renderer::placeColour4a(150,150,250,250,panelX1+350,panelY2-210,panelX2-130,panelY2-20);
      linesDrawn = font8x8.drawText("Relationship: Solid",panelX1+355,panelY2-215,panelX2-135,panelY2-25,false,false);
      
      
      Renderer::placeTexture4(panelX1+240, panelY2-160, panelX1+340, panelY2-60, &TEX_PORTRAIT_SNEK, true);
      Renderer::placeTexture4(panelX2-120, panelY2-160, panelX2-20, panelY2-60, &TEX_PORTRAIT_LING, true);
    }
    
    // Render inventory menu
    if (inventoryActive)
    {
      Renderer::placeColour4a(30,140,40,250,panelX1+235,panelY1+35,panelX2-15,panelY2-10);
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-220);
      Renderer::placeColour4a(150,150,250,250,panelX1+350,panelY2-210,panelX2-130,panelY2-20);
      
      
      font8x8.drawText("Inventory", panelX1 + 250,panelY2-230,panelX1 + 450,panelY2-250,true,true);
      font8x8.drawText("Storage", panelX1 + 650,panelY2-230,panelX1 + 850,panelY2-250,true,true);
      
      int inventoryY = panelY2-250;
      
      for (int row = 0;row<12;++row)
      { int currentX = panelX1+250;
        for (int i=0;i<10;++i)
        {
          Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
          if ( row==0 )
          {
            font8x8.drawText(DataTools::toString(i),currentX,inventoryY,currentX+32,inventoryY-32,true,true);
          }
          currentX += 34;
        }
        inventoryY -= 34;
        if ( row == 9) {inventoryY -= 5;}
      }
      
      inventoryY = panelY2-250;
      
      for (int row = 0;row<12;++row)
      { int currentX = panelX1+650;
        for (int i=0;i<10;++i)
        {
          Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
          if ( row==0 )
          {
            font8x8.drawText(DataTools::toString(i),currentX,inventoryY,currentX+32,inventoryY-32,true,true);
          }
          currentX += 34;
        }
        inventoryY -= 34;
        if ( row == 9) {inventoryY -= 5;}
      }

      
    }
    
  }
  
	void logicTick()
	{
    return;
	}
  
	bool keyboardEvent (Keyboard* _keyboard)
	{
			// ESCAPE - Close all submenus and go back to main game.
			// If all submenus are already closed, bring up main menu.
		if(_keyboard->isPressed(Keyboard::ESCAPE)) /* Flush console. */
		{
      if ( conversationActive )
      { world.incrementTicksBacklog(5); }
      
      manualActive=false;
      conversationActive=false;
			_keyboard->keyUp(Keyboard::ESCAPE);	
		}
    
      // TAB will switch between adventure mode and god mode.
      // Make sure to relinquish control of character.
		if(_keyboard->isPressed(Keyboard::TAB))
		{
      _keyboard->keyUp(Keyboard::TAB);
      activeMenu = MENU_WORLDSIMULATOR;
		}
    
    if(_keyboard->isPressed(Keyboard::RIGHT))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        wl->moveObject(playerCharacter,playerCharacter->x+1,playerCharacter->y);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
        
      }
      
      _keyboard->keyUp(Keyboard::RIGHT);
    }
    if(_keyboard->isPressed(Keyboard::LEFT))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        wl->moveObject(playerCharacter,playerCharacter->x-1,playerCharacter->y);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
      }
      
      _keyboard->keyUp(Keyboard::LEFT);
    }
    if(_keyboard->isPressed(Keyboard::UP))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y+1);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
      }
      _keyboard->keyUp(Keyboard::UP);
    }
    if(_keyboard->isPressed(Keyboard::DOWN))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y-1);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
      }
      _keyboard->keyUp(Keyboard::DOWN);
    }
      // PERIOD = WAIT
    if(_keyboard->isPressed(Keyboard::PERIOD))
    {
      world.incrementTicksBacklog(1);
      _keyboard->keyUp(Keyboard::PERIOD);
    }
    
      // SPACE = TALK, INTERACT
    if(_keyboard->isPressed(Keyboard::SPACE) && conversationActive==false)
    {

      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        // wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y-1);
        // worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        // world.updateMaps();
        // playerCharacter->updateKnowledge();
        
        
        
        Vector <Character*> * vNearbyCharacter = wl->getAdjacentCharacters(playerCharacter->x,playerCharacter->y);
        
        conversationCharacter = 0;
        
        if ( vNearbyCharacter != 0)
        {
          for (int i=0;i<vNearbyCharacter->size(); ++i)
          {
            std::cout<<(*vNearbyCharacter)(i)->getFullName()<<".\n";
            if ((*vNearbyCharacter)(i)!=playerCharacter)
            {
              conversationCharacter = (*vNearbyCharacter)(i);
            }
          }
        }
        
        if (conversationCharacter != 0)
        {
          std::cout<<playerCharacter->getFullName()<<" talks to "<<conversationCharacter->getFullName()<<".\n";
          conversationActive=true;
        }
      }
      
      _keyboard->keyUp(Keyboard::SPACE);
    }

		guiManager.keyboardEvent(_keyboard);
		worldViewer.keyboardEvent(_keyboard);
		return false;
	}
  
	bool mouseEvent (Mouse* _mouse)
	{
    guiManager.mouseEvent(_mouse);
    
		if (buttonCenterCamera.clicked==true)
		{
      Console("Centered view");
      worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
			buttonCenterCamera.unclick();
		}
    
      // Toggle the manual view.
		if (buttonManual.clicked==true)
		{
      std::cout<<"MANUAL\n";
      manualActive = !manualActive;
			buttonManual.unclick();
		}
      // Toggle the inventory view.
		if (buttonInventory.clicked==true)
		{
      std::cout<<"INVENTORY\n";
      inventoryActive = !inventoryActive;
			buttonInventory.unclick();
		}
    
      // Toggle sneak.
		if (buttonSneak.clicked==true)
		{
      playerCharacter->isSneaking = !playerCharacter->isSneaking;
      
      if ( playerCharacter->isSneaking )
      { Console("Entered sneaking mode");
        world.updateMaps();
        playerCharacter->updateKnowledge();
      }
      else
      { Console("Exited sneaking mode");
      }
			buttonSneak.unclick();
		}
    
		worldViewer.mouseEvent(_mouse);
		return false;
	}
  
	void eventResize()
	{
		buttonCenterCamera.setPanel(panelX1,panelY1+304,panelX1+32,panelY1+320);
		buttonSneak.setPanel(panelX1+33,panelY1+304,panelX1+65,panelY1+320);
		buttonInventory.setPanel(panelX1+66,panelY1+304,panelX1+98,panelY1+320);
		buttonManual.setPanel(panelX1+99,panelY1+304,panelX1+131,panelY1+320);
    
		worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);


	}
	
  
};

#endif
