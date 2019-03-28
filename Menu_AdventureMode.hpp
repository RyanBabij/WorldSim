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

// Stores the description, index, and interaction id


#include "Recipe.hpp"

class Menu_Crafting: public GUI_Interface
{
  Vector <Item*> vItem;
  Vector <Recipe*> vRecipe;
  
  int selectedRecipe;
  
  public:
  
  Menu_Crafting()
  {
    selectedRecipe=0;
    vItem.reserve(100);
    vRecipe.reserve(100);
  }
  ~Menu_Crafting()
  {
  }
  
  void build()
  {
    selectedRecipe=0;
    
    //check inventory grid.
    for (int _y=0;_y<10;++_y)
    {
      for (int _x=0;_x<10;++_x)
      {
        if (inventoryGrid[_x][_y]!=0)
        {
          vItem.push(inventoryGrid[_x][_y]);
          inventoryGrid[_x][_y]->addToRecipeManager();
        }
      }
    }
    vRecipe.clear();
      
    auto vValidRecipe = recipeManager.getValidRecipes();
      // Render the valid recipes.
    for (int i=0;i<vValidRecipe->size();++i)
    {
      vRecipe.push((*vValidRecipe)(i));
    }

  }
  
  void /* GUI_Interface */ render()
  {
    Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
    font8x8.drawText("Available recipes:",panelX1+250,(panelY2-40),panelX1+500,(panelY2-50),false,true);
    
    if (vRecipe.size() > 0)
    {
      Renderer::placeColour4a(180,180,180,255,panelX1+250,(panelY2-50)-(selectedRecipe*10),panelX1+600,(panelY2-50)-((selectedRecipe+1)*10));
    }
    for (int i=0;i<vRecipe.size();++i)
    {
      font8x8.drawText(vRecipe(i)->getName(),panelX1+250,(panelY2-50)-(i*10),panelX1+500,(panelY2-50)-((i+1)*10),false,true);
    }
  }
  
	bool /* GUI_Interface */ mouseEvent (Mouse* _mouse)
	{
    // Scroll up and down interaction select
    if (_mouse->isWheelDown)
    {
      ++selectedRecipe;
      if (selectedRecipe>=vRecipe.size()) {selectedRecipe=0;}
      _mouse->isWheelDown=false;
      _mouse->isWheelUp=false;
    }
    else if (_mouse->isWheelUp)
    {
      --selectedRecipe;
      if (selectedRecipe<0) { selectedRecipe=vRecipe.size()-1; }
      _mouse->isWheelDown=false;
      _mouse->isWheelUp=false;
    }
    // MAKE RECIPE
    if (_mouse->isLeftClick)
    {
      recipeManager.makeRecipe(playerCharacter, selectedRecipe);
      
      _mouse->isLeftClick=false;
      return true;
    }
    
    return false;
  }
  
};


// Finally getting around to breaking the Menu_AdventureMode into some submenus.
// Currently interactions are 1:1. I want an item to have multiple possible interactions with an object.
// For example log->ground could be "build campfire" or "build wall".
// This should also allow us to more easily sort interactions by priority.
class InteractManager: public GUI_Interface
{
  int selectedInteraction;
  Wildcat::Font* font;
  
  struct Interaction /* Stores the description of the interaction and all indexing */
  {
    std::string description;
    int vType; /* 0 = generic, 1 = Item, 2 = Character, 3 = Creature, 4 = terrain */
    int vIndex; /* Index in the vector of the object. */
    int interactID; /* Indicates the subtype of interaction (stab/slash/etc). */
    
    Interaction( std::string _description, int _vType, int _vIndex, int _interactID )
    {
      description=_description;
      vType = _vType;
      vIndex = _vIndex;
      interactID = _interactID;
    }
  };
  
  public:
  
  unsigned long int x,y; /* Tile which the player wants to interact on */
  LocalTile* localSelected;
  
  Item * sourceItem; /* Item the player is using. 0 = unarmed */
  
