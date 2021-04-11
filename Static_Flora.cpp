#pragma once
#ifndef WORLDSIM_STATIC_FLORA_CPP
#define WORLDSIM_STATIC_FLORA_CPP

/* WorldSim: Static_Flora
   #include "Static_Flora.cpp"

	Implementation of Static_Flora.hpp
	
*/

#include "Static_Flora.hpp"

Flora::Flora(const std::string _name /* = "Flora" */, const unsigned short int _spawnWeight /* =1 */)
{
	biome = 0;
	name=_name;
	spawnWeight=_spawnWeight;
	ingredient = ingredientGenerator.generateIngredient();
}
void Flora::increment(unsigned short int nDays)
{
	// for (int i=0;i<nDays;++i)
	// {
		// const unsigned short int newEasy = easyFood+easyFoodIncrement;
		// if ( newEasy > 255 ) { easyFood = 255; }
		// else { easyFood = newEasy; }
		// if ( easyFood > maxEasyFood ) { easyFood = maxEasyFood; }

		// const unsigned short int newMedium = mediumFood+mediumFoodIncrement;
		// if ( newMedium > 255 ) { mediumFood = 255; }
		// else { mediumFood = newMedium; }
		// if ( mediumFood > maxMediumFood ) { mediumFood = maxMediumFood; }
		
		// const unsigned short int newHard = hardFood+hardFoodIncrement;
		// if ( newHard > 255 ) { hardFood = 255; }
		// else { hardFood = newHard; }
		// if ( hardFood > maxHardFood ) { hardFood = maxHardFood; }
	// }
}
void Flora::setFoodValues(unsigned char _maxEasy, unsigned char _maxMedium, unsigned char _maxHard)
{
	easyFood = _maxEasy;
	mediumFood = _maxMedium;
	hardFood = _maxHard;
}
void Flora::allowBiome(unsigned char biomeType)
{
	//vAllowedBiomeTypes.push(biomeType);
}
Texture* Flora::currentTexture()
{
	return &TEX_FLORA_PLANT;
}

// TableInterface
std::string Flora::getColumn(std::string _column)
{
	if ( _column=="name" )
	{
		return name;
	}
	// else if ( _column=="size" )
	// {
		// return DataTools::toString(size);
	// }
	// else if ( _column=="type" )
	// {
		// return WorldGenerator2::biomeName[type];
	// }
	return "?";
}
// TableInterface
std::string Flora::getColumnType(std::string _column)
{
	// if ( _column == "size" )
	// {
		// return "number";
	// }
	return "string";
}

#endif
