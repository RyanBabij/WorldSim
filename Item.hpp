#pragma once
#ifndef WORLDSIM_ITEM_HPP
#define WORLDSIM_ITEM_HPP

/* Item.cpp
	#include"Item.hpp"
  
  An item is an object which can be picked up and manipulated. Items are reliant on
  actors (Creatures/Characters) to be moved or used.
  
  Items cannot interfere with movement or line of sight.
  
  In the future stacking should be implemented for identical objects.
  
  Only items can be stored in an inventory.
  
  Item interaction is currently handled using double dispatch. It's possible to override this by using getName() as
  a typeID.

*/

class Ammo
{
  public:
  
  std::string name;
  int baseDamage;
  
};

#include "LocalTile.hpp"
//#include "Character.hpp"

class Character;
class Recipe;


class Item: public WorldObject
{
	public:
	//int x, y;
  int count; /* How many of this object there is (for example ammo) */
  
  int type; /* The type of item. */
  
  int meleeDamage; /* Base damage if you hit someone with this. 0 = not possible. */
  int throwDamage; /* Base damage if you throw this at someone. 0 = not possible */
  
  /* Ammunition might as well be hardcoded since there won't be too many different types. */
  char ammunitionType;

  
  Character* owner; /* Set to 0 if nobody owns it */
  //Character* creator; /* Character who created this item */
  
  
    // CONSUMABLE PROPERTIES
  int consumeTime; /* How long it takes to eat this. 0 = no time cost. -1 = you can't consume it. */
  int hungerRestore; /* -1 = you can't eat it. 0 = you can eat it but get no hunger restored */
  
    // INTERACTION PROPERTIES (currently Item specialisation isn't implemented,
    // so Items can only interact with WorldObject).

  
  
  //Implementing full global coordinates to make life easier for now.
  // This datatype should be able to hold coordinates for any reasonably-sized world.
  //long unsigned int fullX, fullY;
  
  
    /* TEMPORARY WORKAROUND TO ALLOW CHARACTERS TO MOVE ACROSS MAPS.
      CURRENTLY ONLY USED FOR CHARACTERS
      In future World_Local should distinguish between actors and objects */
  //int worldX, worldY;
	
	//bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	//double weight; /* in grams */
	//double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
  
  /* True if line of sight cannot pass this object.
    Might be expanded in future. For example maybe a
    crate will block view of people who are sneaking. */
  //bool blocksView;
  
  /* True if characters and creatures cannot walk into this tile. */
  //bool blocksMovement;
	
	Item();
	virtual ~Item();
  
  
	virtual std::string getName();
	virtual std::string getExtendedInfo();
	
  
  /* ITEM INTERACTIONS
    List of stuff the item can do.
  */
  
    // ATTACK
    // Anything is a weapon if you're desperate enough.
    int slashDamage; /* Causes direct damage. */
    int bluntDamage; /* Bypasses armour values and causes limb damage. */
    int pierceDamage; /* Increases critical chance (piercing vital organs), but limited by armour. */
    float throwMultiplier; /* How well this weapon can be thrown. Most weapons are less effective when thrown. */
  
  // Fishing
  // ChopTree
  
    //Cost is base action points required. -1 means the action is not possible. 0 means no cost.
  int chopTreeCost;
  int fishingCost;
  
  // Used so Player can easily see what recipes an object is used for.
  virtual bool canUseInRecipe(Recipe* _recipe)
  {
    std::cout<<"1\n";
    return false;
  }
	
	//int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
	//	int distanceFrom(WorldObject*); /* Alias. */
    
