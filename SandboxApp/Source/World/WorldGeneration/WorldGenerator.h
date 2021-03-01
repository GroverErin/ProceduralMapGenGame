#pragma once
#include "World/GenertionSettings/NoiseParameters.h"
#include "World/TileMap/TileMap.h"
#include <Utilities/Random/Noise/PerlinNoise.h>
#include <Utilities/Random/Random.h>

#include <array>
#include <thread>

/// <summary>
/// Generates the terrain/map of the world.
/// Terrain:
///		Terrain generation is just a Perlin Noise height map. There are
///		levels in the terrain in increasing height order: Deep Sea, Sea,
///		Shallow Water, Beach, Ground, Hills, and Mountains.
/// Biomes:
///		Biomes are handled with a combination of moisture and tempurature.
///		The tempurature is a Perlin Noise gradient with a low input range
///		because the "equator" of this world is random and should generate
///		unique "poles".
/// </summary>
class WorldGenerator
{
	static constexpr unsigned int kMaxThreads = 7;
	std::thread* m_pThreadPool = nullptr;

	Exelius::PerlinNoise m_noise;
	Exelius::Random m_rand;

	unsigned int m_mapWidth;
	unsigned int m_mapHeight;
	
public:
	NoiseParameters m_heightParameters;
	NoiseParameters m_moistureParameters;

	WorldGenerator();
	~WorldGenerator();

	WorldGenerator(const WorldGenerator&) = default;
	WorldGenerator(WorldGenerator&&) = default;
	WorldGenerator& operator=(const WorldGenerator&) = default;
	WorldGenerator& operator=(WorldGenerator&&) = default;

	void ResetGenerator();

	/// <summary>
	/// Generate the Terrain, Biomes, and Flora.
	/// </summary>
	void GenerateWorld(TileMap& map);

private:

	void GenerateWorldThread(TileMap& map, size_t startIndex, size_t endIndex);

	float GetHeightNoise(Exelius::Vector2f gridPoint);
	float GetMoistureNoise(Exelius::Vector2f gridPoint);
	float GetTempuratureNormal(Exelius::Vector2f gridPoint);

	//void SaltFloraMap(TileMap& map, size_t startIndex, size_t endIndex);
	void SaltFlora(TileMap& map, Exelius::Vector2f gridPoint);

	void GrowFlora(TileMap& map);

	/// <summary>
	/// Calculates the interpreted value of the height map.
	/// </summary>
	float CalculateHeightValue(float heightNoise) const;

	/// <summary>
	/// Calculates the interpreted value of the tempurature map.
	/// This calculation is dependant on the *value* of the height noise.
	/// The conversion from height to tempurature is taken from this article by Lupe Tanner, PH.D.:
	/// https://www.enotes.com/homework-help/what-relationship-between-altitude-temperature-556362#
	/// </summary>
	float CalculateTempuratureValue(float tempMapVal, float heightValue) const;

	/// <summary>
	/// Calculates the interpreted value of the moisture map.
	/// This calculation is dependant on the *value* of the height noise AND the *value* of the tempurature map.
	/// </summary>
	float CalculateMoistureValue(float moistureNoise, float tempValue, float heightValue) const;

	/// <summary>
	/// Set the color of a map tile color based on the values of the Height,
	/// Moisture, and Tempurature maps at the tiles position in the world.
	/// </summary>
	void CalculateBiome(TileMap& map, Exelius::Vector2f gridPoint, float heightValue, float tempValue, float moistureValue);

	void TryGrowForest(TileMap& map, size_t index);

	void TryGrowRock(TileMap& map, size_t index);

	void TryGrowCliff(TileMap& map, size_t index);
};