#pragma once
#ifndef WORLDSIM_DRIVER_INIT_HPP
#define WORLDSIM_DRIVER_INIT_HPP

/* WorldSim: Driver_Init.hpp
  #include "Driver_Init.hpp"

Driver initialization goes here.

Initialize main menus, start timers, initialise any important data.

*/

#include <NameGen/NameGen.hpp>
NameGen ng;



void init()
{	
  Random::seed();

	//Makes cout faster but less reliable
	if ( FAST_COUT )
	{
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
	}
	
	consoleMessage("CONSOLE MESSAGE TEST 1");
	consoleMessage("CONSOLE MESSAGE TEST 2");
	consoleMessage("CONSOLE MESSAGE TEST 3");
  
  playerCharacter = 0;
	
	globalKeyboard.init(); 

	/* Load textures (Tex pointers from Driver_TextureList.hpp). */
	loadTextures();
	
	/* Load font */
	Png fontPng;
	int fileSize;
	unsigned char* fileData = FileManager::getFile("Textures/Font/8x8/8x8_Transparent.png",&fileSize);	
	fontPng.load(fileData,fileSize);
	if(font8x8.loadData(fontPng.data,8,8)==false)
	{ std::cout<<"Error: Font did not load.\n"; }
	delete [] fileData;
	
	logicTickManager.add(&menuTitle);
	idleManager.add(&world);
	logicTickManager.add(&world);


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
	
}

#endif
