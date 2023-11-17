#pragma once
#ifndef WORLDSIM_GOVERNMENT_POSITION_HPP
#define WORLDSIM_GOVERNMENT_POSITION_HPP

/* WorldSim: Government_Position.hpp
	#include "Government_Position.hpp"
*/

class Government;

class Government_Position
{
	public:
		Government* government;
		Character* character;

		Government_Position(Government* _government);
		Government_Position(const Government_Position& other);
		Government_Position& operator=(const Government_Position& other);
		virtual ~Government_Position() = default;

		bool operator==(const Character* otherCharacter) const;
		virtual void govern() = 0;
		virtual void assign(Character* _character);
		bool empty();
};

class Government_Leader : public Government_Position
{
	public:
		Government_Leader(Government* _government);
		virtual void govern() override;
};

class Government_Scribe : public Government_Position
{
	public:
		Government_Scribe(Government* _government);
		virtual void govern() override;
};

class Government_Captain : public Government_Position
{
	public:
		Government_Captain(Government* _government);
		virtual void govern() override;
};

#endif // WORLDSIM_GOVERNMENT_POSITION_HPP
