#pragma once
#ifndef WORLDSIM_DRIVER_SETTINGS_CPP
#define WORLDSIM_DRIVER_SETTINGS_CPP

/* WorldSim: Driver_Settings.cpp
#include "Driver_Settings.cpp"

	Gameplay settings and other global settings go here. Some of this stuff will probably end up in an .ini file.
	Should probably be encapsulated in a GlobalSettings:: object
	Note that global objects such as RNG should be declared elsewhere to avoid dependency hell. This header should only
	be for variables, strings and defines.
*/

#include <string>

//      OS       ///////////////////////////////////////////////////////////////////////////////////////////////////////

#define WILDCAT_USE_OPENGL
//#define WILDCAT_USE_DIRECT3D

// What OS we are compiling for. Currently only Windows and Linux are supported cos I don't got a Mac.
// 32-bit Windows defines _WIN32; 64-bit Windows defines _WIN32 and _WIN64
// Note that threading is disabled on Linux until I can figure out how to get it compiling
#ifdef _WIN32
	#include <System/Windows.hpp> //#define WILDCAT_WINDOWS
	#define WILDCAT_THREADING
#elif defined(__linux__)
	#define WILDCAT_LINUX
	#undef WILDCAT_THREADING // disable threading
#else
	#error Unknown OS!
#endif

//#define WILDCAT_AUDIO

// MAKES COUT MUCH FASTER BUT DOES NOT GUARANTEE ORDER OR COMPLETION BEFORE EXIT/CRASH.
const bool FAST_COUT = false;

/* If CPU isn't relinquished, then OpenGL tends to run at 100% CPU for some reason. */
/* This uses MsgWaitForMultipleObjects, which will let the OS do other things until any kind of input event occurs */
const bool RELINQUISH_CPU = true;
const unsigned int RELINQUISH_CPU_TIMEOUT = 100; /* 10-100 seems to be okay. Maximum amount of time to relinquish */

//      THREADING       ////////////////////////////////////////////////////////////////////////////////////////////////
// Threading will not be optional in the future,
// it's way too complex to maintain a non-threaded version of the same code.

#ifdef WILDCAT_THREADING
	// Threading libraries
	#include <atomic>
	#include <mutex>
	#include <shared_mutex>
#endif

// I believe these require MingW-W64
// They do nothing if WILDCAT_THREADING is not defined
#include <System/Thread/Mutex.hpp>
#include <System/Thread/Atomic.hpp>

#define AUTO_SET_CORES // The number of cores will be set to the OS reported number of cores
unsigned short int N_CORES = 1; // can be changed in init if threads are enabled
const unsigned short int MAX_CORES = 4; // The number of cores will never exceed this value even if the OS has more.



//      PATHS       ////////////////////////////////////////////////////////////////////////////////////////////////////

// This needs to error if path is bad
//const std::string SAVE_FOLDER_PATH = "C:\\Users\\Lenovo\\Desktop\\WorldSim-savedata";

std::string currentSavePath = "";

#define SAVE_DATA // Program will save data to file. It saves a lot of data and it can end up taking up a lot of space, not good for your SSD.

//     WINDOW        ///////////////////////////////////////////////////////////////////////////////////////////////////

// SYSTEM STRINGS
const std::string VERSION_NUMBER = "0.0.205indev";

#ifdef WILDCAT_THREADING
	const std::string THREAD_STATUS = "threaded";
#else
	const std::string THREAD_STATUS = "unthreaded";
#endif

#ifdef WILDCAT_WINDOWS
	const std::string VERSION = VERSION_NUMBER+" Win32 "+THREAD_STATUS;
#else
	const std::string VERSION = VERSION_NUMBER+" Lin32 "+THREAD_STATUS;
#endif

const std::string G_WINDOW_TITLE = "WorldSim";

unsigned int RESOLUTIONX=1024, RESOLUTIONY=900;
const bool MAXIMISE_WINDOW = false;

//      GAME       /////////////////////////////////////////////////////////////////////////////////////////////////////

// Player stuff.
// The player is a character object, however he has a few special attributes which are handled differently.
// For efficiency these are handled globally to prevent making a class which is only inherited by a single
// character.
// This probably should be somewhere else
class Character;
Character * playerCharacter = 0; /* Global link to the player character (null if player hasn't selected a character) */
class Item;
Item * inventoryGrid [10][10];

// MAIN MENU ENUMS
enum enumMenu
{ MENU_UNKNOWN, MENU_TITLE, MENU_OPTIONS, MENU_LOADGAME, MENU_WORLDGENERATOR, MENU_WORLDSIMULATOR, MENU_ADVENTUREMODE };
// Should be MENU_TITLE but I'm bypassing it for now because the main menu has no function atm.
enumMenu activeMenu = MENU_WORLDGENERATOR;

