#pragma once
#ifndef WORLDSIM_MENU_ADVENTUREMODE_HPP
#define WORLDSIM_MENU_ADVENTUREMODE_HPP

#include "World_Viewer.hpp"

/* Menu_AdventureMode.hpp
	#include "Menu_AdventureMode.hpp"

  Adventure mode menu. The player can select a character and play as them.
  This class contains the code for the Adventure Mode GUI.
*/

//PLAYER INVENTORY
// NPCs only have a list of items in their inventory.
// However the player gets a grid which saves the position of items
// The grid is 10*10. I think it's doubtful that a player will need more than
// 100 inventory slots.

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
    
      /* Button to view character sheet - CHR */
    GUI_Button buttonCharacterSheet;
    
        //Render very basic manual for now. Future manual should have stuff like bestiary, alchemy instructions, crafting instructions, etc.
    bool manualActive;
    
      // Render conversation menu
    bool conversationActive;
      Character* conversationCharacter;
      
      // Render character sheet
    bool characterSheetActive;
    
      //Render inventory menu
    bool inventoryActive;
      int clickedItemSlot;
      Vector <Item*> vItemsOnFloor;
        // Carried item is an item selected by the mouse. The item will follow the cursor.
      Item* carriedItem;
      
      Item * inventoryGrid [10][10];
      int selectedHotbar;
      
      //Render item selection
    bool itemSelectionActive;
      unsigned long int tileSelectAbsoluteX, tileSelectAbsoluteY;
      LocalTile* localTileSelected; /* LocalTile that the player last selected */
      int selectedItemSlot;
        bool subItemSelectionActive;
        Item* useItem; /* Item to use on target */
    
      // CTRL is modifier for inventory menu. Will add 1 of item to inventory, or transfer 1 of item to storage. Or drop 1 of item.
    bool holdingCTRL;
    
    int mouseX, mouseY;
    
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
    
    buttonCharacterSheet.active = true;
    buttonCharacterSheet.text = "CHR";
    buttonCharacterSheet.font = font;
		buttonCharacterSheet.setColours(&cNormal,&cHighlight,0);
		guiManager.add(&buttonCharacterSheet);
    
    itemSelectionActive=false;
    tileSelectAbsoluteX=-1;
    tileSelectAbsoluteY=-1;
    localTileSelected=0;
    selectedItemSlot=0;
    subItemSelectionActive=false;
    useItem=0;
    
    characterSheetActive=false;
    
    manualActive=false;
    conversationActive = false;
      conversationCharacter=0;
    
    inventoryActive=false;
      clickedItemSlot=-1;
      carriedItem=0;
      
    holdingCTRL = false;
    
    mouseX=0;
    mouseY=0;
    
    selectedHotbar=0;
    
    //Initialise inventory grid.
    for (int _y=0;_y<10;++_y)
    {
      for (int _x=0;_x<10;++_x)
      {
        inventoryGrid[_x][_y]=0;
      }
    }
      
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
    
    Renderer::placeColour4a(150,150,150,250,panelX1,panelY1+220,panelX1+220,panelY1+320);
    linesDrawn = font8x8.drawText("Action Menu",panelX1,panelY1+220,panelX1+220,panelY1+320,true,true);
    
    //BARS: Health, Stamina, Hunger, Thirst, Energy
    
    Renderer::placeColour4a(255,0,0,255,panelX1,panelY1+232,panelX1+220,panelY1+234);
    Renderer::placeColour4a(0,0,255,255,panelX1,panelY1+229,panelX1+220,panelY1+231);
    Renderer::placeColour4a(128,64,0,255,panelX1,panelY1+226,panelX1+220,panelY1+228);
    Renderer::placeColour4a(128,128,255,255,panelX1,panelY1+223,panelX1+220,panelY1+225);
    Renderer::placeColour4a(255,128,0,255,panelX1,panelY1+220,panelX1+220,panelY1+222);

  
  
    guiManager.render();
    
		// DATE
		//Renderer::placeColour4a(150,150,150,255,panelX2-208,panelY2-20,panelX2,panelY2-10);
		std::string strDate = world.calendar.toString();
		font8x8.drawText("DATE: "+strDate,panelX1,panelY1+235,panelX1+220,panelY1+245, true, true);
    
    //Draw hotbar
    int currentX2 = panelX1 + 240;
    for (int i=0;i<10;++i)
    { Renderer::placeColour4a(120,120,120,250,currentX2,panelY1+5,currentX2+32,panelY1+37);
      if ( inventoryGrid[i][0] != 0 )
      { Renderer::placeTexture4(currentX2,panelY1+5,currentX2+32,panelY1+37, inventoryGrid[i][0]->currentTexture(), false);
      }
      
      if ( i==selectedHotbar )
      { Renderer::placeTexture4(currentX2,panelY1+5,currentX2+32,panelY1+37, &TEX_GUI_TILE_SELECTION, false);
      }
      
      currentX2+=34;
    }
    
    // Render item selection
    if (itemSelectionActive && localTileSelected != 0)
    {
      //std::cout<<"Items:\n";
      Renderer::placeColour4a(120,120,120,255,panelX1+250,panelY2,panelX1+600,(panelY2)-(localTileSelected->vObject.size()*10));
      Renderer::placeColour4a(180,180,180,255,panelX1+250,panelY2-(selectedItemSlot*10),panelX1+600,panelY2-((selectedItemSlot+1)*10));
      for (int i=0;i<localTileSelected->vObject.size();++i)
      {
        font8x8.drawText(localTileSelected->vObject(i)->getName(),panelX1+250,(panelY2)-(i*10),panelX1+600,(panelY2)-(i*10)-10,false,true);
      }
      //std::cout<<"Selecting tile: "<<worldViewer.hoveredXTileLocal<<", "<<worldViewer.hoveredYTileLocal<<".\n";
      //Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
      //linesDrawn = font8x8.drawText(ADVENTURE_MODE_MANUAL,panelX1+250,panelY1+45,panelX2-25,panelY2-25,false,false);
    }
    
    // Render manual page
    if (manualActive)
    {
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
      linesDrawn = font8x8.drawText(ADVENTURE_MODE_MANUAL,panelX1+250,panelY1+45,panelX2-25,panelY2-25,false,false);
    }
    
    //Render character sheet
    if (characterSheetActive)
    {
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
      linesDrawn = font8x8.drawText("CHARACTER SHEET\nSkills:\nFishing: "+DataTools::toString((int)playerCharacter->skillFishing),panelX1+250,panelY1+45,panelX2-25,panelY2-25,false,false);
    }
    
    // Render conversation menu
    if (conversationActive && conversationCharacter != 0)
    {
      Renderer::placeColour4a(30,140,40,250,panelX1+235,panelY1+40,panelX2-15,panelY2-10);
      
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-162,panelY2-220);
      linesDrawn = font8x8.drawText(playerCharacter->getFullName()+" talks to "+conversationCharacter->getFullName()+".\n\n["+playerCharacter->getFullName()+"]: COLONEL, WHAT'S A CONVERSATION SYSTEM DOING HERE?\n["+conversationCharacter->getFullName()+"]: Snake, remember what De Gaulle said \"The graveyards are full of indispensable men.\" Snake, you're all alone and surrounded by bad guys. Try to be careful and avoid getting into a fight whenever you can.\n["+playerCharacter->getFullName()+"]: I FEEL ASLEEP.\n\n(Press ESC to exit)",panelX1+242,panelY1+42,panelX2-164,panelY2-222,false,false);
      
      Renderer::placeColour4a(150,150,250,250,panelX2-160,panelY1+40,panelX2-20,panelY2-220);
      linesDrawn = font8x8.drawText("WE\nMORROWIND\nNOW",panelX2-158,panelY1+42,panelX2-22,panelY2-222,false,false);
      
      Renderer::placeColour4a(150,150,250,250,panelX1+350,panelY2-210,panelX2-130,panelY2-20);
      linesDrawn = font8x8.drawText("Disposition: Solid",panelX1+355,panelY2-215,panelX2-135,panelY2-25,false,false);
      
      
      Renderer::placeTexture4(panelX1+240, panelY2-160, panelX1+340, panelY2-60, &TEX_PORTRAIT_SNEK, true);
      Renderer::placeTexture4(panelX2-120, panelY2-160, panelX2-20, panelY2-60, &TEX_PORTRAIT_LING, true);
    }
    
    // Render inventory menu
    // Yes this will need to be moved into its own file at some point,
    // but hey who cares as long as it works.
    if (inventoryActive)
    {
      // background panels for inventory menu.
      Renderer::placeColour4a(30,140,40,250,panelX1+235,panelY1+35,panelX2-15,panelY2-10);
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-220);
      Renderer::placeColour4a(150,150,250,250,panelX1+350,panelY2-210,panelX2-130,panelY2-20);
      
      font8x8.drawText("Inventory", panelX1 + 250,panelY2-230,panelX1 + 450,panelY2-250,true,true);
      font8x8.drawText("Storage", panelX1 + 650,panelY2-230,panelX1 + 850,panelY2-250,true,true);

      
      // render inventory slots
      int inventoryY = panelY2-250;
      int currentFloorItem = 0;
        
      for (int row = 0;row<12;++row)
      { int currentX = panelX1+250;
        for (int i=0;i<10;++i)
        {
          Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
          
          if (row < 10 && i < 10 && inventoryGrid[i][row]!=0)
          {
            //Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
            Renderer::placeTexture4(currentX,inventoryY-32,currentX+32,inventoryY, inventoryGrid[i][row]->currentTexture(), false);
          }
          
            // floor inventory
          if (row == 10 || row == 11 )
          {
            if ( currentFloorItem < vItemsOnFloor.size() && vItemsOnFloor(currentFloorItem) != carriedItem )
            {
              Renderer::placeTexture4(currentX,inventoryY-32,currentX+32,inventoryY, vItemsOnFloor(currentFloorItem)->currentTexture(), false);
              ++currentFloorItem;
            }
          }
          
          if ( row==0 )
          {
            font8x8.drawText(DataTools::toString(i),currentX,inventoryY,currentX+32,inventoryY-32,true,true);
          }
          currentX += 34;
        }
        inventoryY -= 34;
          // make a gap for floor inventory
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

      // Render carried item
      if ( carriedItem != 0)
      {
        Renderer::placeTexture4(mouseX,mouseY-32,mouseX+32,mouseY, carriedItem->currentTexture(), false);
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
      characterSheetActive=false;
      inventoryActive=false;
      itemSelectionActive=false;
      worldViewer.showHoveredTile = false;
			_keyboard->keyUp(Keyboard::ESCAPE);	
		}
    
      // TAB will switch between adventure mode and god mode.
      // Make sure to relinquish control of character.
		if(_keyboard->isPressed(Keyboard::TAB))
		{
      _keyboard->keyUp(Keyboard::TAB);
      activeMenu = MENU_WORLDSIMULATOR;
		}
    
    if(_keyboard->isPressed(Keyboard::RIGHT) || _keyboard->isPressed(Keyboard::D) || _keyboard->isPressed(Keyboard::d))
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
      _keyboard->keyUp(Keyboard::D);
      _keyboard->keyUp(Keyboard::d);
    }
    if(_keyboard->isPressed(Keyboard::LEFT) || _keyboard->isPressed(Keyboard::A) || _keyboard->isPressed(Keyboard::a))
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
      _keyboard->keyUp(Keyboard::A);
      _keyboard->keyUp(Keyboard::a);
    }
    if(_keyboard->isPressed(Keyboard::UP) || _keyboard->isPressed(Keyboard::W) || _keyboard->isPressed(Keyboard::w))
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
      _keyboard->keyUp(Keyboard::W);
      _keyboard->keyUp(Keyboard::w);
    }
    if(_keyboard->isPressed(Keyboard::DOWN) || _keyboard->isPressed(Keyboard::S) || _keyboard->isPressed(Keyboard::s))
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
      _keyboard->keyUp(Keyboard::S);
      _keyboard->keyUp(Keyboard::s);
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
    
    // Hotbar selection keys
		if(_keyboard->isPressed(Keyboard::ONE))
		{
      selectedHotbar=0;
			_keyboard->keyUp(Keyboard::ONE);	
		}
		if(_keyboard->isPressed(Keyboard::TWO))
		{
      selectedHotbar=1;
			_keyboard->keyUp(Keyboard::TWO);	
		}
		if(_keyboard->isPressed(Keyboard::THREE))
		{
      selectedHotbar=2;
			_keyboard->keyUp(Keyboard::THREE);	
		}
		if(_keyboard->isPressed(Keyboard::FOUR))
		{
      selectedHotbar=3;
			_keyboard->keyUp(Keyboard::FOUR);	
		}
		if(_keyboard->isPressed(Keyboard::FIVE))
		{
      selectedHotbar=4;
			_keyboard->keyUp(Keyboard::FIVE);	
		}
		if(_keyboard->isPressed(Keyboard::SIX))
		{
      selectedHotbar=5;
			_keyboard->keyUp(Keyboard::SIX);	
		}
		if(_keyboard->isPressed(Keyboard::SEVEN))
		{
      selectedHotbar=6;
			_keyboard->keyUp(Keyboard::SEVEN);	
		}
		if(_keyboard->isPressed(Keyboard::EIGHT))
		{
      selectedHotbar=7;
			_keyboard->keyUp(Keyboard::EIGHT);	
		}
		if(_keyboard->isPressed(Keyboard::NINE))
		{
      selectedHotbar=8;
			_keyboard->keyUp(Keyboard::NINE);	
		}
		if(_keyboard->isPressed(Keyboard::ZERO))
		{
      selectedHotbar=9;
			_keyboard->keyUp(Keyboard::ZERO);	
		}
    
      // I = TOGGLE INVENTORY
    if(_keyboard->isPressed(Keyboard::I) || _keyboard->isPressed(Keyboard::i))
    {
      inventoryActive = !inventoryActive;
      
      // build list of items on the floor
      if (inventoryActive)
      {
        vItemsOnFloor.clear();
        World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
        if ( wl != 0 )
        {
          for (int i=0;i<wl->vItem.size();++i)
          {
            if (wl->vItem(i)->x == playerCharacter->x && wl->vItem(i)->y == playerCharacter->y)
            {
              vItemsOnFloor.push(wl->vItem(i));
            }
          }
        }
      } 
      
      _keyboard->keyUp(Keyboard::I);
      _keyboard->keyUp(Keyboard::i);
    }

      // E = Use item. Currently context-sensitive. In future there might be a menu of choices.
    if(_keyboard->isPressed(Keyboard::E) || _keyboard->isPressed(Keyboard::e))
    {
      itemSelectionActive = !itemSelectionActive;
      if ( selectedHotbar >= 0 && selectedHotbar < 10 && inventoryGrid[selectedHotbar][0] != 0 )
      {
        useItem = inventoryGrid[selectedHotbar][0];
        
        // Check here if item is self-use, or requires target.
        
        //std::cout<<"Using: "<<useItem->getName()<<".\n";
        //playerCharacter->useItem(useItem);
      }
      
      worldViewer.showHoveredTile = itemSelectionActive;
      localTileSelected=0;
      subItemSelectionActive=false;
      selectedItemSlot=0;

      _keyboard->keyUp(Keyboard::E);
      _keyboard->keyUp(Keyboard::e);
    }
    
		guiManager.keyboardEvent(_keyboard);
		worldViewer.keyboardEvent(_keyboard);
		return false;
	}
  
	bool mouseEvent (Mouse* _mouse)
	{
    //int itemClicked = 0; /* 0-99 == Inventory. 100-119 == Ground */
    mouseX = _mouse->x;
    mouseY = _mouse->y;
    
    if (itemSelectionActive)
    {
      
      // Scroll up and down item select
      if (subItemSelectionActive && _mouse->isWheelDown && localTileSelected != 0)
      {
        ++selectedItemSlot;
        if (selectedItemSlot>=localTileSelected->vObject.size()) {selectedItemSlot=0;}
        _mouse->isWheelDown=false;
      }
      if (subItemSelectionActive && _mouse->isWheelUp && localTileSelected != 0)
      {
        --selectedItemSlot;
        if (selectedItemSlot<0) { selectedItemSlot=localTileSelected->vObject.size()-1; }
        _mouse->isWheelUp=false;
      }
      
      if (subItemSelectionActive && _mouse->isLeftClick)
      {
        
        if(localTileSelected->vObject(selectedItemSlot) == 0)
        {
          subItemSelectionActive=false;
          itemSelectionActive=false;
          _mouse->isLeftClick=false;
          worldViewer.showHoveredTile = false;
          localTileSelected=0;
          selectedItemSlot=0;
          return false;
        }
        if ( useItem== 0)
        {
          std::cout<<"You punch the "<<localTileSelected->vObject(selectedItemSlot)->getName()<<".\n";
        }
        else
        {
          std::cout<<"You use the "<<useItem->getName()<<" on the "<<localTileSelected->vObject(selectedItemSlot)->getName()<<".\n";
          useItem->interact(localTileSelected->vObject(selectedItemSlot));
        }

        
        subItemSelectionActive=false;
        itemSelectionActive=false;
        worldViewer.showHoveredTile = false;
        localTileSelected=0;
        selectedItemSlot=0;
        _mouse->isLeftClick=false;
      }
      if (subItemSelectionActive==false && _mouse->isLeftClick)
      {
        tileSelectAbsoluteX=worldViewer.hoveredAbsoluteX;
        tileSelectAbsoluteY=worldViewer.hoveredAbsoluteY;
        
        if (world.isSafe(tileSelectAbsoluteX,tileSelectAbsoluteY) && world(tileSelectAbsoluteX,tileSelectAbsoluteY)->vObject.size() != 0)
        {
          localTileSelected=world(tileSelectAbsoluteX,tileSelectAbsoluteY);
          subItemSelectionActive=true;
        }
        _mouse->isLeftClick=false;
      }

    }
    
    // Player can use CTRL+scroll to scroll the hotbar.
		if(_mouse->isWheelDown && _mouse->ctrlPressed)
		{
      ++selectedHotbar;
      if (selectedHotbar>9) {selectedHotbar=0;}
			_mouse->isWheelDown=false;
		}
		if(_mouse->isWheelUp && _mouse->ctrlPressed)
		{
      --selectedHotbar;
      if (selectedHotbar<0) {selectedHotbar=9;}
			_mouse->isWheelUp=false;
		}
    

    
    if (inventoryActive)
    {
      if (_mouse->isLeftClick)
      {
        // render inventory slots
        int inventoryY = panelY2-250;
        int currentFloorItem = 0;
        
        for (int row = 0;row<12;++row)
        { int currentX = panelX1+250;
          for (int i=0;i<10;++i)
          {
            //Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
            if (_mouse->x >= currentX && _mouse->x <= currentX+32
              && _mouse->y >=inventoryY-32 && _mouse->y <= inventoryY)
            {
              //std::cout<<"CLICKED INVENTORY\n";
              //std::cout<<"SLOT: "<<row*10 + i<<".\n";
              clickedItemSlot = row*10 + i;
              
              if (clickedItemSlot < 100) // INVENTORY GRID
              {
                //Swap carried item and inventory grid item.
                Item * tempItem = carriedItem;
                carriedItem = inventoryGrid[i][row];
                inventoryGrid[i][row]=tempItem;
                _mouse->isLeftClick=false;
              }
              else if ( clickedItemSlot-100 >= 0) // PICKING ITEM OFF GROUND
              {
                if (_mouse->ctrlPressed)
                {
                  if (clickedItemSlot-100 < vItemsOnFloor.size() ) // If player clicked on full ground slot.
                  {
                    Item* tempItem = carriedItem;
                    carriedItem = vItemsOnFloor(clickedItemSlot-100);
                    vItemsOnFloor.removeSlot(clickedItemSlot-100);
                    _mouse->isLeftClick=false;
                    World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
                    if ( wl != 0 )
                    {
                      wl->removeItem(carriedItem);
                      // Put down carried item if necessary
                      if (tempItem != 0)
                      {
                        // Put item on map.
                        wl->putObject(tempItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(tempItem);
                      }
                    }
                  }
                  else // If player clicked on empty ground slot
                  {
                    if (carriedItem != 0) // Drop item on ground
                    {
                      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
                      if ( wl != 0 )
                      {
                        // Put item on map.
                        wl->putObject(carriedItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(carriedItem);
                        vItemsOnFloor.push(carriedItem);
                        carriedItem=0;
                      }
                    }
                  }
                }
                else
                {
                  if (clickedItemSlot-100 < vItemsOnFloor.size() ) // If player clicked on full ground slot.
                  {
                    Item* tempItem = carriedItem;
                    carriedItem = vItemsOnFloor(clickedItemSlot-100);
                    vItemsOnFloor.removeSlot(clickedItemSlot-100);
                    _mouse->isLeftClick=false;
                    World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
                    if ( wl != 0 )
                    {
                      wl->removeItem(carriedItem);
                      // Put down carried item if necessary
                      if (tempItem != 0)
                      {
                        // Put item on map.
                        wl->putObject(tempItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(tempItem);
                      }
                    }
                  }
                  else // If player clicked on empty ground slot
                  {
                    if (carriedItem != 0) // Drop item on ground
                    {
                      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
                      if ( wl != 0 )
                      {
                        // Put item on map.
                        wl->putObject(carriedItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(carriedItem);
                        vItemsOnFloor.push(carriedItem);
                        carriedItem=0;
                      }
                    }
                  }
                }
              }
            }
            
              // floor inventory
            if (row == 10 || row == 11 )
            {
                ++currentFloorItem;
            }
            
            if ( row==0 )
            {
              //font8x8.drawText(DataTools::toString(i),currentX,inventoryY,currentX+32,inventoryY-32,true,true);
            }
            currentX += 34;
          }
          inventoryY -= 34;
            // make a gap for floor inventory
          if ( row == 9) {inventoryY -= 5;}
        }
        //clickedItemSlot = -1;
      }
      //if (_mouse->isRightClick)
      //{carriedItem = 0; clickedItemSlot=-1;
      //}
    }
    
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
    
      // Toggle the character sheet view.
		if (buttonCharacterSheet.clicked==true)
		{
      std::cout<<"CHARACTER SHEET\n";
      characterSheetActive = !characterSheetActive;
			buttonCharacterSheet.unclick();
		}
      // Toggle the inventory view.
		if (buttonInventory.clicked==true)
		{
      std::cout<<"INVENTORY\n";
      inventoryActive = !inventoryActive;
      
      // build list of items on the floor
      if (inventoryActive)
      {
        vItemsOnFloor.clear();
        World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
        if ( wl != 0 )
        {
          for (int i=0;i<wl->vItem.size();++i)
          {
            if (wl->vItem(i)->x == playerCharacter->x && wl->vItem(i)->y == playerCharacter->y)
            {
              vItemsOnFloor.push(wl->vItem(i));
            }
          }
        }
      }
      
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
		buttonCharacterSheet.setPanel(panelX1+132,panelY1+304,panelX1+164,panelY1+320);
    
		worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);


	}
	
  
};

#endif