    // All interactions need to be kept sorted into their object categories.
  Vector <WorldObject*> vGeneric;
    Vector <Item*> vItem;
    Vector <Character*> vCharacter;
    Vector <Creature*> vCreature;
  
  Vector <Interaction*> vInteraction;
  
  //Index lookup.
  
  InteractManager()
  {
    x=ABSOLUTE_COORDINATE_NULL;
    y=ABSOLUTE_COORDINATE_NULL;
    localSelected=0;
    sourceItem=0;
    selectedInteraction = 0;
    
    font = &font8x8;
  }
  
    // GUI INTERFACE
	bool /* GUI_Interface */ mouseEvent (Mouse* _mouse)
	{

      // Scroll up and down interaction select
      if (_mouse->isWheelDown)
      {
        ++selectedInteraction;
        if (selectedInteraction>=vInteraction.size()) {selectedInteraction=0;}
        _mouse->isWheelDown=false;
        _mouse->isWheelUp=false;
      }
      else if (_mouse->isWheelUp)
      {
        --selectedInteraction;
        if (selectedInteraction<0) { selectedInteraction=vInteraction.size()-1; }
        _mouse->isWheelDown=false;
        _mouse->isWheelUp=false;
      }
    
      else if (_mouse->isLeftClick)
      {
        if (sourceItem==0)
        {
          sourceItem = &itemHand;
          itemHand.owner = playerCharacter;
        }
        

        std::cout<<"Using interaction: "<<vInteraction(selectedInteraction)->description<<".\n";
        
        
        Interaction* _interaction = vInteraction(selectedInteraction);
        int selectedVector = _interaction->vType;
        int selectedIndex = _interaction->vIndex;
        int interactionType = _interaction->interactID;
        
        if ( selectedVector == 0 ) /* generic */
        {
          sourceItem->interact(vGeneric(selectedIndex),interactionType);
        }
        else if ( selectedVector == 1 ) /* Item */
        {
          sourceItem->interact(vItem(selectedIndex),interactionType);
        }
        else if ( selectedVector == 2 ) /* Character */
        {
          sourceItem->interact(vCharacter(selectedIndex),interactionType);
        }
        else if ( selectedVector == 3 ) /* Creature */
        {
          sourceItem->interact(vCreature(selectedIndex),interactionType);
        }
        else if ( selectedVector == 4 ) /* Terrain */
        {
          std::cout<<"Terrain interaction\n";
          sourceItem->interact(localSelected,interactionType);
        }
        
        _mouse->isLeftClick=false;
        return true;
      }
    
    return false;
  }
  
