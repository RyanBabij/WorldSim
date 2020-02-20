#pragma once
#ifndef WORLDSIM_STATIC_HPP
#define WORLDSIM_STATIC_HPP

/* WorldSim:Static.hpp
   #include "Static.hpp"

   Statics are objects which can't move on their own, and therefore
   they don't need to know their coordinates.
   They also only tick once per day.

   Static subclasses:
      Plant
      Tree
      Building
      Furniture

   Statics may block movement and/or LOS
*/

class Static: public HasTexture
{
   public:
   unsigned char blockMove;
   unsigned char blockLOS;
   
   // HP
   // DAMAGE RESIST
   
   Static()
   {
   }
   
   virtual Texture* currentTexture()
   {
      return 0;
   }
};

// object capable of holding Ingredients
class Harvestable
{
   public:
};

class Flora: public Static
{
   public:
   Flora()
   {
   }
};

class Static_Tree: public Static
{
   public:
   
   Static_Tree()
   {
   }
   
   // Texture* currentTexture()
   // {
      // // //if (chopAmount==0)
      // // { return &TEX_OBJECT_STUMP;
      // // }

      // // //if ( growth==0 )
      // // {
         // // return &TEX_WORLD_TERRAIN_FOREST_SAPLING;
      // // }
      // return &TEX_WORLD_TERRAIN_FOREST_TREE;
   // }
};

#endif
