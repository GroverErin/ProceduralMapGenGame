#pragma once

#include <Utilities/Random/Random.h>

#include <string>
#include <vector>

struct Rule
{
	Rule(const char* pred, const char* succ, float wght)
		: predecessor(pred)
		, successor(succ)
		, weight(wght)
	{

	}

	const char* predecessor;
	const char* successor;
	float weight;
};

class FormalGrammar
{
public:
	virtual ~FormalGrammar() = default;

	virtual void Initialize();

	virtual std::string RunGrammar();

protected:
	std::string& GetState() { return m_state; }

	void AddRule(const char* predecessor, const char* successor, float weight);

private:

	Rule ChooseRule();
	bool IsRuleValid(Rule& rule);
	bool IsNonTerminating(const char& c);

	Exelius::Random m_rand;

	std::string m_state;
	std::vector<Rule> m_rules;
	std::vector<const char*> m_nonTermSet;
};