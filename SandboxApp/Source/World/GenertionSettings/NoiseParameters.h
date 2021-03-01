#pragma once

class NoiseParameters
{
	//----------------------------------------------------------------------------------------------------
	// Parameter Limits and Defaults
	//----------------------------------------------------------------------------------------------------

	static constexpr unsigned int kDefaultOctaves = 1;
	static constexpr unsigned int kMaxOctaves = 6;
	static constexpr float kDefaultPersistance = 0.5f;
	static constexpr unsigned int kDefaultInputRange = 1;
	static constexpr unsigned int kMaxInputRange = 15;

	unsigned int m_numOctaves;
	float m_persistance;
	unsigned int m_inputRange;
	unsigned int m_seed;

public:
	NoiseParameters()
		: m_numOctaves(kDefaultOctaves)
		, m_persistance(kDefaultPersistance)
		, m_inputRange(kDefaultInputRange)
		, m_seed(0)
	{
		//
	}

	void ResetParameters()
	{
		m_numOctaves = kDefaultOctaves;
		m_persistance = kDefaultPersistance;
		m_inputRange = kDefaultInputRange;
	}

	void ResetSeed()
	{
		m_seed = 0;
	}

	unsigned int GetSeed() const { return m_seed; }
	void SetSeed(unsigned int newSeed) { m_seed = newSeed; }

	void SetParameters(unsigned int octaves, unsigned int inputRange, float persistance, unsigned int seed)
	{
		if (octaves > 0 && octaves <= kMaxOctaves)
			m_numOctaves = octaves;

		if (inputRange > 0 && inputRange <= kMaxInputRange)
			m_inputRange = inputRange;

		if (persistance >= 0.0f && persistance <= 1.0f)
			m_persistance = persistance;

		m_seed = seed;
	}

	int GetOctaves() const { return m_numOctaves; }
	void ChangeOctaves(int amount)
	{
		m_numOctaves += amount;
		if (m_numOctaves > kMaxOctaves)
			m_numOctaves = kMaxOctaves;
		else if (m_numOctaves < 1)
			m_numOctaves = 1;
	}

	float GetPersistance() const { return m_persistance; }
	void ChangePersistance(float amount)
	{
		m_persistance += amount;
		if (m_persistance > 1.0f)
			m_persistance = 1.0f;
		else if (m_persistance < 0.0f)
			m_persistance = 0.0f;
	}

	unsigned int GetInputRange() const { return m_inputRange; }
	void ChangeInputRange(int amount)
	{
		m_inputRange += amount;
		if (m_inputRange > kMaxInputRange)
			m_inputRange = kMaxInputRange;
		else if (m_inputRange < 1)
			m_inputRange = 1;
	}
};