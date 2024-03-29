#pragma once
#ifndef WORLDSIM_DRIVER_INIT_HPP
#define WORLDSIM_DRIVER_INIT_HPP

/* WorldSim: Driver_Init.hpp
#include "Driver_Init.hpp"

Driver initialization goes here.

Initialize main menus, start timers, initialise any important data.

*/

#include <File/WTFManager.hpp> // loading raws

void init()
{
	std::cout<<"\n*** INIT ***\n";
	
	globalCalendar.set(0,0,0,CALENDAR_INITIAL_HOUR,CALENDAR_INITIAL_MINUTE,0);
	globalCalendar.configure(SECONDS_PER_MINUTE,MINUTES_PER_HOUR,HOURS_PER_DAY,DAYS_PER_MONTH,MONTHS_PER_YEAR);

	SEEDER.seed(time(NULL));
	Random::seed();
	globalRandom.seed(SEEDER);
	globalNameGen.seed(SEEDER);
	gNameGeneratorWord.seed(SEEDER);
	
	// load globalSettings
	globalSettings.load(FileManager::getFileAsString("raw/ini.wtf"));
	
	std::cout<<"   *** RAWS ***\n";
	WTFManager wtfManager;
	
	// BUILD COLOURS FROM THE RAW
		// load colours. Used for Flora and anything else with Colour attributes
	wtfManager.parse(FileManager::getFileAsString("raw/colours.wtf"));
		// Load creature species.
	wtfManager.parse(FileManager::getFileAsString("raw/creatures.wtf"));
		// Load texture raws
	wtfManager.parse(FileManager::getFileAsString("raw/textures.wtf"));
	
	std::cout<<"All raws:\n"<<wtfManager.getAll()<<"\n\n";
	
	

		// LOAD COLOURS
	
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
	ColourRGB <unsigned char> c (64,64,64);
	ColourRGB <unsigned char> * c2 = colourManager.getClosestTo(c);
	
	if (c2)
	{
		std::cout<<"The closest colour to "<<c.toString()<<" is "<<c2->toString()<<"\n";
	}
	
	
	// Load texture references
	textureManager.loadRaw(FileManager::getFileAsString("raw/textures.wtf"));
	
		// LOAD CREATURE SPECIES
	// I'm not sure if we should load raws out here or let the class handle it. For now I'll leave it for the generator
	// class but I might change it later.
	creatureGenerator.loadRaw(FileManager::getFileAsString("raw/creatures.wtf"));

		
	std::cout<<"   *** END RAWS ***\n";
	
			// CLEAN_SAVES_ON_EXIT=false;
			// QUIT_FLAG=true;

	//Makes cout faster but less reliable
	if ( FAST_COUT )
	{
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
	}
	else
	{
		std::ios_base::sync_with_stdio(true);
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
	{
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
	}
	
	/* Load white font */
	{
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
			fontPng.makeWhite();
			if(font8x8White.loadData(&fontPng,8,8)==false)
			{ std::cout<<"ERROR: Font white did not load.\n"; }
			delete [] fileData;
		}
	}
	
	font8x8.ySpacing=2;
	font8x8White.ySpacing=2;

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
