#pragma once
#ifndef WORLDSIM_DRIVER_GLOBAL_OBJECTS_HPP
#define WORLDSIM_DRIVER_GLOBAL_OBJECTS_HPP

//      GLOBAL RNG        //////////////////////////////////////////////////////////////////////////////////////////////

#include <Math/Random/RandomLehmer.hpp>
// use this to seed all RNGs instead of time(NULL)
RandomLehmer SEEDER;

RandomLehmer globalRandom; // global rng. Maybe should be replaced with static class functions.
RandomLehmer RNG_TEST; // for testing

#include <Math/Random/GlobalRandom.hpp>
/* No need for a globalRandom object. Just use Random:: */

//      MISC GLOBAL OBJECTS (SHOULD PROBABLY BE ORGANISED)        //////////////////////////////////////////////////////

#include "Driver_Settings_WordLists.cpp"

#include <Game/NameGen/NameGen.hpp>
NameGen globalNameGen;

#include <Graphics/Colour/Colour.hpp> // loading raw colours
#include <Graphics/Colour/ColourManager.hpp> // loading raw colours
ColourManager <unsigned char> colourManager;

	// VECTOR OF MESSAGES FOR THE PLAYER TO READ (Really should be an object)
#include <string>
#include <Container/Vector/Vector.hpp>
Vector <std::string> vConsoleMessage;

inline void consoleMessage(std::string s)
{
  vConsoleMessage.push(s);
}

inline void Console (std::string s)
{
  vConsoleMessage.push(s);
}

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

	// DYNAMICALLY GENERATED HEADER FILE WITH STRING WHICH COUNTS COMPILATIONS.
#include "CompileCount.hpp"

#include "Creature_Generator.cpp"
Creature_Generator creatureGenerator;

#include <File/SaveFileManager.hpp>
  // Class for managing world save files.
SaveFileManager saveFileManager;

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

// Runs on application exit
#include "Driver_QuitChecker.hpp"
QuitChecker quitChecker;

//Stolen from https://codereview.stackexchange.com/questions/226/formatter-class
// This allows you to pass multiple datatypes as an std::string.
// Use like this: function( Stream() << "Error Recieved" << 42 << " " << some_code << " '" << some_msg << "'");
// Should be moved into Wildcat
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


#endif
