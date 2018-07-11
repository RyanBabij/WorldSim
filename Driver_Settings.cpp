#pragma once
#ifndef WORLDSIM_DRIVER_SETTINGS_CPP
#define WORLDSIM_DRIVER_SETTINGS_CPP

/*
	"GLobalSettings.cpp"
	Gameplay settings and other global settings go here. Some of this stuff will probably end up in an .ini file.
*/

#include <mutex>

std::mutex render_mutex;

int worldPop = 0;
int lastline = 0;

bool FAST_COUT = false;

const int VERSION = 10004;
const std::string G_WINDOW_TITLE = "World Simulator";

const std::string SAVE_FOLDER_PATH = "savedata";

volatile const std::string devmessage = "HEY THIS IS GARO WADDUP.";

/* DEBUG MENU SHORTCUTS */
//bool AUTO_GENERATE_WORLD = false;
//bool AUTO_NEW_SIMULATION = true;
bool QUICKSTART = false; // Skip menu stuff and jump right into the game. (Will use defaults).
  //Quickly go straight into the simulator.
bool QUICKSTART_SIMULATOR = false; // Skip menu stuff and jump right into the game. (Will use defaults).
  // DEFAULT WORLD WHICH IS AUTO-GENERATED.
int QUICKSTART_WORLD_SIZE = 129; /* Please set to (power of 2)+1. */


long long int INITIAL_YEARS_SIMULATE = 0;

  // Default setting on the GUI
int DEFAULT_WORLD_SIZE_SLOT = 2; /* 0 = 129, 1 = 257, 513, 1025, 2049, 4097. You can set it lower for easier debugging. */

int DEFAULT_NUMBER_TRIBES_DWARVEN = 12;
int DEFAULT_NUMBER_TRIBES_HUMAN = 12;
int DEFAULT_NUMBER_TRIBES_ELVEN = 12;
int DEFAULT_NUMBER_CIVS = 0;

bool FOG_OF_WAR = true;

const int LOCAL_MAP_SIZE = 64; /* Size of each city in tiles. Size is CITY_SIZE * CITY_SIZE. */
//const int LOCAL_MAP_SIZE = 2000; /* Size of each city in tiles. Size is CITY_SIZE * CITY_SIZE. */

int TIME_SCALE = 60; /* How many seconds of gametime pass per logic tick. */

//const int PLANTING_TIME = 7200; /* Realistic. */
const int PLANTING_TIME = 100; /* Testing. */
const int HARVESTING_TIME = 100; /* Testing. */



unsigned int RESOLUTIONX=1200, RESOLUTIONY=1000;
bool RESET=false;

bool HOTKEYS_ENABLED = true;

/* Will output the framerate. */
const bool OUTPUT_FRAMERATE = false;
const int OUTPUT_FRAMERATE_SAMPLE_SIZE = 50;

const bool LIMIT_FRAMERATE = true;
const double FRAMERATE = 60;
const double POLLSPERSECOND = 30;
//const double LOGIC_PER_SECOND = 30;
//const double LOGIC_PER_SECOND = 100; /* For debugging. TODO: Need a secret switch to disable timing/Toggle rendering/etc. */
double LOGIC_PER_SECOND = 1;
const double PHYSICS_PER_SECOND = 10;


	// SLOW FRAMERATE CAN BE ACTIVATED TO GET EXTRA CYCLES FOR GAMEPLAY.
int SLOW_FRAMERATE = 1000000/2;
	// TURNS ON SLOW FRAMERATE
bool SLOW_FRAMERATE_ACTIVE = false;

int UFRAMERATE = 1000000/FRAMERATE;
const int UPOLLSPERSECOND = 1000000/POLLSPERSECOND;
int U_LOGIC_PER_SECOND = 1000000/LOGIC_PER_SECOND;
const int U_PHYSICS_PER_SECOND = 1000000/PHYSICS_PER_SECOND;

const char THOUSANDS_DELIMITER = ',';
const char DECIMAL_DELIMITER = '.';

/* Double buffering will make the frame transitions smoother, but it is slower. */
const bool DOUBLE_BUFFERING = false;

/* BUSY_WAIT will cause the program to enter a busy wait if rendering is almost ready. Not sure if it works very well. */
const bool BUSY_WAIT = false;
const int BUSY_WAIT_USEC = 2000;

/* If CPU isn't relinquished, then OpenGL tends to run at 100% CPU for some reason. */
const bool RELINQUISH_CPU = true;
const unsigned int RELINQUISH_CPU_MSEC = 1; /* Previous value: 1. Anywhere from 10-100 seems to be okay. 1000 = 1 second? */

bool QUIT_FLAG = false;

bool DONT_RENDER = false; /* In debugging, rendering can be disabled by pressing R. */
bool LIMIT_LOGIC = true; /* Press S. */

bool FORCE_LOGIC_FRAME = false; /* True will trigger next logic frame immediately. */
bool PAUSE_LOGIC = false; /* True will suspend logic. */

/* Calendar setting when new game is started. */
const int CALENDAR_INITIAL_HOUR = 5;
const int CALENDAR_INITIAL_MINUTE = 59;

/* Global var to flag for a test battle. */
bool BATTLE_TEST = false;
bool BATTLE_TEST_INITIALISED = false;

bool REVEAL_ALL = true;

bool EMULATE_2_BUTTON_MIDDLE_CLICK = false; // Allows middle click with mice without middle click by pressing left and right click.

#endif