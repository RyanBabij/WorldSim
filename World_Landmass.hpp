#pragma once
#ifndef WORLDSIM_WORLD_LANDMASS_HPP
#define WORLDSIM_WORLD_LANDMASS_HPP

/* WorldSim: World_Landmass.hpp
	#include "World_Landmass.hpp"


*/

#include <Container/Table/TableInterface.hpp>

class World_Landmass: public TableInterface
{
	public:
	
	std::string name;
	int size; /* size in tiles */
	
	double averageX, averageY; /* For zooming to landmass. */
	
	World_Landmass()
	{
		name="";
		size=0;
		
		averageX=-1;
		averageY=-1;
	}
	
	virtual ~World_Landmass()
	{
	}
	
	
	std::string getColumn(std::string _column)
	{
		if ( _column=="name" )
		{
			return name;
		}
		else if ( _column=="size" )
		{
			return DataTools::toString(size);
		}

		
		return "?";
	}
	std::string getColumnType(std::string _column)
	{
		if ( _column == "size" || _column == "territory" || _column == "food" )
		{
			return "number";
		}
		return "string";
	}
	
		// Uses a safe rolling average to prevent overflow.
	void getAverageCoordinates(Vector <HasXY*> * vXY)
	{
		int n = 0;
		int n2 = 0;
		averageX = 0;
		averageY = 0;
		for ( int i=0;i<vXY->size();++i)
		{
			double deltaX = (*vXY)(i)->x - averageX;
			averageX += deltaX/++n;
			
			double deltaY = (*vXY)(i)->y - averageY;
			averageY += deltaY/++n2;
		}


	}
};

#endif