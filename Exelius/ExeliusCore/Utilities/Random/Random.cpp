#include "Random.h"

#include <stdint.h>
#include <limits>
#include <ctime>

namespace Exelius
{
	Random::Random(unsigned long long seedLow, unsigned long long seedHigh)
		: m_seed{seedLow, seedHigh}
	{
		if (seedLow == 0 && seedHigh == 0)
		{
			m_seed[0] = time(nullptr);
			m_seed[1] = time(nullptr);
		}
	}

	unsigned long long Random::Rand()
	{
		unsigned long long x = m_seed[0];
		const unsigned long long y = m_seed[1];
		m_seed[0] = y;
		x ^= x << 23;
		m_seed[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
		return m_seed[1] + y;
	}

	float Random::FRand()
	{
		float randF = static_cast<float>(Rand()) / static_cast<float>(std::numeric_limits<unsigned long long>::max());
		return randF;
	}

	int Random::IRandomRange(int min, int max)
	{
		unsigned long long mod = (((unsigned long long)max - (unsigned long long)min) + 1);
		return min + (int)(Rand() % mod);
	}

	float Random::FRandomRange(float min, float max)
	{
		float rand = FRand();
		float difference = max - min;
		return min + (rand * difference);
	}
}