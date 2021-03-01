//--------------------------------------------------------------------------------------------------
// Derived Grammar
//--------------------------------------------------------------------------------------------------
/*
	NonTerminating =
	{
		S: Start
		W: Weapon
		E: Element
		+: Enchantment
		-: Curse
	}

	Terminating =
	{
		s: Sword
		a: Axe
		b: Bow
		m: Mace
		d: Dagger

		f: Fire
		w: Water
		e: Earth
		v: air

		h: Sharpness
		i: Mighty
		y: Slaying
		o: Protection
		n: Poison

		1: -3
		2: -2
		3: -1
		4:	1
		5:	2
		6:	3
	}

	Production Rules =
	{
		S(1.0f) -> W

		W(0.50f) -> T
		W(0.30f) -> T+
		W(0.25f) -> T-
		W(0.25f) -> Th
		W(0.15f) -> Th+
		W(0.20f) -> TE
		W(0.15f) -> TE+
		W(0.10f) -> ThE
		W(0.10f) -> ThE+

		T(0.2f) -> s
		T(0.2f) -> a
		T(0.2f) -> b
		T(0.2f) -> m
		T(0.2f) -> d

		E(0.25f) -> f
		E(0.25f) -> w
		E(0.25f) -> e
		E(0.25f) -> v

		-(0.1f) -> 1
		-(0.2f) -> 2
		-(0.7f) -> 3

		+(0.7f) -> 4
		+(0.2f) -> 5
		+(0.1f) -> 6
	}
*/
//--------------------------------------------------------------------------------------------------
//	End Derived Grammar
//--------------------------------------------------------------------------------------------------

#pragma once
#include "FormalGrammar/FormalGrammar.h"

class WeaponGenerator final
	: public FormalGrammar
{
public:
	virtual void Initialize() final override;

	virtual std::string RunGrammar() final override;

private:
	std::string BuildWeapon();
};