  // Take the current tile and copy all objects
  // Return false if there's nothing to interact with.
  bool build(Item* _sourceItem, unsigned long int _x, unsigned long int _y)
  {
    vGeneric.clear();
    vItem.clear();
    vCharacter.clear();
    vCreature.clear();
    vInteraction.deleteAll();
    
    x=_x;
    y=_y;
    localSelected=world(x,y);
    sourceItem = _sourceItem;
    
      std::cout<<"Building interactions for: "<<x<<", "<<y<<".\n";
    if (sourceItem ==0 ) // HAND INTERACTIONS
    {
      // ADD TERRAIN INTERACTION
      vInteraction.push( new Interaction ("Punch ground",4,0,0) );
      
      for (int i=0; i<localSelected->vObjectGeneric.size();++i)
      {
        auto vInteract = itemHand.getInteractNames(localSelected->vObjectGeneric(i));

        if ( vInteract !=0 )
        {
          for (int i2=0;i2<vInteract->size();++i2)
          {
            vGeneric.push(localSelected->vObjectGeneric(i2));
            vInteraction.push( new Interaction ((*vInteract)(i2),0,i,i2) );
          }
        }
      }
      for (int i=0; i<localSelected->vItem.size();++i)
      {
        vGeneric.push(localSelected->vItem(i));
        vInteraction.push( new Interaction ("Punch "+localSelected->vItem(i)->getName(),0,i,0) );
      }
      for (int i=0; i<localSelected->vCharacter.size();++i)
      {
        vGeneric.push(localSelected->vCharacter(i));
        vInteraction.push( new Interaction ("Punch "+localSelected->vCharacter(i)->getName(),0,i,0) );
      }
      for (int i=0; i<localSelected->vCreature.size();++i)
      {
        vGeneric.push(localSelected->vCreature(i));
        vInteraction.push( new Interaction ("Punch "+localSelected->vCreature(i)->getName(),0,i,0) );
      }
      
    }
    else  
    {
      // ADD TERRAIN INTERACTION
        auto vInteractTerrain = sourceItem->getInteractNames(localSelected);
        if ( vInteractTerrain !=0 )
        {
          for (int i2=0;i2<vInteractTerrain->size();++i2)
          {
            vInteraction.push( new Interaction ((*vInteractTerrain)(i2),4,0,i2) );
          }
        }
      
      for (int i=0; i<localSelected->vObjectGeneric.size();++i)
      {
        auto vInteract = sourceItem->getInteractNames(localSelected->vObjectGeneric(i));
        auto target = localSelected->vObjectGeneric(i);

        if ( vInteract !=0 )
        {
          for (int i2=0;i2<vInteract->size();++i2)
          {
            vGeneric.push(localSelected->vObjectGeneric(i2));
            vInteraction.push( new Interaction ((*vInteract)(i2),0,i,i2) );
          }
        }
      }
      for (int i=0; i<localSelected->vItem.size();++i)
      {
        auto vInteract = sourceItem->getInteractNames(localSelected->vItem(i));

        if ( vInteract !=0 )
        {
          for (int i2=0;i2<vInteract->size();++i2)
          {
            vItem.push(localSelected->vItem(i2));
            vInteraction.push( new Interaction ((*vInteract)(i2),1,i,i2) );
          }
        }
      }
      for (int i=0; i<localSelected->vCharacter.size();++i)
      {
        auto vInteract = sourceItem->getInteractNames(localSelected->vCharacter(i));

        if ( vInteract !=0 )
        {
          for (int i2=0;i2<vInteract->size();++i2)
          {
            vCharacter.push(localSelected->vCharacter(i2));
            vInteraction.push( new Interaction ((*vInteract)(i2),2,i,i2) );
          }
        }
      }
      for (int i=0; i<localSelected->vCreature.size();++i)
      {
        auto vInteract = sourceItem->getInteractNames(localSelected->vCreature(i));

        if ( vInteract !=0 )
        {
          for (int i2=0;i2<vInteract->size();++i2)
          {
            vCreature.push(localSelected->vCreature(i2));
            vInteraction.push( new Interaction ((*vInteract)(i2),3,i,i2) );
          }
        }
      }
    }


    return (vInteraction.size() > 0);
  }
  
