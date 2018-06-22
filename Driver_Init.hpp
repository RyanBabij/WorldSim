/* Driver_Init.hpp
#include "Driver_Init.hpp"

Driver initialization goes here.

*/

#include <NameGen/NameGen.hpp>
NameGen ng;



void init()
{	
	//Makes cout faster but less reliable
	
	if ( FAST_COUT )
	{
		std::ios_base::sync_with_stdio(false);
		std::cin.tie(NULL);
	}
	
	vConsoleMessage.push("CONSOLE MESSAGE TEST 1");
	vConsoleMessage.push("CONSOLE MESSAGE TEST 2");
	vConsoleMessage.push("CONSOLE MESSAGE TEST 3");
	

	globalKeyboard.init(); 

	/* Load textures (Tex pointers from Driver_TextureList.hpp). */
	std::cout<<"Loading textures.\n";
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
	
	/* WORLD SHITS */
	//world.generateTestWorld();
	//world.loadWorld("World1/09.png");
	//world.loadHeightMap("World1/08.png");
	//world.loadWorldData("World1/WorldData.txt");

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
	debugTimer.init();
	debugTimer.start();

	
	menuTitle.setPanel(0,0,RESOLUTIONX,RESOLUTIONY);
	menuTitle.init();
	menuTitle.setFont(&font8x8);
	menuTitle.backgroundTexture=&TEX_TITLE;
	menuTitle.active=true;
	

	mouseInterfaceManager.add(&menuTitle);
	//keyboardInterfaceManager.add(&menuTitle);
	globalGuiManager.add(&menuTitle);

	
}
