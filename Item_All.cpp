#pragma once
#ifndef WORLDSIM_ITEM_ALL_CPP
#define WORLDSIM_ITEM_ALL_CPP

/* Item_All.cpp
	#include"Item.cpp"
  
  Implementation of Item.hpp.

*/

  void Item_Axe::interact(WorldObject* w)
  {
    //std::cout<<"Oh lawd he choppin\n";
    
    // Chopamount determines render.
    //
    
    if ( w->chopAmount > 0 )
    {
      //std::cout<<"Chop is legal\n";
      
      if (owner!=0)
      {
        std::cout<<"You chop down the tree.\n";
        //Just make a log and give to owner for now.
        auto _log = new Item_Log;
        owner->giveItem(_log);
        
        w->blocksView = false;
        w->blocksMovement = false;
        w->chopAmount=0;
      }
      
    }
    else
    {
      std::cout<<"You can't chop this\n";
    }
  }

  void Item_Fishrod::interact (LocalTile* _tile)
  {
    if (_tile->getName() == "Ocean")
    {
      std::cout<<"You cast your rod and wait...\n";
      

      
      if (Random::oneIn(3))
      {
        std::cout<<"You catch a fish.\n";
        auto fishy = new Item_Fish;
        owner->giveItem(fishy);
      }
      else
      {
        std::cout<<"Nothing bites.\n";
      }
    }
    std::cout<<"You start fishing.\n";
  }


#endif
