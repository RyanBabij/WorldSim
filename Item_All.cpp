#pragma once
#ifndef WORLDSIM_ITEM_ALL_CPP
#define WORLDSIM_ITEM_ALL_CPP

/* Item_All.cpp
   #include"Item.cpp"
  
  Implementation of Item.hpp.

*/

  // HAND
  
Vector <std::string>* Item_Hand::getInteractNames(WorldObject* _target)
{
      if (_target==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
        // Temporary cheat using getName().
      if ( _target->getName() == "Plant" )
      {
        vInteract->push("Harvest plant fibres");
      }
      
      vInteract->push("Punch "+_target->getName());
      return vInteract;
}
void Item_Hand::interact(WorldObject* _target, int interactType /* =0 */)
{
  if (_target==0 || owner==0)
  { return; }

  Console("You harvest some plant fibres");
  owner->giveItem(new Item_PlantFibre);
}

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

  
    //FISHING ROD
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
  Vector <std::string>* Item_Fishrod::getInteractNames(LocalTile* _target)
  {
    if (_target==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Fish on the "+_target->getName());
    return vInteract;
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
    
    //auto vInteract = new Vector <std::string>;
    //vInteract->push("Stab "+_w->getName());
    //vInteract->push("Slash "+_w->getName());
    return 0;
    
    //return vInteract;
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
  void Item_Log::addToRecipeManager()
  {
    recipeManager.addToRecipes(this);
  }
  
    // FISH
  Vector <std::string>* Item_Fish::getInteractNames(Item* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    if (_w->canCook)
    {
      vInteract->push("Cook fish");
    }    
    return vInteract;
  }
  Vector <std::string>* Item_Fish::getInteractNames(Character* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
      vInteract->push("Eat fish");
    return vInteract;
  }
    
  void Item_Fish::interact (Item* _item, int interactType /* =0 */)
  {
      // Check if we can cook with this object
    if ( _item->canCook )
    {
      
      Console ("You cook the fish");
      isCooked=true;
    }
  }
  
  void Item_Fish::interact (WorldObject* _target, int interactType /* =0 */)
  {
      // Check if we can cook with this object
    Console ("Generic fish");
  }

  void Item_Fish::interact (Character* _character, int interactType /* =0 */)
  {
     if (isCooked)
     {
        Console ("You eat the cooked fish");
       owner->consume(this);
     }
     else
     {
        Console ("Nice sushi");
       owner->consume(this);
     }

     

  }
  
  void Item_Fish::addToRecipeManager()
  {
    recipeManager.addToRecipes(this);
  }

   // DEER MEAT
   
  Vector <std::string>* Item_DeerMeat::getInteractNames(Item* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    if (_w->canCook)
    {
      vInteract->push("Cook meat");
    }    
    return vInteract;
  }
  Vector <std::string>* Item_DeerMeat::getInteractNames(Character* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
      vInteract->push("Eat deer meat");
    return vInteract;
  }
    
  void Item_DeerMeat::interact (Item* _item, int interactType /* =0 */)
  {
      // Check if we can cook with this object
    if ( _item->canCook )
    {
      
      Console ("You cook the deer meat");
      isCooked=true;
    }
  }
  
  void Item_DeerMeat::interact (WorldObject* _target, int interactType /* =0 */)
  {
      // Check if we can cook with this object
    Console ("Generic deer meat");
  }

  void Item_DeerMeat::interact (Character* _character, int interactType /* =0 */)
  {
     if (isCooked)
     {
        Console ("You eat the deer meat");
       owner->consume(this);
     }
     else
     {
        Console ("It's fucking raw");
     }
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
  
  void Item_Sword::interact (Character* _target, int interactType /* =0 */)
  {
    if ( owner && _target )
    {
      if ( owner->distanceTo(_target) > 1 )
      {
        Console("Too far away");
      }
      else
      {
        Console ("You stab the character");
        _target->isAlive=false;
      }
    }
    else
    {
      std::cout<<"Error: No owner or target.\n";
    }
    

  }
  
  void Item_Sword::interact (Creature* _target, int interactType /* =0 */)
  {
    if ( owner && _target )
    {
      if ( owner->distanceTo(_target) > 1 )
      {
        Console("Too far away");
      }
      else
      {
        Console ("You stab the creature");
        _target->isAlive=false;
      }
    }
    else
    {
      std::cout<<"Error: No owner or target.\n";
    }
  }
  
  
  // KNIFE
  
    Vector <std::string>* Item_Knife::getInteractNames(Creature* _target)
    {
      if (_target==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_target->getName());
      
      if ( _target->isAlive == false )
      {
        vInteract->push("Skin "+_target->getName());
        vInteract->push("Butcher "+_target->getName());
      }
      
      return vInteract;
    }
    Vector <std::string>* Item_Knife::getInteractNames(Character* _target)
    {
      if (_target==0) { return 0; }
      
      auto vInteract = new Vector <std::string>;
      vInteract->push("Stab "+_target->getName());
      
      if ( _target->isAlive == false )
      {
        vInteract->push("Skin "+_target->getName());
        vInteract->push("Butcher "+_target->getName());
      }
      
      return vInteract;
    }
    
    void Item_Knife::interact (Creature* _target, int interactionType /* =0 */)
    {
      if ( interactionType==0 && _target->isAlive==false )
      {
        Console("It's already dead mate");
      }
      if ( interactionType==0 && _target->isAlive )
      {
        Console("The deer frolics no more");
        _target->isAlive=false;
      }
      if ( interactionType==1 )
      {
        if (_target->nPelt > 0)
        {
          _target->nPelt--;
          auto _pelt = new Item_DeerPelt;
          owner->giveItem(_pelt);
        }
        else
        {
          Console("No more pelts");
        }
      }
      if ( interactionType==2 )
      {
        if (_target->nMeat > 0)
        {
          _target->nMeat--;
          owner->giveItem(new Item_DeerMeat);
        }
        else
        {
          Console("No more meats");
        }
      }
    }
    
    void Item_Knife::interact (Character* _target, int interactionType /* =0 */)
    {
      if ( interactionType==0 && _target->isAlive==false )
      {
        Console("It's already dead mate");
      }
      if ( interactionType==0 && _target->isAlive )
      {
        Console("You stab "+_target->getName());
        _target->isAlive=false;
      }
      if ( interactionType==1 )
      {
        if (_target->nPelt > 0)
        {
          _target->nPelt--;
          auto _pelt = new Item_HumanSkin;
          owner->giveItem(_pelt);
        }
        else
        {
          Console("No more pelts");
        }
      }
      if ( interactionType==2 )
      {
        if (_target->nMeat > 0)
        {
          _target->nMeat--;
          owner->giveItem(new Item_HumanMeat);
        }
        else
        {
          Console("No more meats");
        }
      }
    }
    
  //Plank
  
void Item_Plank::addToRecipeManager()
{
  recipeManager.addToRecipes(this);
}

  //WALL
  
  Vector <std::string>* Item_Wall::getInteractNames(LocalTile* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Build wall north");
    vInteract->push("Build wall east");
    vInteract->push("Build wall south");
    vInteract->push("Build wall west");
    vInteract->push("Build wall NE");
    vInteract->push("Build wall SE");
    vInteract->push("Build wall SW");
    vInteract->push("Build wall NW");
    vInteract->push("Demolish wall");
    
    return vInteract;
  }
  void Item_Wall::interact (LocalTile* obj, int interactionType /* =0 */)
  {
    Console ("We're building a wall, and the Elves are going to pay for it");
    
    if ( interactionType==0) /* North */
    {
      obj->bWall = 0b10001000;
    }
    else if (interactionType==1) /* East */
    {
      obj->bWall = 0b01000100;
    }
    else if (interactionType==2) /* South */
    {
      obj->bWall = 0b00100010;
    }
    else if (interactionType==3) /* West */
    {
      obj->bWall = 0b00010001;
    }
    else if (interactionType==4) /* NE */
    {
      obj->bWall = 0b11001100;
    }
    else if (interactionType==5) /* SE */
    {
      obj->bWall = 0b01100110;
    }
    else if (interactionType==6) /* SW */
    {
      obj->bWall = 0b00110011;
    }
    else if (interactionType==7) /* NW */
    {
      obj->bWall = 0b10011001;
    }
    else /* Demolish */
    {
      obj->bWall = 0b00000000;
    }
  }
  
  //FLOOR
  Vector <std::string>* Item_Floor::getInteractNames(LocalTile* _w)
  {
    if (_w==0) { return 0; }
    
    auto vInteract = new Vector <std::string>;
    vInteract->push("Build floor");
    vInteract->push("Demolish floor");
    
    return vInteract;
  }
  void Item_Floor::interact (LocalTile* obj, int interactionType /* =0 */)
  {
    if ( interactionType==0) /* Build floor */
    {
       Console("Building floor");
      obj->hasFloor = 2;
    }
    else /* Demolish floor */
    {
      obj->hasFloor = 0;
    }
  }
  
Vector <std::string>* Item_Longbow::getInteractNames(Creature* _target)
{
  if (_target==0) { return 0; }
  
  auto vInteract = new Vector <std::string>;
  vInteract->push("Shoot "+_target->getName());
  return vInteract;
}
void Item_Longbow::interact(Creature* _target, int interactType)
{
  if (_target==0 || owner == 0) { return; }
  
  
  
  Console("You shoot the "+_target->getName());
  
  
  //Shooting calculations go here.
  
  auto vShotPath = new Vector <HasXY2 <unsigned long int> *>;
  
  world.rayTrace(owner->fullX,owner->fullY,_target->fullX,_target->fullY,vShotPath);
  
  
  for (int i=0;i<vShotPath->size();++i)
  {
    HasXY2 <unsigned long int> * currentXY = (*vShotPath)(i);

    if (currentXY->x == _target->fullX && currentXY->y == _target->fullY)
    {
      Console("Shot hits");
      _target->die();
      return;
    }
  }
  Console("Shot misses");
}

// PICKAXE

Vector <std::string>* Item_Pickaxe::getInteractNames(LocalTile* _w)
{
  if (_w==0) { return 0; }
  
  auto vInteract = new Vector <std::string>;
  vInteract->push("Mine tile");
  return vInteract;
}

void Item_Pickaxe::interact (LocalTile* _tile, int interactionType /* =0 */)
{
  // For testing you can catch a fish on any tile.
  //if (_tile->getName() == "Ocean")
  if (_tile->nGems>0)
  {
    Console("Mining gems");
  }
  else if (_tile->nMetal>0)
  {
    Console("Mining metal");
  }
  else
  {
    Console("Mining dirt");
    _tile->baseTerrain=DESERT;
  }

}

// DEER PELT

void Item_DeerPelt::addToRecipeManager()
{
  recipeManager.addToRecipes(this);
}

  
#endif