  void /* GUI_Interface */ render()
  {
    // Render background and selection panels.
    Renderer::placeColour4a(120,120,120,255,panelX1+250,panelY2,panelX1+600,(panelY2)-(vInteraction.size()*10));
    Renderer::placeColour4a(180,180,180,255,panelX1+250,panelY2-(selectedInteraction*10),panelX1+600,panelY2-((selectedInteraction+1)*10));
    
    for (int i=0;i<vInteraction.size();++i)
    {
      font8x8.drawText(vInteraction(i)->description,panelX1+250,(panelY2)-(i*10),panelX1+500,(panelY2)-(i*10)-10,false,true);
          //font8x8.drawText("("+DataTools::toString(useItem->interactTime(localTileSelected->vItem(i)))+" sec)",panelX1+500,(panelY2)-((i+j)*10),panelX1+600,(panelY2)-((i+j)*10)-10,false,true);
    }
  }
  

};
InteractManager interactManager;

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
    // Very basic manual for now. Future manual should have stuff like bestiary, alchemy instructions,
    // crafting instructions, etc.
    GUI_Button buttonManual;
      bool manualActive;
    
      /* Button to view character sheet - CHR */
    GUI_Button buttonCharacterSheet;
    
      /* Button to view craftomg menu - CFT */
    GUI_Button buttonCrafting;
      bool craftingMenuActive;
      Menu_Crafting menuCrafting;



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
      

      int selectedHotbar;
      int hoveredInventoryX, hoveredInventoryY;
      
      //Render item selection
    bool itemSelectionActive;
      unsigned long int tileSelectAbsoluteX, tileSelectAbsoluteY;
      LocalTile* localTileSelected; /* LocalTile that the player last selected */
      int nInteractions; /* number of interactions currently in the menu */
      
      // Vectors of each object on selected tile, broken down by type.
      //Vector <Item*> vSelectedTileItems;
      //Vector <Character*> vSelectedTileCharacter;
      
      
      int selectedItemSlot; /* 0 = interact with terrain */
        bool subItemSelectionActive;
        Item* useItem; /* Item to use on target */
    
      // CTRL is modifier for inventory menu. Will add 1 of item to inventory, or transfer 1 of item to storage. Or
      // drop 1 of item.
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

    buttonCrafting.active = true;
    buttonCrafting.text = "CFT";
    buttonCrafting.font = font;
		buttonCrafting.setColours(&cNormal,&cHighlight,0);
		guiManager.add(&buttonCrafting);
    
    
    craftingMenuActive=false;
    
    itemSelectionActive=false;
    tileSelectAbsoluteX=-1;
    tileSelectAbsoluteY=-1;
    localTileSelected=0;
    selectedItemSlot=0;
    subItemSelectionActive=false;
    useItem=0;
    nInteractions=0;
    
    characterSheetActive=false;
    
    manualActive=false;
    conversationActive = false;
      conversationCharacter=0;
    
    inventoryActive=false;
      clickedItemSlot=-1;
      carriedItem=0;
      hoveredInventoryX=-1;
      hoveredInventoryY=-1;
      
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
    
    if (playerCharacter==0)
    { return; }
  
    /* Render adventure mode interface.
    Yes it is very messy at the moment, however it works fine and is just a GUI
    so it doesn't matter too much. */
    
    
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
    font8x8.drawText("Minimap",panelX1,panelY1,panelX1+220,panelY1+220,true,true);
    
    Renderer::placeColour4a(150,150,150,250,panelX1,panelY1+220,panelX1+220,panelY1+320);
    font8x8.drawText("Action Menu",panelX1,panelY1+220,panelX1+220,panelY1+320,true,true);
    
    font8x8.drawText(DataTools::toString(worldViewer.hoveredXTile) + ", " + DataTools::toString(worldViewer.hoveredYTile),panelX1,panelY1+210,panelX1+220,panelY1+220,false,true);
    font8x8.drawText(DataTools::toString(worldViewer.hoveredXTileLocal) + ", " + DataTools::toString(worldViewer.hoveredYTileLocal),panelX1,panelY1+200,panelX1+220,panelY1+210,false,true);
    font8x8.drawText(DataTools::toString(worldViewer.hoveredAbsoluteX) + ", " + DataTools::toString(worldViewer.hoveredAbsoluteY),panelX1,panelY1+190,panelX1+220,panelY1+200,false,true);
    
    if (playerCharacter->thirst > MAX_THIRST - 100)
    {
      font8x8.drawText("THIRSTY",panelX1+230,panelY1+230,panelX1+400,panelY1+240,false,true,255,0,0);
    }
    if (playerCharacter->hunger > MAX_HUNGER - 100)
    {
      font8x8.drawText("HUNGRY",panelX1+230,panelY1+220,panelX1+400,panelY1+230,false,true,255,0,0);
    }
    if (playerCharacter->isAlive == false)
    {
      font8x8.drawText("DED",panelX1+230,panelY1+210,panelX1+400,panelY1+220,false,true,255,0,0);
    }
    
    //BARS: Health, Stamina, Hunger, Thirst, Energy
    double barWidth = 220;
    double hungerWidth = (MAX_HUNGER-playerCharacter->hunger)*(barWidth/MAX_HUNGER);
    double thirstWidth = (MAX_THIRST-playerCharacter->thirst)*(barWidth/MAX_THIRST);
    
    Renderer::placeColour4a(255,0,0,255,panelX1,panelY1+232,panelX1+220,panelY1+234);
    Renderer::placeColour4a(0,0,255,255,panelX1,panelY1+229,panelX1+thirstWidth,panelY1+231);
    Renderer::placeColour4a(128,64,0,255,panelX1,panelY1+226,panelX1+hungerWidth,panelY1+228);
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
      
      // New system: interactManager
      interactManager.render();

      nInteractions = localTileSelected->vItem.size() + localTileSelected->vCharacter.size() + localTileSelected->vCreature.size() + localTileSelected->vObjectGeneric.size() + 1;

    }
    
    // Render manual page
    if (manualActive)
    {
      Renderer::placeColour4a(150,150,250,250,panelX1+240,panelY1+40,panelX2-20,panelY2-20);
      linesDrawn = font8x8.drawText(ADVENTURE_MODE_MANUAL,panelX1+250,panelY1+45,panelX2-25,panelY2-25,false,false);
    }
    
    // Render crafting menu
    if (craftingMenuActive)
    {
      menuCrafting.render();
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
      
        // Put items on player to interact
      Renderer::placeTexture4(panelX1+350,panelY2-210,panelX2-130,panelY2-20, &TEX_NPC, false);

      hoveredInventoryX=-1;
      hoveredInventoryY=-1;
      
      // render inventory slots
      int inventoryY = panelY2-250;
      int currentFloorItem = 0;
        
      for (int row = 0;row<12;++row)
      { int currentX = panelX1+250;
        for (int i=0;i<10;++i)
        {
          Renderer::placeColour4a(120,120,120,250,currentX,inventoryY,currentX+32,inventoryY-32);
          
          if (mouseX >= currentX && mouseX <= currentX+32
              && mouseY >=inventoryY-32 && mouseY <= inventoryY)
              {
                hoveredInventoryX = i;
                hoveredInventoryY = row;
              }
          
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
      
      // Render text
      // We render the stats for 2 items: The item the player has selected,
      // and the item the player is hovering over. This allow easy comparison
      // of 2 items.
      std::string carriedItemStats = "";
      if ( carriedItem )
      {
        carriedItemStats = carriedItem->getName();
      }
      std::string hoveredItemStats = "";
      if ( hoveredInventoryX >= 0 && hoveredInventoryX < 10
       && hoveredInventoryY >= 0 && hoveredInventoryY < 10)
      {
        if (inventoryGrid[hoveredInventoryX][hoveredInventoryY] )
        {
        hoveredItemStats = inventoryGrid[hoveredInventoryX][hoveredInventoryY]->getName();
        }

      }
      
      font8x8.drawText("Inventory", panelX1 + 250,panelY2-230,panelX1 + 450,panelY2-250,true,true);
      font8x8.drawText("Storage", panelX1 + 650,panelY2-230,panelX1 + 850,panelY2-250,true,true);
      font8x8.drawText("Carried stats:\n "+carriedItemStats, panelX1+350,panelY2-210,panelX1+(panelNX/2) ,panelY2-25,false,false);
      font8x8.drawText("Hovered stats\n "+hoveredItemStats, panelX1+(panelNX/2),panelY2-210,panelX2-130,panelY2-25,false,false);

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
      craftingMenuActive=false;
      worldViewer.showHoveredTile = false;
			_keyboard->keyUp(Keyboard::ESCAPE);	
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
      if ( selectedHotbar >= 0 && selectedHotbar < 10 )
      {
        useItem = inventoryGrid[selectedHotbar][0];
      }
      
      worldViewer.showHoveredTile = itemSelectionActive;
      localTileSelected=0;
      subItemSelectionActive=false;
      selectedItemSlot=0;

      _keyboard->keyUp(Keyboard::E);
      _keyboard->keyUp(Keyboard::e);
    }
    
      // Disable keyboard if we're in a menu.
    if (itemSelectionActive || subItemSelectionActive || manualActive || conversationActive || inventoryActive)
    {
      _keyboard->clearAll();
      return true;
    }
    
      // TAB will switch between adventure mode and god mode.
      // Make sure to relinquish control of character.
		if(_keyboard->isPressed(Keyboard::TAB))
		{
      playerCharacter=0;
      
      
      //menuWorldSimulator.active = true;
      
      
      activeMenu = MENU_WORLDSIMULATOR;
      
      _keyboard->keyUp(Keyboard::TAB);
		}
    
    if(_keyboard->isPressed(Keyboard::RIGHT) || _keyboard->isPressed(Keyboard::D) || _keyboard->isPressed(Keyboard::d))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        if ( playerCharacter->isAlive )
        {
        
        wl->moveObject(playerCharacter,playerCharacter->x+1,playerCharacter->y);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
        }
        else
        {
          Console ("BOI U DED, U AINT GOIN NOWHERE");
        }
        
      }
      
      _keyboard->keyUp(Keyboard::RIGHT);
      _keyboard->keyUp(Keyboard::D);
      _keyboard->keyUp(Keyboard::d);
      RENDER_NEXT_FRAME=true;
    }
    if(_keyboard->isPressed(Keyboard::LEFT) || _keyboard->isPressed(Keyboard::A) || _keyboard->isPressed(Keyboard::a))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        if (playerCharacter->isAlive)
        {
          wl->moveObject(playerCharacter,playerCharacter->x-1,playerCharacter->y);
          worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
          world.updateMaps();
          playerCharacter->updateKnowledge();
          
          if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
          else { world.incrementTicksBacklog(1); }
        }
        else
        {
          Console ("BOI U DED, U AINT GOIN NOWHERE");
        }
      }
      
      _keyboard->keyUp(Keyboard::LEFT);
      _keyboard->keyUp(Keyboard::A);
      _keyboard->keyUp(Keyboard::a);
      RENDER_NEXT_FRAME=true;
    }
    if(_keyboard->isPressed(Keyboard::UP) || _keyboard->isPressed(Keyboard::W) || _keyboard->isPressed(Keyboard::w))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        if (playerCharacter->isAlive)
        {
        wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y+1);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
        }
        else
        {
          Console ("BOI U DED, U AINT GOIN NOWHERE");
        }
      }
      _keyboard->keyUp(Keyboard::UP);
      _keyboard->keyUp(Keyboard::W);
      _keyboard->keyUp(Keyboard::w);
      RENDER_NEXT_FRAME=true;
    }
    if(_keyboard->isPressed(Keyboard::DOWN) || _keyboard->isPressed(Keyboard::S) || _keyboard->isPressed(Keyboard::s))
    {
      World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
      if ( wl != 0 )
      {
        if (playerCharacter->isAlive)
        {
        wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y-1);
        worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
        world.updateMaps();
        playerCharacter->updateKnowledge();
        
        if (playerCharacter->isSneaking ) { world.incrementTicksBacklog(2); }
        else { world.incrementTicksBacklog(1); }
        }
        else
        {
          Console ("BOI U DED, U AINT GOIN NOWHERE");
        }
      }
      _keyboard->keyUp(Keyboard::DOWN);
      _keyboard->keyUp(Keyboard::S);
      _keyboard->keyUp(Keyboard::s);
      RENDER_NEXT_FRAME=true;
    }
      // PERIOD = WAIT
    if(_keyboard->isPressed(Keyboard::PERIOD))
    {
      world.incrementTicksBacklog(1);
      _keyboard->keyUp(Keyboard::PERIOD);
      RENDER_NEXT_FRAME=true;
    }
    
      // SHIFT+, and SHIFT+.
      // Move view up or down
      // Basically just toggles subterranean mode right now.
    if(_keyboard->isPressed(Keyboard::LEFT_CHEVRON))
    {
      if (playerCharacter)
      {
        World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
        if (world(playerCharacter->fullX,playerCharacter->fullY)->isCave)
        {
          if (wl->moveDown(playerCharacter))
          {
            worldViewer.subterraneanMode=true;
          }
          else
          {
            Console("There's no way down here");
          }
        }

      }
      _keyboard->keyUp(Keyboard::LEFT_CHEVRON);
    }
      // Move view up or down
    if(_keyboard->isPressed(Keyboard::RIGHT_CHEVRON))
    {
      if (playerCharacter)
      {
        World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
        if (world(playerCharacter->fullX,playerCharacter->fullY)->isCave)
        {
          if (wl->moveUp(playerCharacter))
          {
            worldViewer.subterraneanMode=false;
          }
          else
          {
            Console("There's no way up here");
          }
        }

      }
      _keyboard->keyUp(Keyboard::RIGHT_CHEVRON);
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
    
		guiManager.keyboardEvent(_keyboard);
		worldViewer.keyboardEvent(_keyboard);
		return false;
	}
  
	bool mouseEvent (Mouse* _mouse)
	{
    
    if (playerCharacter==0)
    { return false; }
    
    //int itemClicked = 0; /* 0-99 == Inventory. 100-119 == Ground */
    mouseX = _mouse->x;
    mouseY = _mouse->y;
    
    if (craftingMenuActive)
    {
      menuCrafting.mouseEvent(_mouse);
    }
    
    if (itemSelectionActive)
    {
      // Right click to cancel out of item selection.
      if (subItemSelectionActive==false && _mouse->isRightClick)
      {
        itemSelectionActive=false;
        localTileSelected=0;
        worldViewer.showHoveredTile = false;
      }
      // Right click to cancel out of interaction selection.
      else if (subItemSelectionActive && (_mouse->isRightClick || localTileSelected == 0) )
      {
        subItemSelectionActive=false;
        _mouse->isRightClick=false;
        _mouse->isLeftClick=false;
        localTileSelected=0;
      }
      else if (subItemSelectionActive)
      {
        if(interactManager.mouseEvent(_mouse))
        {
          // Reset the interact menu.
          subItemSelectionActive=false;
          itemSelectionActive=false;
          worldViewer.showHoveredTile = false;
          localTileSelected=0;
          selectedItemSlot=0;
        }
      }
      else if (subItemSelectionActive==false && _mouse->isLeftClick)
      {
        tileSelectAbsoluteX=worldViewer.hoveredAbsoluteX;
        tileSelectAbsoluteY=worldViewer.hoveredAbsoluteY;
        
        if (world.isSafe(tileSelectAbsoluteX,tileSelectAbsoluteY))
        {
          if (interactManager.build(useItem, tileSelectAbsoluteX,tileSelectAbsoluteY))
          {
            localTileSelected=world(tileSelectAbsoluteX,tileSelectAbsoluteY);
            subItemSelectionActive=true;
            //interactManager.clear();

            selectedItemSlot=0;
          }
          else
          {
            if ( useItem != 0 )
            {
              Console("Nothing to use the "+useItem->getName()+" on here.");
            }
            else
            {
              Console("Nothing to use here.");
            }
            
          }
        }
        _mouse->isLeftClick=false;
      }
      


    }
    
    // Player can use CTRL+scroll to scroll the hotbar.
    // Exit from interaction menu because otherwise I don't feel like doing it the proper way right now.
		if(_mouse->isWheelDown && _mouse->ctrlPressed)
		{
      ++selectedHotbar;
      if (selectedHotbar>9) {selectedHotbar=0;}
			_mouse->isWheelDown=false;
			_mouse->isWheelUp=false;
      
      // Reset the interact menu.
      subItemSelectionActive=false;
      itemSelectionActive=false;
      worldViewer.showHoveredTile = false;
      localTileSelected=0;
      selectedItemSlot=0;
      
		}
		if(_mouse->isWheelUp && _mouse->ctrlPressed)
		{
      --selectedHotbar;
      if (selectedHotbar<0) {selectedHotbar=9;}
			_mouse->isWheelDown=false;
			_mouse->isWheelUp=false;
      
      // Reset the interact menu.
      subItemSelectionActive=false;
      itemSelectionActive=false;
      worldViewer.showHoveredTile = false;
      localTileSelected=0;
      selectedItemSlot=0;
		}
    

    
    if (inventoryActive)
    {
      if (_mouse->isLeftClick)
      {
        // render inventory slots
        int inventoryY = panelY2-250;
        int currentFloorItem = 0;
        
        for (int row = 0;row<12;++row)
        {
          int currentX = panelX1+250;
          
          for (int i=0;i<10;++i)
          {
            if (_mouse->x >= currentX && _mouse->x <= currentX+32
              && _mouse->y >=inventoryY-32 && _mouse->y <= inventoryY)
            {
              clickedItemSlot = row*10 + i;
              
              if (clickedItemSlot < 100) // INVENTORY GRID
              {
                if ( carriedItem != 0 ) { carriedItem->owner = playerCharacter; }
                
                //Swap carried item and inventory grid item.
                Item * tempItem = carriedItem;
                carriedItem = inventoryGrid[i][row];
                if ( carriedItem != 0 ) { carriedItem->owner = playerCharacter; }
                inventoryGrid[i][row]=tempItem;
                _mouse->isLeftClick=false;
              }
              else if ( clickedItemSlot-100 >= 0) // PICKING ITEM OFF GROUND
              {
                if (_mouse->ctrlPressed)
                {
                  if (clickedItemSlot-100 < vItemsOnFloor.size() ) // If player clicked on full ground slot.
                  {
                    if ( carriedItem != 0 ) { carriedItem->owner = playerCharacter; }
                    Item* tempItem = carriedItem;
                    carriedItem = vItemsOnFloor(clickedItemSlot-100);
                    if ( carriedItem != 0 ) { carriedItem->owner = playerCharacter; }
                    vItemsOnFloor.removeSlot(clickedItemSlot-100);
                    _mouse->isLeftClick=false;
                    World_Local* wl = world(playerCharacter->worldX,playerCharacter->worldY);
                    if ( wl != 0 )
                    {
                      wl->remove(carriedItem);
                      // Put down carried item if necessary
                      if (tempItem != 0)
                      {
                        // Put item on map.
                        wl->put(tempItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(tempItem);
                        tempItem->owner = 0;
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
                        wl->put(carriedItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(carriedItem);
                        vItemsOnFloor.push(carriedItem);
                        carriedItem->owner = 0;
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
                      wl->remove(carriedItem);
                      // Put down carried item if necessary
                      if (tempItem != 0)
                      {
                        // Put item on map.
                        wl->put(tempItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(tempItem);
                        tempItem->owner=0;
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
                        wl->put(carriedItem,playerCharacter->x,playerCharacter->y);
                        wl->vItem.push(carriedItem);
                        vItemsOnFloor.push(carriedItem);
                        carriedItem->owner=0;
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
    
      // Default action for selected item. Check to make sure we are clicking somewhere valid.
    if (_mouse->isLeftClick && selectedHotbar >= 0 && selectedHotbar < 10 && worldViewer.hoveredAbsoluteX != ABSOLUTE_COORDINATE_NULL && worldViewer.hoveredAbsoluteY != ABSOLUTE_COORDINATE_NULL
     && _mouse->x > panelX1+220 )
    {
      
      if (inventoryGrid[selectedHotbar][0] != 0)
      {
        std::cout<<"DEFAULT USE ITEM: "<<inventoryGrid[selectedHotbar][0]->getName()<<".\n";
      }
      else
      {
        std::cout<<"Default hand action\n";
      }
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
    
      // Toggle the crafting menu.
		if (buttonCrafting.clicked==true)
		{
      std::cout<<"CRAFTING\n";
      craftingMenuActive = !craftingMenuActive;
      menuCrafting.build();
			buttonCrafting.unclick();
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
		buttonCrafting.setPanel(panelX1+165,panelY1+304,panelX1+197,panelY1+320);
    
		worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);

    interactManager.setPanel(panelX1,panelY1,panelX2,panelY2);
    menuCrafting.setPanel(panelX1,panelY1,panelX2,panelY2);

	}
	
  
};

#endif
