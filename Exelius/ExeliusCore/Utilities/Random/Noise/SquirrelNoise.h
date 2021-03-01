#pragma once

namespace Exelius
{
	/// <summary>
	/// This algorithm (called SquirrelNoise3) was originally written by Squirrel Eiserloh and was presented in 
	/// his 2017 GDC talk titled Math for Game Programmers: Noise-Based RNG, which can be found here:
	/// https://www.gdcvault.com/play/1024365/Math-for-Game-Programmers-Noise
	/// 
	/// The use of the static and noexcept were borrowed from an explanation given by David "Rez" Graham.
	/// 
	/// Note: All hard-coded numbers are taken from
	/// Squirrels GDC talk or from Rez.
	/// </summary>
	class SquirrelNoise
	{
		unsigned int m_seed;

		static constexpr unsigned int kBitNoise = 0x68e31da4;
		static constexpr unsigned int kBitNoise1 = 0xb5297a4d;
		static constexpr unsigned int kBitNoise2 = 0x1b56c4e9;

		static constexpr unsigned int kPrime = 198491317;
		static constexpr unsigned int kPrime1 = 6543989;

	public:
		/// <summary>
		/// Creates a SquirrelNoise generator.
		/// </summary>
		/// <param name="seed">(unsigned long long) The seed used by the generator. Default is 0.</param>
		constexpr SquirrelNoise(unsigned int seed = 0) noexcept
			: m_seed(seed)
		{
			//
		}

		//----------------------------------------------------------------------------------------------------
		// 1 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 1 Dimensional Noise
		static constexpr unsigned int Get1DNoise(int x, unsigned int seedOverride) noexcept
		{
			unsigned int mangledBits = static_cast<unsigned int>(x);
			mangledBits *= kBitNoise;
			mangledBits += seedOverride;
			mangledBits ^= (mangledBits >> 8);
			mangledBits *= kBitNoise1;
			mangledBits ^= (mangledBits << 8);
			mangledBits *= kBitNoise2;
			mangledBits ^= (mangledBits >> 8);

			return mangledBits;
		}

		constexpr unsigned int Get1DNoise(int x) const noexcept
		{
			return Get1DNoise(x, m_seed);
		}

		static constexpr float GetUniform1DNoise(int x, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get1DNoise(x, seedOverride));
			return noise / static_cast<float>(0xffffffff);
		}

		constexpr float GetUniform1DNoise(int x) const noexcept
		{
			return GetUniform1DNoise(x, m_seed);
		}

		static constexpr float GetSignedUniform1DNoise(int x, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get1DNoise(x, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		constexpr float GetSignedUniform1DNoise(int x) const noexcept
		{
			return GetSignedUniform1DNoise(x, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// 2 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 2 Dimensional Noise
		static constexpr unsigned int Get2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			return Get1DNoise(x + (kPrime * y), seedOverride);
		}

		constexpr unsigned int Get2DNoise(int x, int y) const noexcept
		{
			return Get2DNoise(x, y, m_seed);
		}

		static constexpr float GetUniform2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get2DNoise(x, y, seedOverride));
			return noise / static_cast<float>(0xffffffff);
		}

		constexpr float GetUniform2DNoise(int x, int y) const noexcept
		{
			return GetUniform2DNoise(x, y, m_seed);
		}

		static constexpr float GetSignedUniform2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get2DNoise(x, y, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		constexpr float GetSignedUniform2DNoise(int x, int y) const noexcept
		{
			return GetSignedUniform2DNoise(x, y, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// 3 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 3 Dimensional Noise
		static constexpr unsigned int Get3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			return Get1DNoise(x + (kPrime * y) + (kPrime1 * z), seedOverride);
		}

		constexpr unsigned int Get3DNoise(int x, int y, int z) const noexcept
		{
			return Get3DNoise(x, y, z, m_seed);
		}

		static constexpr float GetUniform3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get3DNoise(x, y, z, seedOverride));
			return 2.f * (noise / static_cast<float>(0xffffffff)) - 1.0f;
		}

		constexpr float GetUniform3DNoise(int x, int y, int z) const noexcept
		{
			return GetUniform3DNoise(x, y, z, m_seed);
		}

		static constexpr float GetSignedUniform3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get3DNoise(x, y, z, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		constexpr float GetSignedUniform3DNoise(int x, int y, int z) const noexcept
		{
			return GetSignedUniform3DNoise(x, y, z, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// Accessors
		//----------------------------------------------------------------------------------------------------

		unsigned int GetSeed() { return m_seed; }
		void SetSeed(unsigned int seed) { m_seed = seed; }
	};
}