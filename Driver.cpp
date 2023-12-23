/* WorldSim: Driver
  
	Main file for WorldSim. Created by Ryan Babij (https://github.com/RyanBabij/WorldSim)
   
  All code and binaries in WorldSim repo are copyrighted.
  Wildcat code is public domain.
*/

// Global vars and strings (should eventually be ported into globalSettings object)
#include "Driver_Settings.cpp"
#include "Driver_GlobalSettings.cpp"
GlobalSettings globalSettings;

// OpenGL includes go here because some global objects depend on OpenGL defines.
#define GLEW_STATIC // static link GLEW 
// Need to figure out which of this is better. I think GLEW is more supported.
#include <Graphics/OpenGL/glew.h> // THIS CURRENTLY FIXES LINKER CRAP. Also allows RGBA_COMPRESSED, it would seem.
#define FREEGLUT_STATIC // static link freeGLUT
#include <Graphics/OpenGL/freeglut.h> //

// Global objects declared after OpenGL because some objects depend on OpenGL defines.
#include "Driver_GlobalObjects.hpp"


// This is a huge mess and should be cleaned up by making sure headers include all files they need.
#include "Creature_Species.cpp" // Not sure why this needs to be here.
#include "TextureInterface.cpp"
#include "Driver_LoadTextures.hpp"
#include "World.hpp"
#include "World_Local.hpp"


/* Instead of having a global worldViewer, I think it would be better to have lots of local worldviewers, as currently I have a problem with the various menus interfering with the global worldViewer. */
#include "World_Viewer.hpp"
#include "World_Viewer_Minimap.hpp"

#include <Graphics/Render/Renderer.cpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Texture/TextureLoader.hpp>

  // Moving world to global context means that we don't need to create millions of World pointers. */
#include "World.cpp"
World world;

// Being lazy and just having menus as global objects. These could definitely go somewhere better.

/* Title Menu / Main Menu */
#include "Menu_Title.hpp"
Menu_Title menuTitle;
/* Options menu */
#include "Menu_Options.hpp"
Menu_Options menuOptions;
/* Load game menu */
#include "Menu_LoadGame.hpp"
Menu_LoadGame menuLoadGame;

/* Title menu */
#include "Menu_WorldGenerator.hpp"
Menu_WorldGenerator menuWorldGenerator;

#include "Menu_WorldSimulator.hpp"
Menu_WorldSimulator menuWorldSimulator;

#include "Menu_AdventureMode.hpp"
Menu_AdventureMode menuAdventureMode;

// Implementation headers. I think they probably should be going somewhere else, aka the headers that actually need
// the implementations. But there's so many pointers that many of them basically don't need them.

#include "World_Biome.cpp"

#include "Pathing.hpp"

#include "World_Local.cpp"
World_Local * worldLocal;

#include "LocalTile.cpp"
	
#include "Creature.cpp"
  #include "Creature_Knowledge.cpp"
  #include "Creature_All.cpp"
  #include "Creature_Deer.cpp"
  
#include "Character.cpp"
  #include "Character_Knowledge.cpp"

#include "Civ.cpp"
  #include "Civ_Dwarven.cpp"
  
#include "Settlement.cpp"
  #include "Settlement_Dwarven.cpp"

#include "Tribe.cpp"
  #include "Tribe_Dwarven.cpp"
  #include "Tribe_Human.cpp"
  #include "Tribe_Elf.cpp"
  
#include "WorldObject.cpp"
  #include "WorldObject_Tree.cpp"
  #include "WorldObject_Flora.cpp"
  #include "WorldObject_Rock.cpp"
  #include "WorldObject_Sign.cpp"
  
#include "WorldObjectGlobal.cpp"
  #include "WorldObjectGlobal_TribalArtifact.cpp"
  

#include "Item.cpp"
#include "Recipe.cpp"
#include "Item_Attributes.cpp"
#include "ItemFactory.cpp"

#include "Location.cpp"
#include "Stockpile.cpp"


/* Game initialization goes here. */
#include "Driver_Init.hpp"

/* OpenGL function hooks go here. */
#include "Driver_GLHooks.hpp"

// Main dependencies.
#include <Data/ArgReader.hpp>
#include <signal.h> // for catching CTRL+C
#include <iostream>

inline void printHelp()
{
	std::cout<<"\nWorldSim"<<VERSION<<".\n";
	std::cout<<"  Warning: This is not a stable release.\n";
	std::cout<<"  WorldSim is a 2D tile-based sandbox RPG with procedurally generated fantasy world.\n";
	std::cout<<"  License: Public domain. This program uses a modified version of LodePNG.\n";
	std::cout<<"  This is a pre-alpha release, and is not fully functional.\n";

	std::cout<<"Options:\n";
	std::cout<<"None.\n";

	std::cout<<"\n";
	std::cout<<"Version "<<VERSION<<".\n";
	std::cout<<"Compiled: "<<__DATE__<<". "<<__TIME__<<".\n";
	std::cout<<"Compile count: "<<COMPILE_COUNT<<".\n";
	std::cout<<"\n";
}

void pauseGame()
{
	std::cout<<"pauseGame() called.\n";
	PAUSE_LOGIC=true;
}

/* Tidies up the game and shuts down. */
void shutDown(int signal /*=0*/)
{
	std::cout<<"Driver::shutDown().\n";
   QUIT_FLAG=true;
	exit(1);
}

int main(int nArgs, char ** arg)
{
	ArgReader argReader;
	argReader.feed(nArgs,arg);
  
	if (argReader.hasTag("-help") || argReader.hasTag("--help") || argReader.hasTag("-h"))
	{
		printHelp();
		return 0;
	}
  
  
	std::cout<<"\nWorldSim "<<VERSION<<". Warning: This is not a stable release.\n";
	std::cout<<"Warning: This program writes a lot of data to disk, which may wear out SSDs.\n";

	GL_init(nArgs, arg);
	
	/* Initialise game. Load textures, fonts etc. */
	init();
   
   // Catch CTRL+C signals
   // CTRL+C from the CMD seems to terminate threads before they
   // can shutdown properly, leaving the shutdown process hanging
   // fortunately this shouldn't happen in normal builds
   // This function works by intercepting the normal
   // CTRL+C shutdown and replaces it with a safer shutdown.
   // If CTRL+C is signalled a second time it will hard exit,
   // and therefore I also have a time on thread shutdown checks.
#ifdef WILDCAT_THREADING
   signal (SIGINT,shutDown);
#endif
   
#if defined WILDCAT_THREADING
  // std::thread testThread( []
  // {
     // Sleep(1000);
     // while(true)
     // {
        // std::cout<<"Hello, I'm a thread\n";
        // Sleep(1000);
     // }
  // });
#endif
  
	/* Reshape is called here. */
	glutMainLoop();
	
	return 0;
}