  //int distanceTo(int /* _x */, int /* _y */); /* Same, using raw coordinates */
  
  
    // OBJECT INTERACTION
    // In future these functions might need to be expanded to return multiple possibilities, for example
    // "Stab target" and "Slash target". Or "Chop down door" and "Pry open door".
    // The good thing about this approach is that it can list out all possible interactions between objects,
    // which I think is better than making the player guess what everything does.
    // There are different types of objects, such as Items, WorldObjects and LocalTiles.
  virtual void interact (WorldObject* obj, int interactionType=0)
  {
    std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }
  virtual void interact (LocalTile* obj, int interactionType=0)
  {
    std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }
  virtual void interact (Item* obj, int interactionType=0)
  {
    std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }
  virtual void interact (Character* obj, int interactionType=0)
  {
    std::cout<<"Char interact\n";
    //std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }
  virtual void interact (Creature* obj, int interactionType=0)
  {
    std::cout<<"Creature interact\n";
    //std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }

    // HOW LONG THIS TASK WILL TAKE. -1 MEANS YOU CAN'T DO IT. 0 MEANS NO TIME COST.
  virtual int interactTime(WorldObject* _w)
  {
    return 0;
  }
    // Same but for LocalTile object. Used for interaction with terrain.
  virtual int interactTime(LocalTile* _w)
  {
    return 0;
  }
    // Describe the interaction. For example: "Chop down tree.
    // Note, this should be updated to be a vector, because an object can in some cases do multiple things to an object
  virtual Vector <std::string>* getInteractNames(WorldObject* _w)
  {
    return 0;
  }
    virtual Vector <std::string>* getInteractNames(Item* _w)
    {
      return 0;
    }
    virtual Vector <std::string>* getInteractNames(Character* _w)
    {
      return 0;
    }
    virtual Vector <std::string>* getInteractNames(Creature* _w)
    {
      return 0;
    }
  virtual Vector <std::string>* getInteractNames(LocalTile* _w)
  {
    return 0;
  }

  
    // Useful for checking recipe requirements.
  bool sameType (WorldObject _obj)
  {
    return false;
  }
  bool sameType (Item* _item)
  {
    return true;
  }
  
  
  virtual std::string getInteractName(WorldObject* _w)
  {
    return "Interact with "+_w->getName();
  }
  virtual std::string getInteractName(LocalTile* _w)
  {
    return "Interact with "+_w->getName();
  }
  
  
  virtual void addToRecipeManager();

    

	virtual Texture* currentTexture();
};

// So it has come to this
// Obviously you can't call a function for a null pointer, however hand is the default (null) action
// therefore an object must be created to deal with these interactions.
// An alternative is to simply have Character handle this. However I like this approach because it keeps
// Interactions together.
class Item_Hand: public Item
{
  public:
  
  Item_Hand()
  {
    
  }
  std::string getName() override { return "Hand"; }
  
  virtual Vector <std::string>* getInteractNames(WorldObject*) override;
    // virtual Vector <std::string>* getInteractNames(Item* _w) override;
    // virtual Vector <std::string>* getInteractNames(Character* _w) override;
    // virtual Vector <std::string>* getInteractNames(Creature* _w) override;
  // virtual Vector <std::string>* getInteractNames(LocalTile* _w) override;
  
  virtual void interact(WorldObject*, int interactType=0) override;

  // Dummy function
  Texture* currentTexture() override
  {
    return &TEX_PORTRAIT_SNEK;
  }

};
Item_Hand itemHand;

class Item_Sword: public Item
{
  public:
  
  Item_Sword()
  {
    
  }
  std::string getName() { return "Sword"; }
  
  virtual Vector <std::string>* getInteractNames(WorldObject* _w);
    virtual Vector <std::string>* getInteractNames(Item* _w);
    virtual Vector <std::string>* getInteractNames(Character* _w);
    virtual Vector <std::string>* getInteractNames(Creature* _w);
  virtual Vector <std::string>* getInteractNames(LocalTile* _w);

  Texture* currentTexture()
  {
    return &TEX_ITEM_SWORD;
  }

};

class Item_Longbow: public Item
{
  public:
  
  Item_Longbow()
  {
    
  }
  std::string getName() override { return "Longbow"; }
  
  
  Texture* currentTexture() override
  {
    return &TEX_ITEM_LONGBOW;
  }
  
  virtual Vector <std::string>* getInteractNames(Creature* _target) override;
  
  virtual void interact(Creature* _target, int interactType=0) override;

};

class Item_Fishrod: public Item
{
  public:
  
  Item_Fishrod()
  {
    
  }
  std::string getName() { return "Fishing rod"; }
  
  virtual void interact (LocalTile* _tile, int interactionType = 0);
  

  void interact(WorldObject* _w, int interactType=0)
  {
    std::cout<<"You hook the "+_w->getName();
  }
  
  std::string getInteractName(WorldObject* _w)
  {
    return "Fish in "+_w->getName();
  }
  std::string getInteractName(LocalTile* _w)
  {
    return "Cast fishing rod: "+_w->getName();
  }
  
  virtual int interactTime(WorldObject* _w)
  {
    return -1;
  }
  virtual int interactTime(LocalTile* _w)
  {
    if ( _w->getName() == "Ocean")
    {
      return 30;
    }
    return -1;
  }
  
  virtual Vector <std::string>* getInteractNames(LocalTile* _w);
  
  Texture* currentTexture()
  {
    return &TEX_ITEM_FISHROD;
  }

};

class Item_Shovel: public Item
{
  public:
  
  Item_Shovel()
  {
    
  }
  std::string getName() { return "Shovel"; }
  
  
  Texture* currentTexture()
  {
    return &TEX_ITEM_SHOVEL;
  }

};

class Item_Axe: public Item
{
  public:
  
  Item_Axe()
  {
    
  }
  std::string getName() { return "Axe"; }
  
  
  void interact(WorldObject* w, int interactType=0);
  
  
  virtual Vector <std::string>* getInteractNames(WorldObject* _w);
    virtual Vector <std::string>* getInteractNames(Item* _w);
    virtual Vector <std::string>* getInteractNames(Character* _w);
    virtual Vector <std::string>* getInteractNames(Creature* _w);
  virtual Vector <std::string>* getInteractNames(LocalTile* _w);

  virtual void interact (LocalTile* obj, int interactionType = 0)
  {
    std::cout<<"You chop the TILE "<<obj->getName()<<".\n";
  }
  virtual void interact (Item* obj, int)
  {
    std::cout<<"You chop the ITM "<<obj->getName()<<".\n";
  }
  virtual void interact (Character* obj, int interactionType=0)
  {
    //std::cout<<"You chop the CHARACTER "<<obj->getName()<<".\n";
  }
  
  std::string getInteractName(WorldObject* _w)
  {
    return "Chop "+_w->getName();
  }
  std::string getInteractName(LocalTile* _w)
  {
    return "Chop the "+_w->getName();
  }
  
  virtual int interactTime(WorldObject* _w)
  {
    if (_w->chopAmount > -1 )
    {
      return 5;
    }
    return -1;
  }
  
  Texture* currentTexture()
  {
    return &TEX_ITEM_AXE;
  }

};


class Item_Log: public Item
{
	public:
  
	
  Item_Log()
  {

  }
	virtual ~Item_Log() {}
  
  virtual void interact (LocalTile* obj, int interactionType = 0);
  
  virtual Vector <std::string>* getInteractNames(WorldObject* _w);
    virtual Vector <std::string>* getInteractNames(Item* _w);
    virtual Vector <std::string>* getInteractNames(Character* _w);
    virtual Vector <std::string>* getInteractNames(Creature* _w);
  virtual Vector <std::string>* getInteractNames(LocalTile* _w);


  
  // We need to have 2 interactions:
  // Light a fire
  // Build a wall
  std::string getInteractName(LocalTile* _w)
  {
    return "Light a fire on the "+_w->getName();
  }
  
  virtual int interactTime(WorldObject* _w)
  {
    return -1;
  }
  
  virtual int interactTime(LocalTile* _w)
  {
    return 30;
  }
  
	virtual std::string getName()
  {
    return "Log";
  }

  Texture* currentTexture()
  {
    return &TEX_OBJECT_LOG;
  }

  
};

class Item_Fish: public Item
{
  bool isCooked;
  
	public:
  

	
  Item_Fish()
  {
    consumeTime = 5;
    hungerRestore = 200;
    isCooked = false;
  }
	virtual ~Item_Fish() {}
  
  
	virtual std::string getName() override
  {
    if (isCooked)
    {
      return "Cooked fish";
    }
    return "Fish";
  }
  
  virtual void interact (Item*, int interactType=0) override; /* cook */
  virtual void interact (Character*, int interactType=0) override; /* eat */
  virtual void interact (WorldObject*, int interactType=0) override; /* eat */

  Texture* currentTexture() override
  {
    if (isCooked)
    {
      return &TEX_ITEM_FOOD_COOKED_FISH;
    }
    return &TEX_OBJECT_FISH;
  }
  
  virtual Vector <std::string>* getInteractNames(Item* _w) override;
  virtual Vector <std::string>* getInteractNames(Character* _w) override;

  virtual void addToRecipeManager() override;
};

class Item_Campfire: public Item
{

  
	public:
  
	
  Item_Campfire()
  {
    canCook=true;
  }
	virtual ~Item_Campfire() {}
  
  
	virtual std::string getName()
  {
    return "Campfire";
  }

  Texture* currentTexture()
  {
    return &TEX_OBJECT_CAMPFIRE[CURRENT_ANIMATION_FRAME%5];
  }

  
};

#include "Recipe.hpp"

class Item_Plank: public Item
{
  public:
  
  Item_Plank()
  {
  }
  virtual ~Item_Plank()
  {
  }
  
  // Used so Player can easily see what recipes an object is used for.
  virtual bool canUseInRecipe(Recipe* _recipe)
  {
    if (_recipe==0) { return false; }
    std::cout<<"2\n";
    if (_recipe->canUse(this) > 0 )
    {
      std::cout<<"Return true\n";
      return true;
    }
    std::cout<<"Return false\n";
    return false;
  }
  // Used so Player can easily see what recipes an object is used for.
  virtual bool canUseInRecipe(Recipe_Wall* _recipe)
  {
    std::cout<<"3\n";
    return true;
  }
  
  Texture* currentTexture()
  {
    return &TEX_OBJECT_PLANK;
  }
  std::string getName() { return "Plank"; }
  
  virtual void addToRecipeManager();
};

class Item_Wall: public Item
{
  public:
  
  Item_Wall()
  {
  }
  virtual ~Item_Wall()
  {
  }
  
  Vector <std::string>* getInteractNames(LocalTile* _w);
  virtual void interact (LocalTile* obj, int interactionType = 0);
  
  Texture* currentTexture()
  {
    return &TEX_WALL_GREYBRICK_SOUTH;
  }
};

class Item_Floor: public Item
{
  public:
  
  Item_Floor()
  {
  }
  virtual ~Item_Floor()
  {
  }
  
  Vector <std::string>* getInteractNames(LocalTile* _w);
  virtual void interact (LocalTile* obj, int interactionType = 0);
  
  Texture* currentTexture()
  {
    return &TEX_FLOOR_WOOD;
  }
};

class Item_PlantFibre: public Item
{
  public:
  
  Item_PlantFibre()
  {
  }
  virtual ~Item_PlantFibre()
  {
  }
  
  Texture* currentTexture() override
  {
    return &TEX_CRAFTING_FLAX;
  }
};

  // Merker for debugging
class Item_Marker_Red: public Item
{
  public:
  
  Item_Marker_Red()
  {
    
  }
  std::string getName() override { return "Debug Marker"; }

  // Dummy function
  Texture* currentTexture() override
  {
    return &TEX_MARKER_RED;
  }

};
  // Merker for debugging
class Item_Marker_Green_Small: public Item
{
  public:
  
  Item_Marker_Green_Small()
  {
    
  }
  std::string getName() override { return "Debug Marker"; }

  // Dummy function
  Texture* currentTexture() override
  {
    return &TEX_MARKER_GREEN_SMALL;
  }

};

#endif
