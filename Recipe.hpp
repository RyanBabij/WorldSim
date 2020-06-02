#pragma once
#ifndef WORLDSIM_RECIPE_HPP
#define WORLDSIM_RECIPE_HPP

/* Recipe.hpp
   #include "Recipe.hpp"
  
A recipe is any collection of objects which can be consumed to make another object. Usually there are three types
of recipes: Alchemy, crafting and cooking. I think most inputs/outputs will be items. But hey you never know.

There are a lot of things to deal with here. For example what if you can make an object using either 2 wood or 2 metal,
but not one of each? For now I'm bypassing this by just creating a class for every non-inclusive recipe. So for example
you can have a Wooden_Wall_Recipe and a Stone_Wall_Recipe.

There's a Crafting Manager to figure out what can and can't be made from the inventory. It uses double dispatch.
Basically each Item sends itself to the Manager which sorts it into each recipe. The recipes recieve the items if they
are relevant and keep an internal total of how many valid items they have been given. From that we can easily obtain
a list of all recipes which can be made with the current set of items. We can also figure out what an item can be used
for.

I have no idea if this implementation is a good one. However I believe Minecraft just used typeId so fuck it, we'll
do it live.

A proper polymorphic abstraction system would probably break up craftable items into components. However realistically
I don't think that's going to happen because some Recipes might depend on outputs from other recipes.

One alternate possibility is having certain Items inherit a Craftable interface. But idk if it'll work out.

Recipe_Wooden_Wall and Recipe_Stone_Wall should link to each other as variants, so they can be indicated as such in GUI

Recipes could probably be grouped into categories. For example Wooden_Wall, Brick_Wall, Stone_Wall and Palisade could
all be of type Wall. The AI might mainly concern itself with building a wall, not the type of wall.

A Wall Manager could decide which wall the AI should build in the circumstances.

It's possible to override this system by using getName(), which basically functions as a typeID system.

*/

class Item_Log;
class Item_Plank;
class Item_DeerPelt;

class Recipe
{
  public:
  
  Vector <WorldObject*> vInput;
  Item* output;
  
  Recipe();
  ~Recipe();
  
  /* Add a requirement to make this recipe. count is how many of the object you need. isConsumed true if the items
  are removed from the world when you make this recipe */
  void addRequirement(WorldObject* _object, int count, bool isConsumed);
  void addRequirement(Item* _object, int count, bool isConsumed);
  
  
  // Using double dispatch because I can't think of anything better
  // In future we might decide on a small list of raw crafting inputs and just worry about those.
  virtual int canUse (WorldObject*) /* return the amount of object needed. -1 means this object can't be used */
  { return -1; }
    // Every item must be listed here in order to specialise the base class.
  virtual int canUse (Item *) { return -1; }
  virtual int canUse (Item_Plank *) { return -1; }
  virtual int canUse (Item_Log*) { return -1; }
  virtual int canUse (Item_Fish *) { return -1; }
  virtual int canUse (Item_DeerPelt *) { return -1; }
  
  
  /* These should probably be in recipe manager. */
  /* recipe will take object and add it up to determine how many of this recipe can be made */
  virtual void countUp(WorldObject*) {}
  virtual void countUp(Item*) {}
  virtual void countUp(Item_Plank*) {}
  virtual void countUp(Item_Log*) {}
  virtual void countUp(Item_Fish*) {}
  virtual void countUp(Item_DeerPelt*) {}
  
  virtual void make(Character*) { std::cout<<"RECIPE MAKED\n"; }
  
  virtual int getTotal()
  {
    return 0;
  }
  
  virtual std::string getName()
  {
    return "Generic recipe";
  }
  
};

class Recipe_Wall: public Recipe
{
  public:
  Vector <Item_Plank*> vPlank;

  virtual int canUse (Item_Plank * _object) override /* return the amount of object needed. -1 means this object can't be used */
  {
    std::cout<<"Item_Plank\n";
    return 3;
  }
  
  virtual std::string getName() override
  {
    return "Wooden Wall";
  }
  
  virtual void countUp(WorldObject* _object) override {}
  virtual void countUp(Item* _item) override {}
  virtual void countUp(Item_Plank* _plank) override
  {
    vPlank.push(_plank);
  }
  
  virtual int getTotal() override
  {
    // 5 planks to make a wall.
    return (vPlank.size()/2);
  }
};
Recipe_Wall recipeWall;

class Recipe_GrilledFish: public Recipe
{
  public:
  
  Vector <Item_Fish*> vFish;

  virtual int canUse (Item_Fish * _object) override /* return the amount of object needed. -1 means this object can't be used */
  {
    std::cout<<"Item_Fish\n";
    return 3;
  }
  
  virtual std::string getName() override
  {
    return "Grilled fish";
  }
  
  virtual void countUp(WorldObject* _object) override {}
  virtual void countUp(Item* _item) override {}
  virtual void countUp(Item_Fish* _plank) override
  {
    std::cout<<"Addfish\n";
    vFish.push(_plank);
  }
  
  virtual int getTotal() override
  {
    std::cout<<"FISH: "<<vFish.size()<<"\n";
    // 5 planks to make a wall.
    return vFish.size();
  }
};
Recipe_GrilledFish recipeGrilledFish;

// LEATHER CLOTHES
// 5 of any pelt
class Recipe_LeatherClothes: public Recipe
{
  public:
  
  Vector <Item_DeerPelt*> vPelt;

  virtual int canUse (Item_DeerPelt * _object) override /* return the amount of object needed. -1 means this object can't be used */
  {
    std::cout<<"Pelts\n";
    return 1;
  }
  
