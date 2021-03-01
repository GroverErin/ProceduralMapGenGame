#pragma once
#include "Utilities/Math/Math.h"
#include "Utilities/Random/Noise/SquirrelNoise.h"

namespace Exelius
{
	/// <summary>
	/// This algorithm (called PerlinNoise) was originally written by Ken Perlin and was used for Disney's
	/// 1982 film "Tron", developed at Mathamatical Applications Group, Inc (MAGI), which can be found here:
	/// https://en.wikipedia.org/wiki/Perlin_noise
	/// </summary>
	class PerlinNoise
	{
		static constexpr unsigned int kPrime = 198491317;
		unsigned int m_seed;

	public:

		constexpr PerlinNoise(unsigned int seed = kPrime) noexcept
			: m_seed(seed)
		{
			//
		}

		//----------------------------------------------------------------------------------------------------
		// Noise Functions
		//----------------------------------------------------------------------------------------------------

		static constexpr float GetNoise(float x, float y, unsigned int seedOverride) noexcept
		{
			//Determine the grid cell coordinates.
			int xFloor = (int)x;
			int xCeiling = xFloor + 1;
			int yFloor = (int)y;
			int yCeiling = yFloor + 1;

			// Caluculate the delta position (i.e. how far away from the top left grid point is from our point) 
			// and smooth the results.  These will be used as lerp weights.
			float smoothWeightX = SmootherStep(x - (float)xFloor);
			float smoothWeightY = SmootherStep(y - (float)yFloor);

			// Interpolate the noise, using the smoothed X value as a weight.
			float topLeftNoise = DotGridGradient(xFloor, yFloor, x, y, seedOverride);
			float topRightNoise = DotGridGradient(xCeiling, yFloor, x, y, seedOverride);
			float resultX = Lerp(topLeftNoise, topRightNoise, smoothWeightX);

			float bottomLeftNoise = DotGridGradient(xFloor, yCeiling, x, y, seedOverride);
			float bottomRightNoise = DotGridGradient(xCeiling, yCeiling, x, y, seedOverride);
			float resultY = Lerp(bottomLeftNoise, bottomRightNoise, smoothWeightX);

			// Finally, interpolate between the resulting X and Y values using the smoothed Y value as a 
			// weight.  This is our final noise value.
			float finalNoise = Lerp(resultX, resultY, smoothWeightY);
			return finalNoise;
		}

		constexpr float GetNoise(float x, float y) const noexcept
		{
			return GetNoise(x, y, m_seed);
		}

		static constexpr float GetNoise(float x, float y, float maxX, float maxY, unsigned int noiseInputRange, unsigned int seedOverride) noexcept
		{
			float noiseGridX = (x / maxX) * static_cast<float>(noiseInputRange);
			float noiseGridY = (y / maxY) * static_cast<float>(noiseInputRange);
			return GetNoise(noiseGridX, noiseGridY, seedOverride);
		}

		constexpr float GetNoise(float x, float y, float maxX, float maxY, unsigned int noiseInputRange) const noexcept
		{
			return GetNoise(x, y, maxX, maxY, noiseInputRange, m_seed);
		}

		static constexpr float GetNormalizedNoise(float x, float y, unsigned int seedOverride) noexcept
		{
			return Normalize(GetNoise(x, y, seedOverride), -0.707f, 0.707f);
		}

		constexpr float GetNormalizedNoise(float x, float y) const noexcept
		{
			return GetNormalizedNoise(x, y, m_seed);
		}

		static constexpr float GetAverageNoise(float x, float y, float maxX, float maxY, unsigned int noiseinputRange, unsigned int numOctaves, float persistance, unsigned int seedOverride) noexcept
		{
			constexpr unsigned int kSeedMultiplier = 7322071;
			if (numOctaves <= 0)
				return 0.0f;

			float noise = 0.0f;
			float currentAmplitude = 1.0f;
			float totalAmplitude = 0.0f;
			for (unsigned int i = 0; i < numOctaves; ++i)
			{
				totalAmplitude += currentAmplitude;

				seedOverride = seedOverride + (i * kSeedMultiplier);
				float localNoise = GetNoise(x, y, maxX, maxY, noiseinputRange, seedOverride);
				noise += localNoise * currentAmplitude;

				currentAmplitude *= persistance;
				noiseinputRange *= 2;
			}

			noise /= totalAmplitude;

			noise = Normalize(noise, -0.707f, 0.707f);

			// This may not be necessary.
			noise = SmootherStep(noise);
			return noise;
		}

		constexpr float GetAverageNoise(float x, float y, float maxX, float maxY, unsigned int noiseinputRange, unsigned int numOctaves, float persistance) const noexcept
		{
			return GetAverageNoise(x, y, maxX, maxY, noiseinputRange, numOctaves, persistance, m_seed);
		}

		//----------------------------------------------------------------------------------------------------
		// Accessors
		//----------------------------------------------------------------------------------------------------

		unsigned int GetSeed() { return m_seed; }
		void SetSeed(unsigned int seed) { m_seed = seed; }

	private:

		static constexpr float DotGridGradient(int cellX, int cellY, float gridX, float gridY, unsigned int seed) noexcept
		{
			// Compute the distance vector.
			float distanceX = gridX - (float)cellX;
			float distanceY = gridY - (float)cellY;

			// Calculate a random unit vector for the x and y coordinates.
			// Dylan pointed out that I need to get a uniform (equally likely) value from these
			// , which fixed a bug where I was getting no "peak" values in my noise.
			float unitX = SquirrelNoise::GetUniform3DNoise(cellY, cellX, 0, seed);
			float unitY = SquirrelNoise::GetUniform3DNoise(cellY, cellX, 100, seed);

			// Dot product between the two vectors.
			return (distanceX * unitX + distanceY * unitY);
		}
	};
}