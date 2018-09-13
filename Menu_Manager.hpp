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
  
  All menus are given the entire window by default.
  
*/

// Menu references are hardcoded here.



class Menu_Interface: public GUI_Interface
{
  public:
  
  Menu_Interface()
  {
  }
  
  ~Menu_Interface()
  {
  }
  
  enumMenu menuID;
};

class Menu_Manager
{
  private:
  
  public:
  

  
  Vector <Menu_Interface*> vMenu;
  
  Menu_Manager()
  {
  }
  
  void add(Menu_Interface* _menu)
  {
    vMenu.add(_menu);
  }
  
  void switchTo(enumMenu _menu)
  {
    for(auto &m : vMenu)
    {
      if ( _menu == m->menuID )
      {
        m->active = true;
      }
      else
      {
        m->active = false;
      }
    }
  }
  
	void setFont(Wildcat::Font* _font)
	{
    for(auto &m : vMenu)
    {
      m->setFont(_font);
    }
	}
  
  /* GUI_Interface::render */
  void render()
  {
    for(auto &m : vMenu)
    {
      if ( m->active )
      {
        m->render();
        return;
      }
    }
  }
  
  void eventResize()
  {
    for(auto &m : vMenu)
    {
      m->eventResize();
    }
  }
  
  void initAll()
  {
    for(auto &m : vMenu)
    {
      m->init();
    }
  }
};


#include "Menu_Title.hpp"
#include "Menu_Options.hpp"






#endif
