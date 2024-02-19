#pragma once
#ifndef WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_CPP
#define WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_CPP


#include "World_Viewer_Rain_Manager.hpp"

RainDrop::RainDrop()
{
	x = 0;
	y = 0;
}

void RainDrop::drop(int amount)
{
	y -= amount;
}

void RainDrop::render()
{
	Renderer::placeColour4a(60, 60, 255, 255, x, y, x + 3, y + 4);
}

RainManager::RainManager() { }

void RainManager::init(int _x1, int _y1, int _x2, int _y2, World* _world, int _maxRain, double _rainPercentOfX)
{
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	MAX_RAIN = _maxRain;
	RAIN_PER_FRAME = static_cast<int>(_rainPercentOfX * (x2 - x1));

	world = _world;

	rand.seed();
}

void RainManager::updateRain()
{
	if (world->isRaining && vRainDrop.size() < MAX_RAIN)
	{
		for (int i = 0; i < RAIN_PER_FRAME; ++i)
		{
			RainDrop* rain = new RainDrop;
			rain->y = y2 - rand.randomInt(20);

			rain->x = rand.randomInt(x2 - x1) + x1;
			vRainDrop.push(rain);
		}
	}
	for (int i = 0; i < vRainDrop.size(); ++i)
	{
		vRainDrop(i)->drop(rand.randomInt(3) + 18);
		if (vRainDrop(i)->y < y1)
		{
			delete vRainDrop(i);
			vRainDrop.removeSlot(i);
			--i;
		}
	}
}


void RainManager::render()
{
	if (world==0 || worldViewer.pixelsPerLocalTile < 1)
	{return;}

	unsigned long int bX,bY;
	worldViewer.toTileCoords(0,0,&bX,&bY);

	Renderer::setColourMode();
	glColor3ub(0,0,220);
	for (int i=0;i<vRainDrop.size();++i)
	{
		int rX = vRainDrop(i)->x;
		int rY = vRainDrop(i)->y;
		unsigned long int aX,aY;

		worldViewer.toTileCoords(rX,rY,&aX,&aY);

		if ( world->isGenerated(aX,aY) )
		{
			if ( (*world)(aX,aY)->hasFloor == 0 )
			{
				vRainDrop(i)->render();
			}
		}
		else
		{
			vRainDrop(i)->render();
		}
	}
	glColor3ub(255,255,255);
	Renderer::setTextureMode();
}


#endif // WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_CPP