// DIRECTION ENUMS
enum enumDirection { NORTH, EAST, SOUTH, WEST };

// RACE ENUM
enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};

// BASE TERRAIN ENUM (I DON'T KNOW IF THIS IS BEING USED)
enum class eLocalTerrain { G=0, GRASS=1, WATER=2 };

//DEBUG SETTINGS

bool FOG_OF_WAR = true; // Enable/disable fog rendering
bool QUICKSTART_GENERATOR = true; // Skip menu stuff and immediately generate a default world.
//Quickly go straight into the simulator.
const bool QUICKSTART_SIMULATOR = false; // Skip menu stuff and jump right into the game. (Will use defaults). 
const bool ENABLE_BACKGROUND_SIMULATION = false; /* Can cause choppy performance when debugging */

int worldPop = 0;
int lastline = 0;

// DEFAULT WORLD SETTINGS

const int QUICKSTART_WORLD_SIZE = 129; /* Please set to (power of 2)+1. */
long long int INITIAL_YEARS_SIMULATE = 0;

// Default setting on the GUI
const int DEFAULT_WORLD_SIZE_SLOT = 1; /* 0 = 33, 1 = 65, 129, 257, 513, 1025. Set it lower for easier debugging. */

const int DEFAULT_NUMBER_TRIBES_DWARVEN = 0;
const int DEFAULT_NUMBER_TRIBES_HUMAN = 12;
const int DEFAULT_NUMBER_TRIBES_ELVEN = 0;
const int DEFAULT_NUMBER_CIVS = 0;

/* Calendar setting when new game is started. */
const int CALENDAR_INITIAL_HOUR = 8;
const int CALENDAR_INITIAL_MINUTE = 0;
/* Determines speed of day/night cycle and other time scaling */
/* In future there might just be some time speed multiplier */
/* Or less hours in a day */
const int CALENDAR_SECONDS_PER_MINUTE = 2;

// SCALING

/* Size of each local map in tiles. Size is LOCAL_MAP_SIZE * LOCAL_MAP_SIZE.
Should be (n^2+1).
Realistically it should be 2049, however I might need to reduce it
depending on scaling and performance considerations.
It will likely be set to low values during development.
The final value will probably be fixed, because changing it will probably significantly change gameplay.
Right now I'm thinking 1025 might be a good balanced value, making each local map a square kilometer.
*/
//const int LOCAL_MAP_SIZE = 65;
// 129 is good for testing, but we will probably want to go much higher for release.
const int LOCAL_MAP_SIZE = 257;
int TIME_SCALE = 10; /* How many seconds of gametime pass per logic tick. */
// Timescale should be based on distances between cells.
// At 1:1 scale a cell is 5km*5km. Such a cell should take about 1 hour to walk across.
// Final local map size will probably be 513, and therefore timescale is 3600/512, or 7 seconds per tile.
// We can round this up to 10 for simplicity.
const int MAX_VIEW_RANGE = 20;

// CHARACTER STUFF
const int MAX_HUNGER = 1000;
const int MAX_THIRST = 300;

//      RENDERING       ////////////////////////////////////////////////////////////////////////////////////////////////

const bool OUTPUT_FRAMERATE = false; /* Will output the framerate. */
const int OUTPUT_FRAMERATE_SAMPLE_SIZE = 50;

const bool LIMIT_FRAMERATE = false;
const double FRAMERATE = 120; // SETTING IT TO A REASONABLE VALUE CAN GREATLY REDUCE STRESS ON GPU
const double POLLSPERSECOND = 30; // NOT CURRENTLY IMPLEMENTED
double LOGIC_PER_SECOND = 1;
const double PHYSICS_PER_SECOND = 10;
double ANIMATION_PER_SECOND = 10; // SETS SPEED OF ANIMATIONS

// SLOW FRAMERATE CAN BE ACTIVATED TO GET EXTRA CYCLES FOR GAMEPLAY.
int SLOW_FRAMERATE = 1000000/2;
// TURNS ON SLOW FRAMERATE
bool SLOW_FRAMERATE_ACTIVE = false;
int UFRAMERATE = 1000000/FRAMERATE;
const int UPOLLSPERSECOND = 1000000/POLLSPERSECOND;
int U_LOGIC_PER_SECOND = 1000000/LOGIC_PER_SECOND;
const int U_PHYSICS_PER_SECOND = 1000000/PHYSICS_PER_SECOND;
const int UANIMATIONSPERSECOND = 1000000/ANIMATION_PER_SECOND;
int CURRENT_ANIMATION_FRAME = 0; /* 0 - 99 */

