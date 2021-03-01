#pragma once

namespace Exelius
{
	class Random
	{
		unsigned long long m_seed[2];

	public:
		// [REZ] Why would you not expect Low before High? It seems like it makes more sense to me this way.
		Random(unsigned long long seedLow = 0, unsigned long long seedHigh = 0);

		unsigned long long Rand();

		float FRand();

		//Return a int value from a specified range.
		int IRandomRange(int min, int max);

		//Return a float value from a specified range.
		float FRandomRange(float min, float max);

	};
}