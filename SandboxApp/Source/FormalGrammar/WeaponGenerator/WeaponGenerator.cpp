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

#include "WeaponGenerator.h"
#include <iostream>

void WeaponGenerator::Initialize()
{
	FormalGrammar::Initialize();

	// Set the rules for this grammar.
	AddRule("S", "W", 1.0f);

	//------------------------------
	AddRule("W", "T",		0.5f);
	AddRule("W", "T+",		0.3f);
	AddRule("W", "T-",		0.25f);
	AddRule("W", "Th",		0.25f);
	AddRule("W", "Th+",		0.15f);
	AddRule("W", "TE",		0.2f);
	AddRule("W", "TE+",		0.15f);
	AddRule("W", "ThE",		0.1f);
	AddRule("W", "ThE+",	0.1f);
	//------------------------------

	AddRule("T", "s", 0.2f);
	AddRule("T", "a", 0.2f);
	AddRule("T", "b", 0.2f);
	AddRule("T", "m", 0.2f);
	AddRule("T", "d", 0.2f);

	AddRule("E", "f", 0.25f);
	AddRule("E", "w", 0.25f);
	AddRule("E", "e", 0.25f);
	AddRule("E", "v", 0.25f);

	AddRule("-", "1", 0.1f);
	AddRule("-", "2", 0.2f);
	AddRule("-", "3", 0.7f);

	AddRule("+", "4", 0.7f);
	AddRule("+", "5", 0.2f);
	AddRule("+", "6", 0.1f);
}

std::string WeaponGenerator::RunGrammar()
{
	FormalGrammar::RunGrammar();
	std::cout << GetState() << '\n';

	std::string weapon = std::move(BuildWeapon());
	std::cout << weapon << '\n';
	return weapon;
}

std::string WeaponGenerator::BuildWeapon()
{
	std::string weapon = "";
	std::string& state = GetState();
	for (const char& c : state)
	{
		switch (c)
		{
		case 's':
			weapon += "Sword";
			break;
		case 'a':
			weapon += "Axe";
			break;
		case 'b':
			weapon += "Bow";
			break;
		case 'm':
			weapon += "Mace";
			break;
		case 'd':
			weapon += "Dagger";
			break;

		case 'f':
			weapon = "Flaming " + weapon;
			break;
		case 'w':
			weapon = "Frozen " + weapon;
			break;
		case 'e':
			weapon = "Earthly " + weapon;
			break;
		case 'v':
			weapon = "Turbulent " + weapon;
			break;

		case 'h':
			weapon += " of Sharpness";
			break;
		case 'i':
			weapon = "Mighty " + weapon;
			break;
		case 'y':
			weapon += " of Slaying";
			break;
		case 'o':
			weapon += " of Protection";
			break;
		case 'n':
			weapon = "Poisoned " + weapon;
			break;

		case '1':
			weapon += " -3";
			break;
		case '2':
			weapon += " -2";
			break;
		case '3':
			weapon += " -1";
			break;

		case '4':
			weapon += " +1";
			break;
		case '5':
			weapon += " +2";
			break;
		case '6':
			weapon += " +3";
			break;

		default:
			weapon += "INVALID";
			break;
		}
	}

	return weapon;
}