  virtual std::string getName() override
  {
    return "Leather clothes";
  }
  
  virtual void countUp(WorldObject* _object) override {}
  virtual void countUp(Item* _item) override {}
  virtual void countUp(Item_DeerPelt* _input) override
  {
    std::cout<<"Add pelt\n";
    vPelt.push(_input);
  }
  
  virtual int getTotal() override
  {
    std::cout<<"PELTS: "<<vPelt.size()<<"\n";
    // 5 pelts to make clothes.
    return vPelt.size();
  }
  
  virtual void make(Character* _character) override;

};
Recipe_LeatherClothes recipeLeatherClothes;

// Waterskin
// 1 of any pelt
class Recipe_Waterskin: public Recipe
{
  public:
  
  Vector <Item_DeerPelt*> vPelt;

  virtual int canUse (Item_DeerPelt * _object) override /* return the amount of object needed. -1 means this object can't be used */
  {
    std::cout<<"Pelts\n";
    return 1;
  }
  
  virtual std::string getName() override
  {
    return "Waterskin";
  }
  
  virtual void countUp(WorldObject* _object) override {}
  virtual void countUp(Item* _item) override {}
  virtual void countUp(Item_DeerPelt* _input) override
  {
    std::cout<<"Add pelt\n";
    vPelt.push(_input);
  }
  
  virtual int getTotal() override
  {
    std::cout<<"PELTS: "<<vPelt.size()<<"\n";
    // 5 pelts to make clothes.
    return vPelt.size();
  }
  
  virtual void make(Character* _character) override;

};
Recipe_Waterskin recipeWaterskin;

// Log Shelter
// 1 of any pelt
class Recipe_LogShelter: public Recipe
{
  public:
  
  Vector <Item_Log*> vInput;

  virtual int canUse (Item_Log * _object) override /* return the amount of object needed. -1 means this object can't be used */
  {
    return 1;
  }
  
  virtual std::string getName() override
  {
    return "Shelter";
  }
  
  virtual void countUp(WorldObject* _object) override {}
  virtual void countUp(Item* _item) override {}
  virtual void countUp(Item_Log* _input) override
  {
    std::cout<<"Adding log shelt\n";
    vInput.push(_input);
  }
  
  virtual int getTotal() override
  {
    std::cout<<" log shelt\n";
    // 5 pelts to make clothes.
    return vInput.size();
  }
  
  virtual void make(Character* _character) override;

};
Recipe_LogShelter recipeLogShelter;

// Inventory manager can give 2 types of information. What recipes can be made with current items. And also what reciped
// an item can be used in.
class RecipeManager
{
  public:
  
  int x,y; /* Where to put output */
  
  /* List of Recipes which can be made based on inputs */
  Vector <Recipe*> vValidList;
  
  //put one of every recipe here
  
  Vector <Recipe*> vRecipe;
  
  int canUse (Item_Plank* _object)
  {
    return -1;
  }

  void addRecipe(Recipe* _recipe)
  {
    vRecipe.push(_recipe);
  }
  
  void addToRecipes (Item*)
  {
    std::cout<<"Adding item\n";
  }
  void addToRecipes(Item_Plank* _item)
  {
    std::cout<<"Adding plank\n";
    recipeWall.countUp(_item);
  }
  void addToRecipes(Item_Fish* _item)
  {
    std::cout<<"adding fish\n";
    recipeGrilledFish.countUp(_item);
  }
  void addToRecipes(Item_DeerPelt* _item)
  {
    recipeLeatherClothes.countUp(_item);
    recipeWaterskin.countUp(_item);
  }
  void addToRecipes(Item_Log* _item)
  {
    std::cout<<"adding log\n";
    recipeLogShelter.countUp(_item);
  }
  
  int getTotals()
  {
    return recipeWall.getTotal();
    return recipeGrilledFish.getTotal();
    return recipeLeatherClothes.getTotal();
    return recipeWaterskin.getTotal();
    return recipeLogShelter.getTotal();
  }
  
  Vector <Recipe*> * getValidRecipes()
  {
    vValidList.clear();
    if ( recipeWall.getTotal() > 0 )
    {
      vValidList.push(&recipeWall);
    }
    if ( recipeGrilledFish.getTotal() > 0 )
    {
      vValidList.push(&recipeGrilledFish);
    }
    if ( recipeLeatherClothes.getTotal() > 0 )
    {
      vValidList.push(&recipeLeatherClothes);
    }
    if ( recipeWaterskin.getTotal() > 0 )
    {
      vValidList.push(&recipeWaterskin);
    }
    if ( recipeLogShelter.getTotal() > 0 )
    {
      vValidList.push(&recipeLogShelter);
    }
    return &vValidList;
  }
  
  void makeRecipe(Character* _character, int _index)
  {
    if (vValidList.isSafe(_index))
    {
      std::cout<<"Making: "<<vValidList(_index)->getName()<<".\n";
      vValidList(_index)->make(_character);
    }
  }
  
  std::string getPrerequisites()
  {
    std::string sPrerequisites = "";
    for (int i=0;i<recipeGrilledFish.vInput.size();++i)
    {
      sPrerequisites+=recipeGrilledFish.vInput(i)->getName();
    }
    return sPrerequisites;
  }
  
  // int canMakeRecipe(Recipe_Wall* _recipe, Vector <Item*> vInventory)
  // {
    // int nPlank=0;
    // // for (int i=0;i<vInventory.size();++i)
    // // {
      // // if (recipeWall.canUse(vInventory(i))
      // // {
        // // ++nPlank;
      // // }
    // // }
    // return nPlank;
  // }

  
};
RecipeManager recipeManager;

#endif
