/* Driver.cpp
	Worldsim
	Project name: Worldsim.

	This project takes a world generated using worldgen and simulates the rise of civilisations on it.
*/


#define WILDCAT_USE_OPENGL
//#define WILDCAT_USE_DIRECT3D

	// What OS we are compiling for. Currently only Windows and Linux are supported cos I don't got a Mac.
#include <Wildcat/Windows.hpp>
//#define WILDCAT_LINUX

	// DYNAMICALLY GENERATED HEADER FILE WITH STRING WHICH COUNTS COMPILATIONS.
#include "CompileCount.hpp"

#include <iostream>


#include "Driver_Settings.cpp"

#include <Container/Vector/Vector.hpp>

	// VECTOR OF MESSAGES FOR THE PLAYER TO READ.
Vector <std::string> vConsoleMessage;

void consoleMessage(std::string s)
{
  vConsoleMessage.push(s);
}

#include <cstdlib>
#include <string>
#include <sstream>

//Stolen from https://codereview.stackexchange.com/questions/226/formatter-class
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

// This allows you to pass multiple datatypes as an std::string.
// Use like this: function( Stream() << "Error Recieved" << 42 << " " << some_code << " '" << some_msg << "'");

#include <GL/GLee.h> // THIS CURRENTLY FIXES LINKER CRAP. Also allows RGBA_COMPRESSED, it would seem.
#include <GL/freeglut.h> //

#include "Coordinate2D.hpp"

#include <Render/Renderer.cpp>

//#include <Output/ThreadedCout.hpp>

#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Texture/TextureLoader.hpp>

// #include <File/FileLog.hpp>
// FileLog gameLog;
// FileLog marriageLog;
// FileLog birthLog;
// FileLog deathLog;

#include "TextureInterface.cpp"

#include "WorldObject.cpp"

#include "Driver_LoadTextures.hpp"

#include "World.hpp"

#include "World_Local.hpp"




#include <Graphics/Png/Png.hpp>
#include <File/FileManager.hpp>

#include <string>

//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  

void printHelp()
{
	std::cout<<"\nWorldSim by Garosoft (garosoft.org)\n";
	std::cout<<"  Worldgen is a command line tool that generates a world map and exports it as an uncompressed PNG.\n";
	std::cout<<"  License: Public domain. This program uses a modified version of LodePNG.\n";
	std::cout<<"  This is an alpha release, and is not fully functional.\n";

	std::cout<<"Options:\n";
	std::cout<<"  -o \"x\". Required. Output the world data as: x.png. Must be alphanumeric. Default value is \"world\".\n";
	std::cout<<"  -s x. Create world of size (x,x). x should be (2^x)+1. Default value: 1025. Large values may crash the program due to memory limitations.\n";
	std::cout<<"  -n x. Generate x number of worlds. Output files will have numbers appended. Default value: 1.\n";
	std::cout<<"  -ocean x. Approximate percentage of the world that will be ocean. Default value: 66. Value of x must be between 0-100.\n";
	std::cout<<"  -seed x. Seed used to generate world. Default value is random. Value will be saved to the text file in case you get a world you want to generate again later. The maximum value for seed is "<<INT_MAX<<" (INT_MAX).\n";
	std::cout<<"  -seedland x.\n";
	std::cout<<"  -i. Enables island mode. The map will have water around the border. Disabled by default.\n";
	std::cout<<"  -c. Compress PNG. Disabled by default.\n";
	std::cout<<"  -wrapX. Wrap the X-axis.\n";
	std::cout<<"  -wrapY. Wrap the Y-axis.\n";
	std::cout<<"  -caves. Optional. Number of caves to spawn into the world. Default value: 64.\n";
	std::cout<<"  -load \"path\". Optional. Instead of generating a world, will load this world data file.";
	//std::cout<<"  NOT IMPLEMENTED: -id x. Each world has a unique id, if you want to generate a particular world, you can enter its id.\n";

	std::cout<<"\nExample:\n<example>\n";

	std::cout<<"\n";
	std::cout<<"Version "<<VERSION<<".\n";
	std::cout<<"Compiled: "<<__DATE__<<". "<<__TIME__<<".\n";
	std::cout<<"Compile count: "<<COMPILE_COUNT<<".\n";
	std::cout<<"\n";
	
	std::cout<<"Testing Github commits.\n";
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
	}
	~QuitChecker()
	{
		std::cout<<"Thankyou for playing. ^_^\n";
		gameTime.update();
		std::cout<<"Time played: "<<gameTime.seconds/60/60<<" hours, "<<gameTime.seconds/60<<" minutes, "<<gameTime.seconds<<" seconds.\n";
	}
};
QuitChecker quitChecker;

#include <stdio.h>
#include <Math/Random/GlobalRandom.hpp>
/* No need for a globalRandom object. Just use Random:: */
#include <Graphics/Font/Font.hpp>
Wildcat::Font font8x8;

#include <Device/Mouse/Mouse.hpp>
Mouse globalMouse;
#include <Device/Keyboard/Keyboard.hpp>
Keyboard globalKeyboard;


#include <Time/Timer.hpp>
Timer frameRateTimer;
Timer pollRateTimer;
Timer logicRateTimer;
Timer physicsRateTimer;
/* Use this for checking algo speeds. */
Timer debugTimer;

	/* WORLD HAS BEEN MOVED INTO GLOBAL SCOPE BECAUSE EVERYTHING NEEDS TO REFER TO IT */
#include "World.cpp"
World world;

#include "World_Local.cpp"
World_Local * worldLocal;


//#include "Creature.cpp"
// #include "Building.cpp"
	// #include "Building/Building_Farm.cpp"
	
#include "Character.cpp"

#include "Tribe.cpp"
#include "Tribe_Dwarven.cpp"
#include "Tribe_Human.cpp"
	
#include "WorldObjectGlobal.cpp"


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

# include <GUI/GUI_Manager.hpp>
#include <GUI/GUI.hpp>
/* GUI manager. Manages all GUI controls. */
GUI_Manager globalGuiManager;

/* Instead of having a global worldViewer, I think it would be better to have lots of local worldviewers, as currently I have a problem with the various menus interfering with the global worldViewer. */
#include "World_Viewer.hpp"
WorldViewer worldViewer;


#include <LogicTick/LogicTickInterface.hpp>
#include <LogicTick/LogicTickManager.hpp>
LogicTickManager logicTickManager;


#include <IdleTick/IdleTickInterface.hpp>
#include <IdleTick/IdleTickManager.hpp>
IdleTickManager idleManager;


/* Title Menu / Main Menu */
#include "Menu_Title.hpp"
//Menu_Title menuWorldGenerator;
Menu_Title menuTitle;

/* Title menu */
#include "Menu_WorldGenerator.hpp"

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


int main(int nArgs, char ** arg)
{
	//std::cout<<"\nGarosoft: TheGuild WIP.\n\n";
	GL_init(nArgs, arg);
	
	/* Initialise game. Load textures, fonts etc. */
	init();
  
	/* Reshape is called here. */
	glutMainLoop();
	
	return 0;
}