#pragma once
#ifndef WORLDSIM_DRIVER_SETTINGS_CPP
#define WORLDSIM_DRIVER_SETTINGS_CPP

/* WorldSim: Driver_Settings.cpp
  #include "Driver_Settings.cpp"

	Gameplay settings and other global settings go here. Some of this stuff will probably end up in an .ini file.
*/

//#include <mutex>

//std::mutex render_mutex;

#include <limits.h> // We need ULONG_MAX for absolute coords.
#define ABSOLUTE_COORDINATE_NULL ULONG_MAX // Used as null-value for absolute coordinates, which can't use negative value.

// This is a guaranteed valid value for int for all implementations of C++.
// Generally I think the actual maximum is much higher, but we'll figure
// that out later.
#define PORTABLE_INT_MAX 32767


  // Player stuff.
  // The player is a character object, however he has a few special attributes which are handled differently.
  // For efficiency these are handled globally to prevent making a class which is only inherited by a single
  // character.
class Character;
Character * playerCharacter = 0; /* Global link to the player character (null if player hasn't selected a character) */
class Item;
Item * inventoryGrid [10][10];

  // SYSTEM STRINGS
const std::string VERSION = "0.0.113 Win32 dev";
const std::string G_WINDOW_TITLE = "WorldSim";
const std::string SAVE_FOLDER_PATH = "savedata";

  // MAIN MENU ENUMS
enum enumMenu { MENU_UNKNOWN, MENU_TITLE, MENU_OPTIONS, MENU_LOADGAME, MENU_WORLDGENERATOR, MENU_WORLDSIMULATOR, MENU_ADVENTUREMODE };
// Should be MENU_TITLE but I'm bypassing it for now because the main menu has no function atm.
enumMenu activeMenu = MENU_WORLDGENERATOR;

  // RACE ENUM
enum enumRace { NONE=0, HUMAN=1, DWARVEN=2, ELVEN=3};

  // BASE TERRAIN ENUM (I DON'T KNOW IF THIS IS BEING USED)
enum class eLocalTerrain { G=0, GRASS=1, WATER=2 };

  //DEBUG SETTINGS

bool FOG_OF_WAR = true; // Enable/disable fog rendering
bool QUICKSTART_GENERATOR = true; // Skip menu stuff and immediately generate a default world.
  //Quickly go straight into the simulator.
const bool QUICKSTART_SIMULATOR = false; // Skip menu stuff and jump right into the game. (Will use defaults). 

int worldPop = 0;
int lastline = 0;

  // DEFAULT WORLD SETTINGS

const int QUICKSTART_WORLD_SIZE = 129; /* Please set to (power of 2)+1. */
long long int INITIAL_YEARS_SIMULATE = 0;

  // Default setting on the GUI
const int DEFAULT_WORLD_SIZE_SLOT = 2; /* 0 = 129, 1 = 257, 513, 1025, 2049, 4097. You can set it lower for easier debugging. */

const int DEFAULT_NUMBER_TRIBES_DWARVEN = 0;
const int DEFAULT_NUMBER_TRIBES_HUMAN = 6;
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
*/
const int LOCAL_MAP_SIZE = 65;
//const int LOCAL_MAP_SIZE = 129;
int TIME_SCALE = 60; /* How many seconds of gametime pass per logic tick. */
const int MAX_VIEW_RANGE = 80;

  // CHARACTER STUFF
const int MAX_HUNGER = 300;

// The number of local maps to hold in memory at once. Minimum should be 4.
// (One for the player's current map, and three neighboring maps).
// However additional maps should be allocated for background processing.
// Not currently functional
const int MAX_LOCAL_MAPS_IN_MEMORY = 10;

unsigned int RESOLUTIONX=1024, RESOLUTIONY=720;
bool RESET=false;

const bool HOTKEYS_ENABLED = false;

/* Will output the framerate. */
const bool OUTPUT_FRAMERATE = false;
const int OUTPUT_FRAMERATE_SAMPLE_SIZE = 50;

const bool LIMIT_FRAMERATE = true;
const double FRAMERATE = 60; // SETTING IT TO A REASONABLE VALUE CAN GREATLY REDUCE STRESS ON GPU
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


  // RENDER AND FRAME STUFF

/* Double buffering will make the frame transitions smoother, but it is slower. */
const bool DOUBLE_BUFFERING = false;

/* BUSY_WAIT will cause the program to enter a busy wait if rendering is almost ready. Not sure if it works very well. */
const bool BUSY_WAIT = false;
const int BUSY_WAIT_USEC = 2000;

/* If CPU isn't relinquished, then OpenGL tends to run at 100% CPU for some reason. */
const bool RELINQUISH_CPU = true;
const unsigned int RELINQUISH_CPU_MSEC = 1; /* Previous value: 1. Anywhere from 10-100 seems to be okay. 1000 = 1 second? */



// GLOBAL FLAGS

bool QUIT_FLAG = false;
bool DONT_RENDER = false; /* In debugging, rendering can be disabled by pressing R. */
bool LIMIT_LOGIC = true; /* Press S. */
bool FORCE_LOGIC_FRAME = false; /* True will trigger next logic frame immediately. */
bool PAUSE_LOGIC = false; /* True will suspend logic. */
  // MAKES COUT MUCH FASTER BUT DOES NOT GUARANTEE ORDER OR COMPLETION BEFORE EXIT/CRASH.
const bool FAST_COUT = false;

bool EMULATE_2_BUTTON_MIDDLE_CLICK = true; // Allows middle click with mice without middle click by pressing left and right click.

// MORE STRINGS

const char THOUSANDS_DELIMITER = ',';
const char DECIMAL_DELIMITER = '.';

std::string NYA;

const std::string ADVENTURE_MODE_MANUAL = "Adventure mode manual. Click MAN again or press ESC to close it.\nWelcome to Adventure Mode. This manual will be used to explain some basic stuff you can do in the game.\n\n INV - Access inventory and equipment. (not currently implemented)\n SNK - Sneak. Your line of sight will increase and allow you to see around corners. However your movement speed will be halved. Sneaking will also reduce your visibility so that if you are next to fog (from the NPC's view), you can't be seen.\nSPT - Sprint (not implemented). You will travel twice as fast but will have low view distance.\n\nSPACE - Talk to adjacent NPC.";

#endif