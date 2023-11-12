#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_HPP
#define GUILD_SETTLEMENT_DWARVEN_HPP

#include "Settlement.hpp"

class Settlement_Dwarven: public Settlement
{
	private:


	public:
		Settlement_Dwarven();

		/* SIMULATE X TURNS OF THE SETTLEMENT. */
		void incrementTicks ( int /* nTicks */ );

		// WorldObjectGlobal virtual.
		Texture* currentTexture();
};

#endif
