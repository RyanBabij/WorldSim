#pragma once
#ifndef WORLDSIM_HAS_MONEY_CPP
#define WORLDSIM_HAS_MONEY_CPP

/* WorldSim: HasMoney
	#include "HasMoney.cpp"

	An entity (character, government, stockpile, etc) which has a money balance.
	
*/


class HasMoney
{
	private:
		int nMoney;
	public:
	
		HasMoney()
		{
			nMoney=0;
		}
		
		void addMoney(int _money)
		{
			nMoney+=_money;
		}
		void giveMoney(int _money)
		{ addMoney(_money); }
		
		int getMoney()
		{
			return nMoney;
		}
		
		bool takeMoney(int _amount)
		{
			if ( _amount <= nMoney )
			{
				nMoney-=_amount;
				return true;
			}
			return false;
		}
		
		int takeMoneyUpTo(int _amount)
		{
			if ( _amount <= nMoney )
			{
				nMoney-=_amount;
				return _amount;
			}
			int amountCanTake = nMoney;
			nMoney=0;
			return amountCanTake;
		}
		
};

#endif // WORLDSIM_HAS_MONEY_CPP
