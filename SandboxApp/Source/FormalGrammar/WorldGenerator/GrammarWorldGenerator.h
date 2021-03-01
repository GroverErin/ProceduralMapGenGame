//--------------------------------------------------------------------------------------------------
// Derived Grammar
//--------------------------------------------------------------------------------------------------
/*
	NonTerminating =
	{
		S: Start
		W: World
		K: Kingdom
		C: Castle
		T: Town
		V: Village
		H: Hovel
		D: Dungeon
		E: Enemy
		I: Item
	}

	//These terminating conditions could be additionally linked to a random name generator...
	Terminating =
	{
		0: World Name
		1: Kingdom Name
		2: Castle Name
		3: Town Name
		4: Village Name
		5: Dungeon Name

		k: King
		q: Queen
		p: Prince
		l: Princess
		m: Servant
		n: Knight

		b: Blacksmith
		i: Innkeeper
		!: Questgiver
		s: Shopkeeper

		v: Male Villager
		f: Female Villager
		c: Child

		w: Wolf
		g: Goblin
		e: Demon
		r: Bandit
		d: Dragon
		u: Slime

		x: Weapon (Also runs Weapon Generation as part of World Generation!)
		t: Treasure
	}

	Production Rules =
	{
		S() -> 0W

		W() -> 1K
		W() -> 1K1K

		K() -> 2C3T
		K() -> 2C3T5D
		K() -> 2C3T4V
		K() -> 2C3T4V5D
		K() -> 2C4V
		K() -> 2C4V5D
		K() -> 4V
		K() -> 4V5D
		K() -> 3T
		K() -> 3T5D
		K() -> 3T4V
		K() -> 3T4V5D

		C() -> kqplnm!D
		C() -> kqm5D
		C() -> kqpl!
		C() -> kqln

		T() -> b!!!isHHHHHHHH
		T() -> b!!issHHHHH

		V() -> !iHHH
		V() -> !HHHH

		H() -> vfc
		H() -> vf
		H() -> fc
		H() -> vfcc

		D() -> EEEII
		D() -> EEEEEIII
		D() -> EI

		E() -> uuu
		E() -> wwg
		E() -> d
		E() -> e
		E() -> rr
		E() -> gg

		I() -> x
		I() -> t
	}

*/
//--------------------------------------------------------------------------------------------------
//	End Derived Grammar
//--------------------------------------------------------------------------------------------------

#pragma once
#include "FormalGrammar/FormalGrammar.h"
#include "FormalGrammar/WeaponGenerator/WeaponGenerator.h"

class GrammarWorldGenerator final
	: public FormalGrammar
{
public:
	virtual void Initialize() final override;

	virtual std::string RunGrammar() final override;

private:
	std::string BuildWorld();

	std::string CreateIndent(int numIndents);

	WeaponGenerator m_weaponGen;
};