/* Double buffering will make the frame transitions smoother, but it is slower. */
const bool DOUBLE_BUFFERING = false;

/* BUSY_WAIT: program will enter a busy wait if rendering is almost ready. Not sure if it works very well. */
const bool BUSY_WAIT = false;
const int BUSY_WAIT_USEC = 2000;

const bool LAZY_RENDERING=false; /* Only render if something happened */
bool RENDER_NEXT_FRAME=true; /* If there has been no input or state change, there's no need to render */

const bool COMPRESS_TEXTURES = false; /* May save graphics memory. Seems to significantly slow rendering */

//      MISC       /////////////////////////////////////////////////////////////////////////////////////////////////////

#include <limits.h> // We need ULONG_MAX for absolute coords.
#define ABSOLUTE_COORDINATE_NULL ULONG_MAX // Used as null-value for absolute coordinates
// I think this is probably pointless because any normal Windows/Linux/Mac system is going to use 4 bytes for int.
// Also such a thing may be specified explicitly with somthing like uint32_t. I need to move over to this system.
#define PORTABLE_INT_MAX 32767

// program will not do any garbage collection during shutdown
// makes testing easier but should be disabled for builds
#define FAST_EXIT

// The number of local maps to hold in memory at once. Minimum should be 4.
// (One for the player's current map, and three neighboring maps).
// However additional maps should be allocated for background processing.
// This should be set and adjusted based on the amount of RAM the user has.
// Not currently functional

// For rippling we'll probably want a minimum of 25 cells around the player permanently loaded.
const int MAX_LOCAL_MAPS_IN_MEMORY = 3;
const int MAX_LOCAL_MAPS_IN_BACKGROUND_MEMORY = 3;

// Coordinates of the map loaded for debugging.
int DEBUG_X=-1;
int DEBUG_Y=-1;

bool RESET=false;

bool HOTKEYS_ENABLED = true;

bool CLEAN_SAVES_ON_EXIT = true;




bool NO_BACKLOG=false;



// GLOBAL FLAGS

// increments each time a local map is accessed
// this allows us to cache the oldest maps
// only update when touching a new map.
// when this reaches overflow the IDs must be rebuilt
unsigned long int CACHE_ID=0;

Mutex mutexCout;
Mutex mutexCacheID; // lock to increment cache ID.

#ifdef WILDCAT_THREADING
	std::atomic <bool> QUIT_FLAG=false;

	// game will delay shutting down until the shutdown
	// manager can lock this mutex
	std::shared_mutex MUTEX_SHUTDOWN;
	std::shared_mutex MUTEX_TEST;
	//std::mutex mutexCout;

	// The maximum number of maps that may be cached in memory. This is set based on the amount
	// of free RAM, but has a minimum value of 12.
	// Currently it seems a local map requires about 2MB of RAM, but that will likely increase
	// in the future.
	// If the user has a fast swap file then it might be better to just cache everything
	// in virtual RAM and let the OS handle it. In this case it might be best to let
	// the user set the cache size.
	std::atomic <int> MAP_CACHE_SIZE = 12;
#else
	bool QUIT_FLAG = false;
	unsigned int MAP_CACHE_SIZE = 12;
#endif

bool DONT_RENDER = false; /* In debugging, rendering can be disabled by pressing R. */
bool LIMIT_LOGIC = true; /* Press S. */
bool FORCE_LOGIC_FRAME = false; /* True will trigger next logic frame immediately. */
bool PAUSE_LOGIC = false; /* True will suspend logic. */


bool EMULATE_2_BUTTON_MIDDLE_CLICK = true; // mmiddle click without middle mouse button by pressing left and right click

// MORE STRINGS

const char THOUSANDS_DELIMITER = ',';
const char DECIMAL_DELIMITER = '.';

std::string NYA;

const std::string ADVENTURE_MODE_MANUAL = "Adventure mode manual. Click MAN again or press ESC to close it.\nWelcome to Adventure Mode. This manual will be used to explain some basic stuff you can do in the game.\n\nButtons:\nINV - Access inventory and equipment. (not currently implemented)\n SNK - Sneak. Your line of sight will increase and allow you to see around corners. However your movement speed will be halved. Sneaking will also reduce your visibility so that if you are next to fog (from the NPC's view), you can't be seen.\nSPT - Sprint (not implemented). You will travel twice as fast but will have low view distance.\n\nHotkeys:\nE - use equipped item. Click on a tile to use your item there. Use mousewheel to scroll through options. Left click to select an option. Right-click to exit menu.\nSPACE - Talk to adjacent NPC.";

#endif