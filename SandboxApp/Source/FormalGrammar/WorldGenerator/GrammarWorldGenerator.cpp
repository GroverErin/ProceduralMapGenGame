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
		6: Family Name

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

		x: Weapon
		t: Treasure
	}

	Production Rules =
	{
		S -> 0W

		W(0.6f) -> 1K
		W(0.4f) -> 1K1K

		K(0.3f) -> 2C3T5D4V4V3T4V5D
		K(0.6f) -> 2C3T4V5D5D2C4V3T
		K(0.1f) -> 2C4V5D2C3T

		C(0.6f) -> kqplnm!5D
		C(0.15f) -> kqmmm5D
		C(0.15f) -> kqpl!
		C(0.1f) -> kqln

		T(0.55f) -> b!!!is6H6H6H6H6H6H6H6H
		T(0.45f) -> b!!iss6H6H6H6H6H

		V(0.4f) -> !i6H6H6H
		V(0.6f) -> !6H6H6H6H

		H(0.35f) -> vfc
		H(0.2f) -> vf
		H(0.15f) -> fc
		H(0.3f) -> vfcc

		D(0.5f) -> EEEII
		D(0.2f) -> EEEEEIII
		D(0.3f) -> EI

		E(0.3f) -> uuu
		E(0.25f) -> wwg
		E(0.05f) -> d
		E(0.08f) -> e
		E(0.12f) -> rr
		E(0.2f) -> gg

		I(0.4f) -> x
		I(0.6f) -> t
	}

*/
//--------------------------------------------------------------------------------------------------
//	End Derived Grammar
//--------------------------------------------------------------------------------------------------

#include "GrammarWorldGenerator.h"
#include "FormalGrammar/WeaponGenerator/WeaponGenerator.h"
#include <iostream>

void GrammarWorldGenerator::Initialize()
{
	FormalGrammar::Initialize();
	m_weaponGen.Initialize();

	// Set the rules for this grammar.
	AddRule("S", "0W", 1.0f);

	AddRule("W", "1K", 0.6f);
	AddRule("W", "1K1K", 0.4f);

	AddRule("K", "2C3T5D4V4V3T4V5D", 0.3f);
	AddRule("K", "2C3T4V5D5D2C4V3T", 0.6f);
	AddRule("K", "2C4V5D2C3T", 0.1f);

	AddRule("C", "kqplnm!5D", 0.6f);
	AddRule("C", "kqmmm5D", 0.15f);
	AddRule("C", "kqpl!", 0.15f);
	AddRule("C", "kqln", 0.1f);

	AddRule("T", "b!!!is6H6H6H6H6H6H6H6H", 0.55f);
	AddRule("T", "b!!iss6H6H6H6H6H", 0.45f);

	AddRule("V", "!i6H6H6H", 0.4f);
	AddRule("V", "!6H6H6H6H", 0.6f);

	AddRule("H", "vfc", 0.35f);
	AddRule("H", "vf", 0.2f);
	AddRule("H", "fc", 0.15f);
	AddRule("H", "vfcc", 0.3f);

	AddRule("D", "EEEII", 0.5f);
	AddRule("D", "EEEEEIII", 0.2f);
	AddRule("D", "EI", 0.3f);

	AddRule("E", "uuu", 0.3f);
	AddRule("E", "wwg", 0.25f);
	AddRule("E", "d", 0.05f);
	AddRule("E", "e", 0.08f);
	AddRule("E", "rr", 0.12f);
	AddRule("E", "gg", 0.2f);

	AddRule("I", "x", 0.4f);
	AddRule("I", "t", 0.6f);
}

std::string GrammarWorldGenerator::RunGrammar()
{
	FormalGrammar::RunGrammar();

	std::string world = std::move(BuildWorld());

	system("CLS");
	std::cout << "String: " << GetState() << "\n\n";
	std::cout << world << '\n';
	return GetState();
}

std::string GrammarWorldGenerator::BuildWorld()
{
	std::string world = "";
	int currentIndent = 0;
	std::string& state = GetState();

	for (const char& c : state)
	{
		switch (c)
		{
		case '0':
			world += "World\n";
			break;
		case '1':
			currentIndent = 1;
			world += CreateIndent(currentIndent);
			world += "Kingdom\n";
			break;
		case '2':
			currentIndent = 2;
			world += CreateIndent(currentIndent);
			world += "Castle\n";
			currentIndent += 1;
			break;
		case '3':
			currentIndent = 2;
			world += CreateIndent(currentIndent);
			world += "Town\n";
			currentIndent += 1;
			break;
		case '4':
			currentIndent = 2;
			world += CreateIndent(currentIndent);
			world += "Village\n";
			currentIndent += 1;
			break;
		case '5':
			currentIndent = 2;
			world += CreateIndent(currentIndent);
			world += "Dungeon\n";
			currentIndent += 1;
			break;
		case '6':
			currentIndent = 3;
			world += CreateIndent(currentIndent);
			world += "Family\n";
			currentIndent += 1;
			break;

		case 'k':
			world += CreateIndent(currentIndent);
			world += "King\n";
			break;
		case 'q':
			world += CreateIndent(currentIndent);
			world += "Queen\n";
			break;
		case 'p':
			world += CreateIndent(currentIndent);
			world += "Prince\n";
			break;
		case 'l':
			world += CreateIndent(currentIndent);
			world += "Princess\n";
			break;
		case 'm':
			world += CreateIndent(currentIndent);
			world += "Servant\n";
			break;
		case 'n':
			world += CreateIndent(currentIndent);
			world += "Knight\n";
			break;

		case 'b':
			world += CreateIndent(currentIndent);
			world += "Blacksmith\n";
			break;
		case 'i':
			world += CreateIndent(currentIndent);
			world += "Innkeeper\n";
			break;
		case '!':
			world += CreateIndent(currentIndent);
			world += "Questgiver\n";
			break;
		case 's':
			world += CreateIndent(currentIndent);
			world += "Shopkeeper\n";
			break;

		case 'v':
			world += CreateIndent(currentIndent);
			world += "Male Villager\n";
			break;
		case 'f':
			world += CreateIndent(currentIndent);
			world += "Female Villager\n";
			break;
		case 'c':
			world += CreateIndent(currentIndent);
			world += "Child\n";
			break;

		case 'w':
			world += CreateIndent(currentIndent);
			world += "Wolf\n";
			break;
		case 'g':
			world += CreateIndent(currentIndent);
			world += "Goblin\n";
			break;
		case 'e':
			world += CreateIndent(currentIndent);
			world += "Demon\n";
			break;
		case 'r':
			world += CreateIndent(currentIndent);
			world += "Bandit\n";
			break;
		case 'd':
			world += CreateIndent(currentIndent);
			world += "Dragon\n";
			break;
		case 'u':
			world += CreateIndent(currentIndent);
			world += "Slime\n";
			break;

		case 'x':
			world += CreateIndent(currentIndent);
			world += m_weaponGen.RunGrammar();
			world += "\n";
			break;
		case 't':
			world += CreateIndent(currentIndent);
			world += "Treasure\n";
			break;

		default:
			world += "INVALID";
			break;
		}
	}

	return world;
}

std::string GrammarWorldGenerator::CreateIndent(int numIndents)
{
	std::string indentString = "";
	for (int i = 0; i < numIndents; ++i)
	{
		indentString += "\t";
	}
	return indentString;
}