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
        Console("You chop down the tree.");
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
    // For testing you can catch a fish on any tile.
    //if (_tile->getName() == "Ocean")
    if (true)
    {
      if (Random::flip())
      {
        Console("You catch a fishy.");
        auto fishy = new Item_Fish;
        owner->giveItem(fishy);
      }
      else
      {
        Console("Nothing bites");
      }
    }

  }

  void Item_Log::interact (LocalTile* obj)
  {
    Console (Stream() << "The "<<obj->getName()<<" is lit fam.");
    auto lit = new Item_Campfire;
    owner->giveItem(lit);
  }
  
  void Item_Fish::interact (WorldObject* _item)
  {
      // Check if we can cook with this object
    if ( _item->canCook )
    {
      
      Console ("You cook the fish");
      isCooked=true;
    }
  }

#endif
