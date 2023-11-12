#pragma once
#ifndef WORLDSIM_RESOURCE_CPP
#define WORLDSIM_RESOURCE_CPP

/* WorldSim: Resource
	#include "Resource.cpp"

	Manage resources for a settlement or civ.
	
	Resources are generally managed on a per-settlement basis in order to create logistics, but a civ will seek to
	move resources efficiently between settlements.
	Resources are hardcoded for simplicity.
	
	In some cases I may break the resources down into their own classes to manage quality, however generally this will
	be best handled by crafting skill.
	
*/

class ResourceManager
{
	public:
	
		// RAW
		int nIron;
		int nCopper;
		int nGold;
		int nSilver;
		int nGems;
		int nDiamonds;
		int nAdamantine;
		int nCoal;
		int nStone;
		int nWood;
		int nMagickaCrystal;
		// SMELTED
		int nSteel;
		int nBronze;
		int nAdamantium;
		
		
		
		ResourceManager()
		{
			// RAW
			nIron=0;
			nCopper=0;
			nGold=0;
			nSilver=0;
			nGems=0;
			nDiamonds=0;
			nAdamantine=0;
			nCoal=0;
			nStone=0;
			nWood=0;
			nMagickaCrystal;
			// SMELTED
			nSteel=0;
			nBronze=0;
			nAdamantium=0;
		}
		
		void addIron(int _iron)
		{
			nIron+=_iron;
		}
};


#endif // WORLDSIM_RESOURCE_CPP
