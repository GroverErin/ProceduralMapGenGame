#include "FormalGrammar.h"

#include <time.h>

void FormalGrammar::Initialize()
{
	m_rules.clear();
};

std::string FormalGrammar::RunGrammar()
{
	m_state = "S";
	bool hasNonTerminatingSymbols = true;

	while (hasNonTerminatingSymbols)
	{
		hasNonTerminatingSymbols = false;
		std::string newState = "";

		// Choose random rule from rules using a weighted random.
		Rule rule = ChooseRule();

		std::vector<size_t> symbolIndexesToReplace;

		for (size_t i = 0; i != m_state.size(); ++i)
		{
			if (m_state.at(i) == *rule.predecessor)
				symbolIndexesToReplace.emplace_back(i);
		}

		// Run the rule on the first valid symbol.
		// O(n)
		size_t lastIndex = 0;
		for (size_t index : symbolIndexesToReplace)
		{
			if (lastIndex != index)
				newState += m_state.substr(lastIndex, index);
			newState += rule.successor;
			lastIndex = index + 1;

			// Had to add a break here because the indexing would break with multiple similar symbols next to each other,
			// and all the similar symbols become the same. Would be better to have a different RunGrammar version.
			// This does make this for loop useless currently.
			//break;
		}

		newState += m_state.substr(lastIndex, m_state.size());

		// Check if we are done.
		// O(n)
		for (const char& c : newState)
		{
			if (IsNonTerminating(c))
			{
				hasNonTerminatingSymbols = true;
				break;
			}
		}

		m_state = std::move(newState);
	}

	return m_state;
}

void FormalGrammar::AddRule(const char* predecessor, const char* successor, float weight)
{
	//If the predecessor has not been added to the list of non-terminating characters, then add it.
	auto it = std::find(m_nonTermSet.begin(), m_nonTermSet.end(), predecessor);
	if (it == m_nonTermSet.end())
		m_nonTermSet.emplace_back(predecessor);

	m_rules.emplace_back(predecessor, successor, weight);
}

Rule FormalGrammar::ChooseRule()
{
	std::vector<Rule> validRules;

	for (auto rule : m_rules)
	{
		if (IsRuleValid(rule))
			validRules.emplace_back(rule);
	}

	float totalWeight = 0.0f;
	for (auto rule : validRules)
	{
		totalWeight += rule.weight;
	}

	float choice = totalWeight * m_rand.FRandomRange(0.0f, 1.0f);
	for (auto rule : validRules)
	{
		choice -= rule.weight;
		if (choice <= 0)
			return rule;
	}

	return validRules.front();
}

bool FormalGrammar::IsRuleValid(Rule& rule)
{
	for (auto c : m_state)
	{
		// If this rule has an association with any part of the current state, then it is valid.
		// We can bail because we don't need to check if it is valid anymore.
		if (*rule.predecessor == c)
		{
			return true;
		}
	}

	return false;
}

bool FormalGrammar::IsNonTerminating(const char& c)
{
	for (auto nonTermChar : m_nonTermSet)
	{
		if (c == *nonTermChar)
			return true;
	}

	return false;
}