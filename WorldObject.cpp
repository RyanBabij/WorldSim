#pragma once
#ifndef GUILD_WORLDOBJECT_CPP
#define GUILD_WORLDOBJECT_CPP

#include"WorldObject.hpp"

/* WorldObject.cpp
	#include"WorldObject.cpp"
	<DOCUMENTATION>
*/

WorldObject::WorldObject()
{
	x=0; y=0;
	weight=0;
	bulk=0;
}

std::string WorldObject::getName()
{
	return "WorldObject";
}

int WorldObject::distanceTo (WorldObject* _obj) 
{
	return std::max(abs(x-_obj->x),abs(y-_obj->y));

	//return (abs(x-_obj->x)+abs(y-_obj->y));

	//return (sqrt(   ((abs((x-_obj->x)))*((abs((x-_obj->x))))+    ((abs((y-_obj->y)))*(abs((y-_obj->y))))    )));
}

int WorldObject::distanceFrom(WorldObject* _obj) { return distanceTo(_obj); } /* Alias. */

Texture* WorldObject::currentTexture()
{
	return &TEX_TEST_404;
}

#endif
