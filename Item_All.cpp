#pragma once
#ifndef WORLDSIM_ITEM_ALL_CPP
#define WORLDSIM_ITEM_ALL_CPP

/* Item_All.cpp
	#include"Item.cpp"
  
  Implementation of Item.hpp.

*/

  // AXE
  
  Vector <std::string>* Item_Axe::getInteractNames(WorldObject* _w)
  {
    if (_w==0) { return 0; }
    auto vInteract = new Vector <std::string>;
    
    if (_w->chopAmount >= 0)
    {
      vInteract->push("Chop "+_w->getName());
    }
    return vInteract;
  }
  
    Vector <std::string>* Item_Axe::getInteractNames(Item* _w)
    {
      if (_w==0) { return 0; }
      return 0;
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Chop "+_w->getName());
      return vInteract;
    }
    Vector <std::string>* Item_Axe::getInteractNames(Character* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Chop "+_w->getName());
      return vInteract;
    }
    Vector <std::string>* Item_Axe::getInteractNames(Creature* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Chop "+_w->getName());
      return vInteract;
    }
  Vector <std::string>* Item_Axe::getInteractNames(LocalTile* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Chop ground");
    return vInteract;
  }

  void Item_Axe::interact(WorldObject* w, int interactType /* =0 */)
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

  void Item_Fishrod::interact (LocalTile* _tile, int interactionType /* =0 */)
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


  
  
  // LOG
  
  void Item_Log::interact (LocalTile* obj, int interactionType /* =0 */)
  {
    std::cout<<"InteractionType is: "<<interactionType<<"\n";
    if (interactionType == 0 )
    {
      Console (Stream() << "The "<<obj->getName()<<" is lit fam.");
      auto lit = new Item_Campfire;
      owner->giveItem(lit);
    }
    else
    {
      Console (Stream() << "You made some planks.");
      auto plank1 = new Item_Plank;
      auto plank2 = new Item_Plank;
      auto plank3 = new Item_Plank;
      owner->giveItem(plank1);
      owner->giveItem(plank2);
      owner->giveItem(plank3);
    }
  }
  
  Vector <std::string>* Item_Log::getInteractNames(WorldObject* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Stab "+_w->getName());
    vInteract->push("Slash "+_w->getName());
    
    return vInteract;
  }
  
    Vector <std::string>* Item_Log::getInteractNames(Item* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
    Vector <std::string>* Item_Log::getInteractNames(Character* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
    Vector <std::string>* Item_Log::getInteractNames(Creature* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
  Vector <std::string>* Item_Log::getInteractNames(LocalTile* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Build campfire");
    vInteract->push("Build planks");

    return vInteract;
  }
  
  void Item_Fish::interact (WorldObject* _item, int interactType /* =0 */)
  {
      // Check if we can cook with this object
    if ( _item->canCook )
    {
      
      Console ("You cook the fish");
      isCooked=true;
    }
  }

  void Item_Fish::interact (Character* _character)
  {
    Console ("You eat the fish");
    owner->consume(this);
  }
  
  
  
  // SWORD
  
  Vector <std::string>* Item_Sword::getInteractNames(WorldObject* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Stab "+_w->getName());
    vInteract->push("Slash "+_w->getName());
    
    return vInteract;
  }
  
    Vector <std::string>* Item_Sword::getInteractNames(Item* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
    Vector <std::string>* Item_Sword::getInteractNames(Character* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
    Vector <std::string>* Item_Sword::getInteractNames(Creature* _w)
    {
      if (_w==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_w->getName());
      vInteract->push("Slash "+_w->getName());
      
      return vInteract;
    }
  Vector <std::string>* Item_Sword::getInteractNames(LocalTile* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Stab "+_w->getName());
    vInteract->push("Slash "+_w->getName());
    
    return vInteract;
  }
  
#endif
