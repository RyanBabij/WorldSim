#pragma once
#ifndef WORLDSIM_GOVERNMENT_HPP
#define WORLDSIM_GOVERNMENT_HPP

/* WorldSim: Government.hpp
	#include "Government.hpp"
*/

#include "Character.hpp"
#include <iostream>

#include "Government_Position.hpp"

class Government
{
	public:
		Settlement* governedSettlement;
		Government_Leader leader;
		Government_Scribe scribe;
		Government_Captain captain;

		Vector<Government_Position> vWorkers;

		Government();
		Government(const Government& other);
		Government& operator=(const Government& other);

		void govern();
		bool needsLeader();
		void setLeader(Character* character);
		void setScribe(Character* character);
		void setCaptain(Character* character);
};

#endif // WORLDSIM_GOVERNMENT_HPP
