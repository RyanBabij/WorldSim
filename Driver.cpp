/* WorldSim: Driver
  
	Main file for WorldSim. Created by Ryan Babij (https://github.com/RyanBabij/WorldSim)
   
  All code and binaries in WorldSim repo are copyrighted.
  Wildcat code is public domain.
*/

#include <string>

#define WILDCAT_USE_OPENGL
//#define WILDCAT_USE_DIRECT3D

   // What OS we are compiling for. Currently only Windows and Linux are supported cos I don't got a Mac.
   // 32-bit Windows defines _WIN32; 64-bit Windows defines _WIN32 and _WIN64
#ifdef _WIN32
#include <System/Windows.hpp> //#define WILDCAT_WINDOWS
#elif defined(__linux__)
#define WILDCAT_LINUX
#else
#error Unknown OS!
#endif

//#define WILDCAT_AUDIO

#define GLEW_STATIC
// Need to figure out which of this is better. I think GLEW is more supported.
#include <Graphics/OpenGL/glew.h> // THIS CURRENTLY FIXES LINKER CRAP. Also allows RGBA_COMPRESSED, it would seem.
#define FREEGLUT_STATIC 
#include <Graphics/OpenGL/freeglut.h> //

	// DYNAMICALLY GENERATED HEADER FILE WITH STRING WHICH COUNTS COMPILATIONS.
#include "CompileCount.hpp"

#include <iostream>

#include "Driver_Settings.cpp"

#include <Container/Vector/Vector.hpp>

	// VECTOR OF MESSAGES FOR THE PLAYER TO READ.
Vector <std::string> vConsoleMessage;

inline void consoleMessage(std::string s)
{
  vConsoleMessage.push(s);
}

inline void Console (std::string s)
{
  vConsoleMessage.push(s);
}

#include <cstdlib>
#include <string>
#include <sstream>

//Stolen from https://codereview.stackexchange.com/questions/226/formatter-class
// This allows you to pass multiple datatypes as an std::string.
// Use like this: function( Stream() << "Error Recieved" << 42 << " " << some_code << " '" << some_msg << "'");
class Stream
{
  public:
    std::stringstream ss_;
    // Build a string by chaining << operators.
    template<class Field> Stream& operator<<(Field f)
    {
      ss_ << f;
      return *this;
    }
    // This is a custom typecast to std::string. (C++11)
    operator std::string() const { return ss_.str(); }
};

#include <File/SaveFileManager.hpp>
  // Class for managing world save files.
SaveFileManager saveFileManager;

#include <Graphics/Render/Renderer.cpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Texture/TextureLoader.hpp>

#include "TextureInterface.cpp"

#include "Driver_LoadTextures.hpp"

#include "World.hpp"

#include "World_Local.hpp"

#include <Graphics/Png/Png.hpp>
#include <File/FileManager.hpp>

#include <string>

void printHelp()
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

/* This object exploits the c++ guarantee that the destructor is always called, in order to deal with unanticipated shutdowns, such as the player clicking the X. However, it seems the destructor guarantee does not apply in some cases, such as ending the process using the task manager, or using ctrl+c from the console. */
class QuitChecker
{
	private:
	Timer gameTime;
	
	public:
	QuitChecker()
	{
		gameTime.init();
		gameTime.start();
    
    // For now we will clear the savedata on startup because it can cause some instability.
    std::cout<<"Deleting temporary data folder ("<<SAVE_FOLDER_PATH<<")\n";
    if (SAVE_FOLDER_PATH.length() > 0 )
    {
      FileManager::deleteDirectory(SAVE_FOLDER_PATH,true);
    }
	}
	~QuitChecker()
	{
    gameTime.update();
    if (gameTime.seconds > 10 )
    {
      std::cout<<"Time played: "<<gameTime.seconds/60<<" minutes.\n";
    }
    
    if (CLEAN_SAVES_ON_EXIT)
    {
      std::cout<<"Deleting temporary data folder ("<<SAVE_FOLDER_PATH<<")\n";
      if (SAVE_FOLDER_PATH.length() > 0 )
      {
        FileManager::deleteDirectory(SAVE_FOLDER_PATH,true);
      }
    }


	}
};
QuitChecker quitChecker;

#include <stdio.h>
#include <Math/Random/GlobalRandom.hpp>
/* No need for a globalRandom object. Just use Random:: */

#include <Graphics/Font/Font.hpp>

// This is the global font for now.
Wildcat::Font font8x8;

#include <Device/Mouse/Mouse.hpp>
Mouse globalMouse;
#include <Device/Keyboard/Keyboard.hpp>
Keyboard globalKeyboard;


#include <System/Time/Timer.hpp>
Timer frameRateTimer;
Timer pollRateTimer;
Timer logicRateTimer;
Timer physicsRateTimer;
Timer animationTimer;
Timer playerKeypressTimer;
/* Use this for checking algo speeds. */
Timer debugTimer;

/* Instead of having a global worldViewer, I think it would be better to have lots of local worldviewers, as currently I have a problem with the various menus interfering with the global worldViewer. */
#include "World_Viewer.hpp"

  // Moving world to global context means that we don't need to create millions of World pointers. */
#include "World.cpp"
World world;

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

#include <Math/Geometry/Geometry.hpp>

#include <Device/Display/DisplayInterface.hpp>
#include <Device/Display/DisplayInterfaceManager.hpp>
/* Global display interface manager, to handle all rendering called by driver. */
DisplayInterfaceManager displayInterfaceManager;

#include <Device/Mouse/MouseInterface.hpp>
#include <Device/Mouse/MouseInterfaceManager.hpp>
/* Global mouse interface manager. To handle all objects that recieve mouse events. */
MouseInterfaceManager mouseInterfaceManager;


#include <Device/Keyboard/KeyboardInterface.hpp>
#include <Device/Keyboard/KeyboardInterfaceManager.hpp>
/* Global keyboard interface manager. To handle all objects that recieve keyboard events. */
KeyboardInterfaceManager keyboardInterfaceManager;

#include <Graphics/GUI/GUI_Manager.hpp>
#include <Graphics/GUI/GUI.hpp>
/* GUI manager. Manages all GUI controls. */
GUI_Manager globalGuiManager;


#include <Interface/LogicTick/LogicTickInterface.hpp>
#include <Interface/LogicTick/LogicTickManager.hpp>
LogicTickManager logicTickManager;

#include <Interface/IdleTick/IdleTickInterface.hpp>
#include <Interface/IdleTick/IdleTickManager.hpp>
IdleTickManager idleManager;


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

/* Initialization goes here. */
#include "Driver_Init.hpp"

/* Tidies up the game and shuts down. */
void shutDown()
{
	std::cout<<"Driver::shutDown().\n";
	exit(0);
}

/* OpenGL function hooks go here. */
#include "Driver_GLHooks.hpp"

#include <Data/ArgReader.hpp>

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

	GL_init(nArgs, arg);
	
	/* Initialise game. Load textures, fonts etc. */
	init();
   
#if defined THREAD_ALL
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