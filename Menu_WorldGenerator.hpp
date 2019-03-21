#pragma once
#ifndef WORLDSIM_MENU_WORLD_GENERATOR_HPP
#define WORLDSIM_MENU_WORLD_GENERATOR_HPP

/* WorldSim: Menu_WorldGenerator.hpp
	#include"Menu_WorldGenerator.hpp"
	
	Implementation of GUI interface.
	This menu is displayed upon starting the game app. It allows the player to start a new game, load a saved game, access the options menu, and quit the application.

*/

#include <GUI/GUI.hpp>

#include <Device/Display/DisplayInterface.hpp>
#include <Device/Mouse/MouseInterface.hpp>
#include <Debug/DebugMacro.hpp>

#include <Data/DataTools.hpp> /* To check if seed is numeric */


#include "Menu_AdventureMode.hpp"


class Menu_WorldGenerator: public GUI_Interface
{
	private:
    GUI_Manager guiManager;
    
    /* Colours / theme. */
    Colour cNormal;
    Colour cSelected;
    Colour cDropPanel;
    Colour cHighlight;

    /* Background image */
    Texture* backgroundTexture;
    /* Button: Back to main menu. */
    GUI_Button buttonBack;
    /* Button: Expand preview window. */
    GUI_Button buttonExpandPreviewWindow;
    /* Textbox: Title for menu. */
    GUI_TextBox textMenuTitle;
    /* TextEntry: Name of the world. */
    GUI_TextEntry textEntryWorldName;
    /* TextEntry: Full seed value. */
    GUI_TextEntry textEntryFullSeed;
    /* TextEntry: Full biome value. */
    GUI_TextEntry textEntryFullBiome;
    /* Button: Generate the world. */
    GUI_Button buttonGenerate;
    /* Button: Keep this world and move to world simulation phase. */
    GUI_Button buttonSimWorld;
    /* Button: Export a bunch of data about the generated world. */
    GUI_Button buttonExportData;
    /* Button: Load a world. Note this only loads starting data. Which is geographic data and starting positions. */
    GUI_Button buttonLoadWorld;
    /* Number entry: World size (tiles). The world is square, so only one input is needed. */
    GUI_CycleButton worldSize;
    /* Textbox: Caption for worldsize selector. */
    GUI_TextBox textWorldSize;
    /* Number entry: World size (tiles). The world is square, so only one input is needed. */
    GUI_CycleButton freeSteps;
    /* Textbox: Caption for worldsize selector. */
    GUI_TextBox textFreeSteps;
    /* WrapX */
    GUI_CycleButton guiWrapX;
    GUI_TextBox textWrapX;
    /* WrapY */
    GUI_CycleButton guiWrapY;
    GUI_TextBox textWrapY;
    /* Island Mode */
    GUI_CycleButton guiIslandMode;
    GUI_TextBox textIslandMode;
    /* Ocean percent */
    GUI_NumberInput nLandPercent;
    GUI_TextBox textOceanPercent;
    /* Number entry: Number of civs to spawn into the world. */
    GUI_NumberInput nCiv;
    GUI_TextBox guiCaptionNCiv;
    /* Number entry: Number of human tribes to spawn into the world. */
    GUI_NumberInput nTribe;
    GUI_TextBox guiCaptionNTribe;
    /* Number entry: Number of Dwarven tribes to spawn into the world. */
    GUI_NumberInput nTribeDwarven;
    GUI_TextBox guiCaptionNTribeDwarven;
    /* Number entry: Number of Elven tribes to spawn into the world. */
    GUI_NumberInput nTribeElven;
    GUI_TextBox guiCaptionNTribeElven;
    
    bool fullScreenPreview;
	

	public:
	
	Menu_WorldGenerator()
	{
		fullScreenPreview=false;
		worldViewer.world = &world;
		worldViewer.active = false;
		buttonGenerate.clicked=false;
	}
	
	void setFont(Wildcat::Font* _font)
	{
		font = _font;
		guiManager.setFont(_font);
	}
	
	void logicTick()
	{
		// if(menuWorldSimulator.active==true)
		// {
			// menuWorldSimulator.logicTick();
		// }
	}
	
