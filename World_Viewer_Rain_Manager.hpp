#pragma once
#ifndef WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_HPP
#define WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_HPP

/* WorldSim: WorldViewer Rain Manager
	#include "World_Viewer_Rain_Manager.hpp"

	Controls rendering of rain.
*/

#include <Container/Vector/Vector.hpp>
//#include "RandomNonStatic.hpp" // Assuming RandomNonStatic is a custom class that needs to be included.
//#include "World.hpp" // Assuming World is a custom class that needs to be included.
// Add other necessary includes here

// Raindrops that are rendered over screen to indicate rain.
// Should start at top of screen and travel down. Should not render on sheltered tiles
class RainDrop
{
public:
	int x, y;

	RainDrop();
	void drop(int amount);
	void render();
};

class RainManager
{
private:
	RandomNonStatic rand;
	Vector<RainDrop*> vRainDrop;
	World* world;
	int x1, x2;
	int y1, y2;
	int MAX_RAIN;
	int RAIN_PER_FRAME;

public:
	RainManager();
	void init(int _x1, int _y1, int _x2, int _y2, World* _world, int _maxRain=10000, double _rainPercentOfX=0.005);
	void updateRain();
	void render();
};

#endif // WORLDSIM_WORLD_VIEWER_RAIN_MANAGER_HPP
