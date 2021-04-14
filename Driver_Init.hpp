#pragma once
#ifndef WORLDSIM_DRIVER_INIT_HPP
#define WORLDSIM_DRIVER_INIT_HPP

/* WorldSim: Driver_Init.hpp
#include "Driver_Init.hpp"

Driver initialization goes here.

Initialize main menus, start timers, initialise any important data.

*/

#include <File/WTFManager.hpp> // loading raws
#include <Graphics/Colour/Colour.hpp> // loading raw colours

void init()
{
	std::cout<<"\n*** INIT ***\n";
	SEEDER.seed(time(NULL));
	Random::seed();
	globalRandom.seed(SEEDER);
	globalNameGen.seed(SEEDER);
	
	// load globalSettings
	globalSettings.load(FileManager::getFileAsString("raw/ini.wtf"));
	
	std::cout<<"   *** RAWS ***\n";
	WTFManager wtfManager;
	
	// BUILD COLOURS FROM THE RAW
	
	// Global container for colour raws.
	ColourManager colourManager;
	
	wtfManager.parse(FileManager::getFileAsString("raw/colours.wtf"));
	
	std::cout<<"All raws:\n"<<wtfManager.getAll()<<"\n\n";
	
	WTFNode* node = wtfManager.getRandom("COLOUR", globalRandom);

	
	if (node==0)
	{
		std::cout<<"Search returned null ptr\n";
	}
	else
	{
		std::cout<<"Searched node: "<<node->toString()<<"\n";
		
		std::cout<<"Getting node RGB.\n";
		std::cout<<node->getSub("NAME")->toString()<<"\n";
		std::cout<<node->getSub("RGB")->toString()<<"\n";
		
		// alternate: get all colours
		Vector <WTFNode*>* vNode = wtfManager.getAllSub("COLOUR");
		for (int i=0;i<vNode->size();++i)
		{
			Vector <std::string> * vRGB = (*vNode)(i)->getValues("RGB");
			std::string colourName = (*vNode)(i)->getValue("NAME");
			
			if (vRGB->size() == 3 && colourName != "")
			{
				colourManager.makeColour(DataTools::toInt( (*vRGB)(0) ) ,DataTools::toInt( (*vRGB)(1) ),
					DataTools::toInt( (*vRGB)(2) ),colourName);
			}
			else
			{
				std::cout<<"Error: Colours are wrong\n";
			}
		}
		std::cout<<"Colours built:\n"<<colourManager.toString()<<"\n";
	}
	
	std::cout<<"   *** END RAWS ***\n";
	
			//CLEAN_SAVES_ON_EXIT=false;
			//QUIT_FLAG=true;

	//Makes cout faster but less reliable
	if ( FAST_COUT )
	{
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
	}

	consoleMessage("GAME STARTED");

	playerCharacter = 0;

	globalKeyboard.init(); 

	/* Load textures (Tex pointers from Driver_TextureList.hpp). */
	loadTextures();

	//LOAD RECIPES
	recipeManager.addRecipe(&recipeGrilledFish);
	recipeManager.addRecipe(&recipeWall);

	/* Load font */
	Png fontPng;
	int fileSize;
	unsigned char* fileData = FileManager::getFile("Textures/Font/8x8/8x8 Transparent v3 plus junk.png",&fileSize);	

	if ( fileData == 0 )
	{
		std::cout<<"ERROR: Font PNG did not load.\n";
	}
	else
	{
		fontPng.load(fileData,fileSize);
		if(font8x8.loadData(&fontPng,8,8)==false)
		{ std::cout<<"ERROR: Font did not load.\n"; }
		delete [] fileData;
	}

	logicTickManager.add(&menuTitle);
	idleManager.add(&world);
	logicTickManager.add(&world);

	initCreatureAttacks();

	/* Start timers. */
	frameRateTimer.init();
	frameRateTimer.start();
	pollRateTimer.init();
	pollRateTimer.start();
	logicRateTimer.init();
	logicRateTimer.start();
	physicsRateTimer.init();
	physicsRateTimer.start();
	animationTimer.init();
	animationTimer.start();
	debugTimer.init();
	debugTimer.start();

	playerKeypressTimer.init();
	playerKeypressTimer.start();

	/* Initialise the main menu */
	menuTitle.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuTitle.init();
	menuTitle.setFont(&font8x8);
	menuTitle.backgroundTexture=&TEX_TITLE;
	menuTitle.active=true;

	/* Initialise the options menu */
	menuOptions.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuOptions.init();
	menuOptions.setFont(&font8x8);
	menuOptions.backgroundTexture=&TEX_TITLE;
	menuOptions.active=false;

	/* Initialise the load game menu */
	menuLoadGame.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuLoadGame.init();
	menuLoadGame.setFont(&font8x8);
	menuLoadGame.backgroundTexture=&TEX_TITLE;
	menuLoadGame.active=false;

	/* Initialise the world generator menu */
	menuWorldGenerator.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuWorldGenerator.init();
	menuWorldGenerator.setFont(&font8x8);
	//menuWorldGenerator.backgroundTexture=&TEX_TITLE;
	menuWorldGenerator.active=false;

	/* Initialise the world simulator menu */
	menuWorldSimulator.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuWorldSimulator.init();
	menuWorldSimulator.setFont(&font8x8);
	//menuWorldSimulator.backgroundTexture=&TEX_TITLE;
	menuWorldSimulator.active=false;

	menuAdventureMode.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuAdventureMode.init();
	menuAdventureMode.setFont(&font8x8);
	//menuAdventureMode.backgroundTexture=&TEX_TITLE;
	menuAdventureMode.active=false;

	mouseInterfaceManager.add(&menuTitle);
	//keyboardInterfaceManager.add(&menuTitle);
	globalGuiManager.add(&menuTitle);
	
	std::cout<<" *** END INIT ***\n\n";
}

#endif