	void init()
	{
		backgroundTexture=&TEX_NEW_GAME_BACKGROUND;
		active=false;
		fullScreenPreview=true;
	
		/* Initialise theme. */
		cNormal.set(200,200,200);
		cSelected.set(180,180,180);
		cDropPanel.set(170,170,170);
		cHighlight.set(170,170,170);
		
		buttonExpandPreviewWindow.setPanel(0,0,32,32);
		
			if(fullScreenPreview==false)
			{
			
				int panelSizeX = panelX2-panelX1;
				int panelSizeY = panelY2-panelY1;
				
				int worldPreviewX1=panelX1+(panelSizeX/2);
				int worldPreviewY1=panelY1+(panelSizeY/2);
		
				//worldViewer.setPanel(worldPreviewX1,worldPreviewY1,panelX2,panelY2);
				//buttonExpandPreviewWindow.setPanel(worldPreviewX1,worldPreviewY1,worldPreviewX1+16,worldPreviewY1+16);
				
					/* Put worldviewer in top right corner of panel. */
				worldViewer.setPanel(panelX2/2,panelY2/2,panelX2,panelY2);
			}
			else
			{
				//worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
        //buttonExpandPreviewWindow.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
				
					/* Make worldviewer fill screen. */
				worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
			}

		buttonBack.text="Back";
		buttonBack.setColours(&cNormal,&cHighlight,0);
		
		/* WORLD SIZE */
		worldSize.currentOption=DEFAULT_WORLD_SIZE_SLOT;
		//worldSize.addOption("17");
		worldSize.addOption("33");
		worldSize.addOption("65");
		worldSize.addOption("129");
		worldSize.addOption("257");
		worldSize.addOption("513");
		worldSize.addOption("1025");
		worldSize.addOption("2049");
		worldSize.addOption("4097");
		worldSize.addOption("8193");
		worldSize.texCycleButton=&TEX_GUI_DECREMENT;
		worldSize.setColours(&cNormal,&cHighlight,0);
		
		freeSteps.currentOption=0;
		freeSteps.addOption("0");
		freeSteps.addOption("1");
		freeSteps.addOption("2");
		freeSteps.addOption("3");
		freeSteps.addOption("4");
		freeSteps.addOption("5");
		freeSteps.addOption("6");
		freeSteps.addOption("7");
		freeSteps.addOption("8");
		freeSteps.addOption("9");
		freeSteps.addOption("10");
		freeSteps.addOption("11");
		freeSteps.addOption("12");
		freeSteps.texCycleButton=&TEX_GUI_DECREMENT;
		freeSteps.setColours(&cNormal,&cHighlight,0);

		/* WrapX */
		guiWrapX.currentOption=0;
		guiWrapX.addOption("Yes");
		guiWrapX.addOption("No");
		guiWrapX.texCycleButton=&TEX_GUI_DECREMENT;
		guiWrapX.setColours(&cNormal,&cHighlight,0);
		guiWrapX.wrap=true;
		
		/* WrapY */
		guiWrapY.currentOption=1;
		guiWrapY.addOption("Yes");
		guiWrapY.addOption("No");
		guiWrapY.texCycleButton=&TEX_GUI_DECREMENT;
		guiWrapY.setColours(&cNormal,&cHighlight,0);
		guiWrapY.wrap=true;
		
		/* Island mode */
		guiIslandMode.currentOption=0;
		guiIslandMode.addOption("Yes");
		guiIslandMode.addOption("No");
		guiIslandMode.texCycleButton=&TEX_GUI_DECREMENT;
		guiIslandMode.setColours(&cNormal,&cHighlight,0);
		guiIslandMode.wrap=true;
		
		/* N CIV */
		nCiv.setNumbers(0,128,DEFAULT_NUMBER_CIVS);
		nCiv.setColours(&cNormal,&cHighlight,0);
		nCiv.texIncrement = &TEX_GUI_INCREMENT;
		nCiv.texDecrement = &TEX_GUI_DECREMENT;
		guiCaptionNCiv.text="# civs:";
		guiCaptionNCiv.setColours(&cNormal);
		guiCaptionNCiv.centeredY=true;
		
		/* Ocean percent */
		nLandPercent.setNumbers(0,100,66);
		nLandPercent.setColours(&cNormal,&cHighlight,0);
		nLandPercent.texIncrement = &TEX_GUI_INCREMENT;
		nLandPercent.texDecrement = &TEX_GUI_DECREMENT;
		textOceanPercent.text="Land percent:";
		textOceanPercent.setColours(&cNormal);
		textOceanPercent.centeredY=true;
		
		/* N TRIBE */
		nTribe.setNumbers(0,100,DEFAULT_NUMBER_TRIBES_HUMAN);
		nTribe.setColours(&cNormal,&cHighlight,0);
		nTribe.texIncrement = &TEX_GUI_INCREMENT;
		nTribe.texDecrement = &TEX_GUI_DECREMENT;
		guiCaptionNTribe.text="# tribes:";
		guiCaptionNTribe.setColours(&cNormal);
		guiCaptionNTribe.centeredY=true;
		
		/* N TRIBE DWARVEN */
		nTribeDwarven.setNumbers(0,100,DEFAULT_NUMBER_TRIBES_DWARVEN);
		nTribeDwarven.setColours(&cNormal,&cHighlight,0);
		nTribeDwarven.texIncrement = &TEX_GUI_INCREMENT;
		nTribeDwarven.texDecrement = &TEX_GUI_DECREMENT;
		guiCaptionNTribeDwarven.text="# Dwarven tribes:";
		guiCaptionNTribeDwarven.setColours(&cNormal);
		guiCaptionNTribeDwarven.centeredY=true;
		
		/* N TRIBE ELVEN */
		nTribeElven.setNumbers(0,100,DEFAULT_NUMBER_TRIBES_ELVEN);
		nTribeElven.setColours(&cNormal,&cHighlight,0);
		nTribeElven.texIncrement = &TEX_GUI_INCREMENT;
		nTribeElven.texDecrement = &TEX_GUI_DECREMENT;
		guiCaptionNTribeElven.text="# Elven tribes:";
		guiCaptionNTribeElven.setColours(&cNormal);
		guiCaptionNTribeElven.centeredY=true;
		
		/* GENERATE BUTTON */
		buttonGenerate.text="1. Generate";
		buttonGenerate.setColours(&cNormal,&cHighlight,0);
		
		/* WORLD NAME */
		textEntryWorldName.setColours(&cNormal,&cHighlight);
		textEntryWorldName.fieldName="World name:";
		textEntryWorldName.characterLimit=20;
			/* Give the world a random name */
		//textEntryWorldName.input = globalNameGen.generateName();
		textEntryWorldName.input = "testworld";
		
		/* WORLD SEED */
		textEntryFullSeed.setColours(&cNormal,&cHighlight);
		textEntryFullSeed.fieldName="World seed:";
		textEntryFullSeed.characterLimit=16	;
		//textEntryFullSeed.input="1902830183";
		textEntryFullSeed.input="";
		
		/* BIOME SEED */
		textEntryFullBiome.setColours(&cNormal,&cHighlight);
		textEntryFullBiome.fieldName="Biome seed:";
		textEntryFullBiome.characterLimit=16;
		textEntryFullBiome.input="";
		
		textWorldSize.text="World size:";
		textWorldSize.setColours(&cNormal);
		
		textFreeSteps.text="Fragmentation:";
		textFreeSteps.setColours(&cNormal);
		
		textWrapX.text="Wrap X:";
		textWrapX.setColours(&cNormal);
		
		textWrapY.text="Wrap Y:";
		textWrapY.setColours(&cNormal);
		
		textIslandMode.text="Island mode:";
		textIslandMode.setColours(&cNormal);
		
		textMenuTitle.text="World Generator";
		textMenuTitle.setColours(&cNormal);
		
		textMenuTitle.centeredX=true;
		textMenuTitle.centeredY=true;
		
		buttonSimWorld.text="2. Simulate World";
		buttonSimWorld.setColours(&cNormal,&cHighlight,0);
		
		buttonExportData.text="Export World Data (N/A)";
		buttonExportData.setColours(&cNormal,&cHighlight,0);
		
		buttonLoadWorld.text="Load world (N/A)";
		buttonLoadWorld.setColours(&cNormal,&cHighlight,0);
		
		buttonExpandPreviewWindow.texture = &TEX_GUI_EXPAND;
		
		guiManager.add(&textMenuTitle);
    //Not currently functional
    //guiManager.add(&buttonBack);
		guiManager.add(&textEntryWorldName);
		guiManager.add(&textEntryFullSeed);
		guiManager.add(&textEntryFullBiome);
		guiManager.add(&buttonGenerate);
		guiManager.add(&textWorldSize);
		guiManager.add(&textFreeSteps);
		guiManager.add(&textIslandMode);
		guiManager.add(&textWrapX);
		guiManager.add(&textWrapY);
		guiManager.add(&worldSize);
		guiManager.add(&freeSteps);
		guiManager.add(&guiIslandMode);
		guiManager.add(&guiWrapX);
		guiManager.add(&guiWrapY);
		guiManager.add(&guiCaptionNCiv);
		guiManager.add(&textOceanPercent);
		guiManager.add(&nCiv);
		guiManager.add(&nLandPercent);
		guiManager.add(&guiCaptionNTribe);
		guiManager.add(&guiCaptionNTribeDwarven);
		guiManager.add(&guiCaptionNTribeElven);
		guiManager.add(&nTribe);
		guiManager.add(&nTribeDwarven);
		guiManager.add(&nTribeElven);
		guiManager.add(&buttonSimWorld);
		guiManager.add(&buttonExportData);
		guiManager.add(&buttonLoadWorld);
		//guiManager.add(&buttonExpandPreviewWindow);
    
    // A TEMPORARY FIX TO STOP ACCIDENTALLY HITTING
    // HOTKEYS WHILE TYPING.
    globalGuiManager.add(&textEntryWorldName);
    globalGuiManager.add(&textEntryFullSeed);
    globalGuiManager.add(&textEntryFullBiome);
		
		guiManager.setFont(font);

	}
	
