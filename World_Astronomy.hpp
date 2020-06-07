#pragma once
#ifndef WORLDSIM_WORLD_ASTRONOMY_HPP
#define WORLDSIM_WORLD_ASTRONOMY_HPP

/* WorldSim: World_Astronomy.hpp
	#include "World_Astronomy.hpp" */
	
/** @brief World astronomical detail container
	This class stores astronomical data from the perspective of the World. It generates basic solar system details, and
	will also generate a basic starmap and constellations. This is all mostly for the sake of lore but may also be useful
	for navigation.
	
	For now the class just generates the names of the moons, sun and planets. In future it might be expanded to include a starmap and planet orbits. The starmap could be used for navigation and also could be used for mythology.
	
	Notes: On Earth there are about 5,000 different stars which can be seen from the surface with the naked eye, about
	half of them are obscured by the earth's surface.
	
	Implementation: Basically we will use the normal technique for making a starmap. Each star is given an elevation and
	compass direction from the 0,0 point on earth. When the player wants to access the starmap they view a standard
	starmap with the star positions adjusted for their latitude/longitude. For simplicity the planet has no tilt, so all
	stars rotate about the Y axis.

	The stars are placed randomly and given random magnitudes as a random distribution would be expected.
	
	In future planets will also be added.
	
	The World can have between 0-3 moons, usually 1.
	
	To solve the constellation problem they could be pre-made and placed randomly in the sky. All constellation stars are maximum magnitude, and all normal stars are dimmer.
*/

#include <Game/NameGen/NameGen.hpp>
#include <Container/Vector/Vector.hpp>


class World_Astronomy
{
	private:
	
	NameGen nameGenAstronomy;
	
	public:
	std::string sunName;
	
	Vector <std::string> vMoon;
	Vector <std::string> vPlanet;
	
	World_Astronomy()
	{
		sunName="";
	}
	
	void generateSystem (int nMoon, int nPlanet)
	{
		sunName = nameGenAstronomy.generateName();
		
		vMoon.clear();
		vPlanet.clear();
		
		for (int i=0;i<nMoon;++i)
		{
			vMoon.push(nameGenAstronomy.generateName());
		}
		
		for (int i=0;i<nPlanet;++i)
		{
			vPlanet.push(nameGenAstronomy.generateName());
		}
	}


};

#endif
