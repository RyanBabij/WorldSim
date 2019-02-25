#pragma once
#ifndef WORLDSIM_ITEM_CPP
#define WORLDSIM_ITEM_CPP

/* Item.cpp
	#include"Item.cpp"
  
  Implementation of Item.hpp.

*/

class Ammo
{
  public:
  
  std::string name;
  int baseDamage;
  
};



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
	
	//int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
	//	int distanceFrom(WorldObject*); /* Alias. */
    
  //int distanceTo(int /* _x */, int /* _y */); /* Same, using raw coordinates */
    

	virtual Texture* currentTexture();
};

class Item_Sword: public Item
{
  public:
  
  Item_Sword()
  {
    
  }
  std::string getName() { return "Sword"; }
  
  
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
  std::string getName() { return "Longbow"; }
  
  
  Texture* currentTexture()
  {
    return &TEX_ITEM_LONGBOW;
  }

};

class Item_Fishrod: public Item
{
  public:
  
  Item_Fishrod()
  {
    
  }
  std::string getName() { return "Fishing rod"; }
  
  
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
  
  
  Texture* currentTexture()
  {
    return &TEX_ITEM_AXE;
  }

};


#endif
