#pragma once
#ifndef WORLDSIM_MENU_MANAGER_HPP
#define WORLDSIM_MENU_MANAGER_HPP

#include <GUI/GUI_Interface.hpp>

//#include "World_Viewer.hpp"

/* WorldSim: Menu_Manager
	#include "Menu_Manager.hpp"
  
  The menu manager keeps track of which menu is active at any given time, 
  and passes control to it.
  
  This fixes the old system where menus would pass control through each 
  other in a hierarchy, leading to a mess of dependencies and conditions.

  Now each menu should be independent, and can be activated at any time.
  
  All menus should now be making use of GUI_Interface instead of
  individually inheriting mouse, keyboard and render inferfaces.
  
  Only one menu may be active at any time.
  
*/

class Menu_Manager
{
  public:
  
  Vector <GUI_Interface*> vMenu;
  
  Menu_Manager()
  {
  }
  
  void add(GUI_Interface* _menu)
  {
    vMenu.add(_menu);
  }
  
  /* GUI_Interface::render */
  void render()
  {
  }
};


#endif
