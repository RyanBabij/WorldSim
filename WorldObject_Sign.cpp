#pragma once
#ifndef GUILD_WORLDOBJECT_SIGN_CPP
#define GUILD_WORLDOBJECT_SIGN_CPP

#include"WorldObject_Sign.hpp"

/* WorldObject_Sign.cpp
   #include"WorldObject_Sign.cpp"
   <DOCUMENTATION>
*/

WorldObject_Sign::WorldObject_Sign()
{
   x=0; y=0;
   weight=0;
   bulk=0;
  
  signText = "";
}

std::string WorldObject_Sign::getName()
{
   return "Sign";
}

Texture* WorldObject_Sign::currentTexture()
{
   return &TEX_LOCAL_SIGN;
}

#endif