	void eventResize()
	{
			/* Menu title. */
		textMenuTitle.setPanel(panelX1, panelY2, panelX2/2, panelY2-20);
	
		/* Bottom left corner. */
		buttonBack.setPanel(panelX1,panelY1+20,panelX1+120,panelY1);
		/* Left column, centered. */
		const int leftColumnCenterX = panelX1+(panelNX/4);
		textEntryWorldName.setPanel(panelX1+20, panelY2-30, (panelX2/2)-20, panelY2-40);
		
		textEntryFullSeed.setPanel(panelX1+20, panelY2-40, (panelX2/2)-20, panelY2-50);
		
		textEntryFullBiome.setPanel(panelX1+20, panelY2-50, (panelX2/2)-20, panelY2-60);
		
		int ySpacing = 2;
		int ySize = 10;
		
		int currentY1 = panelY2 - 60;
		int currentY2 = currentY1-ySize;
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		textWorldSize.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		worldSize.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		textFreeSteps.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		freeSteps.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		textIslandMode.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		guiIslandMode.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		textWrapX.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		guiWrapX.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);

		textWrapY.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		guiWrapY.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);

		textOceanPercent.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		nLandPercent.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);

		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		nCiv.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		guiCaptionNCiv.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX-20, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		nTribe.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		guiCaptionNTribe.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX-20, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
	
		nTribeDwarven.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		guiCaptionNTribeDwarven.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		nTribeElven.setPanel(leftColumnCenterX-20, currentY1, leftColumnCenterX+120, currentY2);
		guiCaptionNTribeElven.setPanel(leftColumnCenterX-180, currentY1, leftColumnCenterX-20, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		buttonGenerate.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		buttonSimWorld.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		buttonExportData.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX+120, currentY2);
		
		currentY1 -= (ySize+ySpacing);
		currentY2 -= (ySize+ySpacing);
		
		buttonLoadWorld.setPanel(leftColumnCenterX-120, currentY1, leftColumnCenterX+120, currentY2);
		
		buttonExpandPreviewWindow.setPanel(0,0,32,32);
		
			
		if(fullScreenPreview==false)
		{
			//std::cout<<"Preview mode.\n";
			int panelSizeX = panelX2-panelX1;
			int panelSizeY = panelY2-panelY1;
			
			int worldPreviewX1=panelX1+(panelSizeX/2);
			int worldPreviewY1=panelY1+(panelSizeY/2);
	
			//worldViewer.setPanel(worldPreviewX1,worldPreviewY1,panelX2,panelY2);
			//buttonExpandPreviewWindow.setPanel(worldPreviewX1,worldPreviewY1,worldPreviewX1+16,worldPreviewY1+16);
			
				/* Put worldviewer in top right corner of panel. */
			//worldViewer.setPanel(panelX2/2,panelY2/2,panelX2,panelY2);
			worldViewer.setPanel(panelX2/2,panelY1	,panelX2,panelY2);
		}
		else
		{
			//std::cout<<"Fullscreen preview.\n";
			//worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
			//buttonExpandPreviewWindow.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
				/* Make worldviewer fill screen. */
			worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
		}
	}
	
	void render()
	{
   
    //This is a quickstart option to quickly generate a world.
    // For some reason generating a world too early can cause a crash on
    // subsequent generate calls.
    // Moving the quickstart to the first render call seems to resolve this.
    if ( QUICKSTART_GENERATOR )
		{
      eventGenerate();
      QUICKSTART_GENERATOR=false;
		}
		
    // if ( textEntryWorldName.selected == true )
    // { HOTKEYS_ENABLED=false; }
    // else
    // { HOTKEYS_ENABLED=true; }
    

    /* World preview takes top-right quarter of screen. */
    if(fullScreenPreview==false)
    {
      int panelSizeX = panelX2-panelX1;
      int panelSizeY = panelY2-panelY1;
      
      int worldPreviewX1=panelX1+(panelSizeX/2);
      //int worldPreviewY1=panelY1+(panelSizeY/2);
      int worldPreviewY1=panelY1;
    
    
        /* Background image. */
      //Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
      //Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,&TEX_WORLD_UNIT_NOMAD_01,true);
      //Renderer::placeColour4(0,0,0,worldPreviewX1,worldPreviewY1,panelX2,panelY2);

      //worldViewer.setPanel(worldPreviewX1,worldPreviewY1,panelX2,panelY2);
      //buttonExpandPreviewWindow.setPanel(worldPreviewX1,worldPreviewY1,worldPreviewX1+16,worldPreviewY1+16);
      

      
    }
    else
    {
      //Renderer::placeColour4(0,0,0,panelX1,panelY1,panelX2,panelY2);
      //worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
      //buttonExpandPreviewWindow.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);

    }
    Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
    //Renderer::placeTexture4(panelX1,panelY1,panelX2,panelY2,backgroundTexture,true);
    worldViewer.render();
    /* Render GUI controls. */
    guiManager.render();
				

	}
	
	bool keyboardEvent(Keyboard* _keyboard)
	{
    if ( guiManager.keyboardEvent(_keyboard) )
    {
      return true;
    }
    
    if(_keyboard->isPressed(Keyboard::ONE)) /* Flush console. */
    {
      eventGenerate();
      _keyboard->unpress(Keyboard::ONE);
      return true;
    }
    
    if(_keyboard->isPressed(Keyboard::TWO)) /* Flush console. */
    {
      _keyboard->unpress(Keyboard::TWO);
      if (world.generated == true)
      {
        std::cout<<"Simulate world.\n";
        activeMenu = MENU_WORLDSIMULATOR;
        world.active=true;
        //active=false;
      }
      else
      {
        std::cout<<"You must first generate a world.\n";
      }
		}
		
		return false;
		
		// //textEntryWorldName.keyboardEvent(_keyboard);

		// else if(guiManager.keyboardEvent(_keyboard)==true)
		// {
			// std::cout<<"worldgen got kb\n";
			// return true;
			// //textEntryWorldName.keyboardEvent(_keyboard);
		// }
		// else
		// {
			// return worldViewer.keyboardEvent(_keyboard);
		// }
		// return false;
	}
	
	bool mouseEvent (Mouse* _mouse)
	{

    worldViewer.mouseEvent(_mouse);
    //buttonExpandMap.mouseEvent(_mouse);

    /* If the guiManager did something with the mouse event. */
    if(guiManager.mouseEvent(_mouse)==true)
    {
    
      if(buttonBack.clicked==true)
      {
        buttonBack.unclick();
        //std::cout<<"Back.\n";
        active=false;
      }
      if(buttonGenerate.clicked==true)
      {
        buttonGenerate.unclick();
        eventGenerate();
        buttonGenerate.unclick();
      }
      if(buttonExpandPreviewWindow.clicked==true)
      {
        fullScreenPreview=!fullScreenPreview;
        /* World preview takes top-right quarter of screen. */
        if(fullScreenPreview==false)
        {
        
          /* Shrink the preview window. Restore GUI. */
        
          int panelSizeX = panelX2-panelX1;
          int panelSizeY = panelY2-panelY1;
          
          int worldPreviewX1=panelX1+(panelSizeX/2);
          int worldPreviewY1=panelY1+(panelSizeY/2);
      
          //worldViewer.setPanel(worldPreviewX1,worldPreviewY1,panelX2,panelY2);
          //buttonExpandPreviewWindow.setPanel(worldPreviewX1,worldPreviewY1,worldPreviewX1+16,worldPreviewY1+16);
          
          buttonSimWorld.active=true;
          buttonExportData.active=true;
          buttonLoadWorld.active=true;
          buttonGenerate.active=true;
          textEntryWorldName.active=true;
          textEntryFullSeed.active=true;
          textEntryFullBiome.active=true;
          textWorldSize.active=true;
          textFreeSteps.active=true;
          textIslandMode.active=true;
          textWrapX.active=true;
          textWrapY.active=true;
            //Not currently functional
            //buttonBack.active=true;
            buttonBack.active=false;
          worldSize.active=true;
          freeSteps.active=true;
          guiWrapX.active=true;
          guiWrapY.active=true;
          guiIslandMode.active=true;
          
          
          
          worldViewer.setPanel(panelX2/2,panelY2/2,panelX2,panelY2);
        }
        else
        {
          /* Expand the preview window. Hide GUI. */
        
          //worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
          //buttonExpandPreviewWindow.setPanel(panelX1,panelY1,panelX1+16,panelY1+16);
          worldViewer.setPanel(panelX1,panelY1,panelX2,panelY2);
          
          //buttonSimWorld.active=true;
          //buttonGenerate.active=true;
          //textEntryWorldName.active=true;
          //textWorldSize.active=true;
          //buttonBack.active=true;
          //worldSize.active=true;
          
        }
        buttonExpandPreviewWindow.unclick();
      }
      if(buttonSimWorld.clicked==true)
      {
        buttonSimWorld.unclick();
        if (world.generated == true)
        {
          activeMenu = MENU_WORLDSIMULATOR;
          world.active=true;
        }
        else
        {
          std::cout<<"You must first generate a world.\n";
        }
        
      }
      if ( buttonExportData.clicked==true)
      {
        std::cout<<"Export data\n";
        world.save();
        buttonExportData.unclick();
      }
      
      if ( buttonLoadWorld.clicked==true)
      {
        buttonLoadWorld.unclick();
        //std::cout<<"Load world data.\n";
        //eventLoad();
        
      }
      
    }
		return false;
	}
	
	bool stealKeyboard()
	{
		return guiManager.stealKeyboard();
	}
	
  void eventGenerate()
  {
    // First check to ensure the world has a name. Important because it's also the name of the savefile.
    
    if ( textEntryWorldName.input == "" )
    {
      std::cout<<"Error: World needs a name.\n";
    }
    else if (DataTools::isAlphaNumeric(textEntryWorldName.input) == false)
    {
      std::cout<<"Error: World name must be alphanumeric.\n";
      textEntryWorldName.input="";
    }
    else
    {
      int worldSizeV = DataTools::toInt(worldSize.getCurrentOption());
      
      if (worldSizeV<0 || worldSizeV>100000)
      {
        std::cout<<"Warning. Strange worldsize. Attempting to continue.\n";
      }
        // If a seed string has been entered, either use the actual number,
        // or hash it to a number if it contains non-numeric characters.
        // This allows for a word seed to have a numeric equivalent.
      int seed = 0;
      if (textEntryFullSeed.input != "")
      {
        if ( DataTools::isNumeric(textEntryFullSeed.input))
        {
          seed = DataTools::toInt(textEntryFullSeed.input);
        }
        else
        {
          std::hash<std::string> hasher;
          seed=hasher(textEntryFullSeed.input);
          if ( seed < 0 )
          {
            seed *= -1;
          }
        }
      }
      
      // GET FRAGMENTATION
      int fragmentation = DataTools::toInt(freeSteps.getCurrentOption());
      
      bool islandMode = true;
      if ( guiIslandMode.currentOption==1 )
      {
        islandMode=false;
      }

      bool wrapX = true;
      if ( guiWrapX.currentOption==1 )
      {
        wrapX=false;
      }
      bool wrapY = true;
      if ( guiWrapY.currentOption==1 )
      {
        wrapY=false;
      }
      
      double landPercent = (double) nLandPercent.currentValue / 100;

      world.generateWorld(textEntryWorldName.input,worldSizeV,worldSizeV,seed,fragmentation,islandMode,wrapX,wrapY,landPercent); /* MEMORY LEAK */
      world.generateTribes(nTribe.currentValue,nTribeDwarven.currentValue,nTribeElven.currentValue);
      world.nameRegions();
      
      //worldViewer.active=true;

    }
  }
  
    // Load the specified world into the generator.
  void eventLoad()
  {
    // First check to ensure the world has a name. Important because it's also the name of the savefile.
    if ( textEntryWorldName.input == "" )
    {
      std::cout<<"Error: World needs a name.\n";
    }
    else if (DataTools::isAlphaNumeric(textEntryWorldName.input) == false)
    {
      std::cout<<"Error: World name must be alphanumeric.\n";
      textEntryWorldName.input="";
    }
    else
    {
      world.loadWorld(textEntryWorldName.input);
    }
    
  }
  
};

#endif
