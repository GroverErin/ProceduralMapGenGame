#include "WorldGenerator.h"
#include "World/GenertionSettings/GeneratorConfig.h"

WorldGenerator::WorldGenerator()
	: m_mapWidth(0)
	, m_mapHeight(0)
{
	ResetGenerator();

	//Create all of the worker threads that we will need.
	m_pThreadPool = new std::thread[kMaxThreads];
}

WorldGenerator::~WorldGenerator()
{
	delete[] m_pThreadPool;
}

void WorldGenerator::ResetGenerator()
{
	m_heightParameters.SetParameters(kDefaultHeightOctaves, kDefaultHeightInputRange, kDefaultHeightPersistance, (unsigned int)m_rand.Rand());
	m_moistureParameters.SetParameters(kDefaultMoistureOctaves, kDefaultMoistureInputRange, kDefaultMoisturePersistance, (unsigned int)m_rand.Rand());
}

void WorldGenerator::GenerateWorld(TileMap& map)
{
	m_mapWidth = map.GetMapWidth();
	m_mapHeight = map.GetMapHeight();

	size_t threadStride = (m_mapWidth * m_mapHeight) / (kMaxThreads + 1);

	size_t startIndex = 0;
	size_t endIndex = threadStride;

	for (size_t i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i] = std::thread(&WorldGenerator::GenerateWorldThread, this, std::ref(map), startIndex, endIndex);
		startIndex += threadStride;
		endIndex += threadStride;
	}

	GenerateWorldThread(map, startIndex, (size_t)m_mapWidth * (size_t)m_mapHeight);

	//Wait for the threads to complete the read task.
	for (unsigned int i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i].join();
	}

	for (int i = 0; i < kNumCellularAutomataIterations; ++i)
	{
		GrowFlora(map);
	}
}

void WorldGenerator::GenerateWorldThread(TileMap& map, size_t startIndex, size_t endIndex)
{
	for (size_t i = startIndex; i < endIndex; ++i)
	{
		const Exelius::Vector2f gridPoint = map.GetTilePosition(i);

		const float heightNoise = GetHeightNoise(gridPoint);
		const float moistureNoise = GetMoistureNoise(gridPoint);
		const float tempuratureNormal = GetTempuratureNormal(gridPoint);

		const float heightValue = CalculateHeightValue(heightNoise);
		const float tempValue = CalculateTempuratureValue(tempuratureNormal, heightValue);
		const float moistureValue = CalculateMoistureValue(moistureNoise, tempValue, heightValue);

		CalculateBiome(map, gridPoint, heightValue, tempValue, moistureValue);

		SaltFlora(map, gridPoint);
	}
}

float WorldGenerator::GetHeightNoise(Exelius::Vector2f gridPoint)
{
	float heightNoise = m_noise.GetAverageNoise(gridPoint.x, gridPoint.y,
		(float)m_mapWidth / kHeightNoiseDivisor, (float)m_mapHeight / kHeightNoiseDivisor,
		m_heightParameters.GetInputRange(), m_heightParameters.GetOctaves(), m_heightParameters.GetPersistance(), m_heightParameters.GetSeed());

	return (heightNoise * powf(sinf(Exelius::PI * (gridPoint.y / (float)m_mapHeight)) * sinf(Exelius::PI * (gridPoint.x / (float)m_mapWidth)), kHeightNoiseExponent));
}

float WorldGenerator::GetMoistureNoise(Exelius::Vector2f gridPoint)
{
	return m_noise.GetAverageNoise(gridPoint.x, gridPoint.y,
		(float)m_mapWidth, (float)m_mapHeight,
		m_moistureParameters.GetInputRange(), m_moistureParameters.GetOctaves(), m_moistureParameters.GetPersistance(), m_moistureParameters.GetSeed());
}

float WorldGenerator::GetTempuratureNormal(Exelius::Vector2f gridPoint)
{
	return (kDefaultPoleTempuratureNormal + (kDefaultEquatorTempuratureNormal - kDefaultPoleTempuratureNormal)
		* powf(sinf(Exelius::PI * (gridPoint.y / (float)m_mapHeight)), kDefaultTempuratureFalloffExponent));
}

float WorldGenerator::CalculateHeightValue(float heightNoise) const
{
	return kMaxHeight * heightNoise;
}

float WorldGenerator::CalculateTempuratureValue(float tempMapVal, float heightValue) const
{
	// The conversion from height to tempurature is taken from this article by Lupe Tanner, PH.D.:
	// https://www.enotes.com/homework-help/what-relationship-between-altitude-temperature-556362#
	return (tempMapVal * kMaxTemp) + (heightValue / kHeightToTempDivisor) * kHeightToTempRateOfChange;
}

float WorldGenerator::CalculateMoistureValue(float moistureNoise, float tempValue, float heightValue) const
{
	float moistureValue = 0.0f;
	if (heightValue > kReefRange)
	{
		const float lerpWeight = 1.0f - ((tempValue - kOceanRange) / kLandHeightRange);
		const float maxMoistureForHeight = Exelius::Lerp(0.0f, kMaxMoisture, lerpWeight);
		moistureValue = (1.0f - moistureNoise) * maxMoistureForHeight;
	}
	return moistureValue;
}

void WorldGenerator::CalculateBiome(TileMap& map, Exelius::Vector2f gridPoint, float heightValue, float tempValue, float moistureValue)
{
	if (heightValue <= kOceanRange)
	{
		map.SetTileColor(gridPoint, kOcean);
		return;
	}

	if (heightValue <= kReefRange)
	{
		map.SetTileColor(gridPoint, kReef);
		return;
	}

	if (heightValue >= kMountainRange)
	{
		map.SetTileColor(gridPoint, kRock);
		return;
	}

	//The tempurature range is under -5 C
	if (tempValue <= kTundraRange)
	{
		map.SetTileColor(gridPoint, kGlacier);
		return;
	}

	//The tempurature range is under 0 C
	else if (tempValue <= kTaigaRange)
	{
		if (moistureValue <= kSnowMoisture)
			map.SetTileColor(gridPoint, kSnow);
		else
			map.SetTileColor(gridPoint, kGrassland);
		return;
	}

	//The tempurature range is under 18 C
	else if (tempValue <= kTemperateRange)
	{
		if (moistureValue <= kSavannaMoisture)
			map.SetTileColor(gridPoint, kSavanna);
		else if (moistureValue <= kGrasslandMoisture)
			map.SetTileColor(gridPoint, kGrassland);
		else
			map.SetTileColor(gridPoint, kSwamp);
		return;
	}

	//The tempurature range is under 30 C
	else if (tempValue <= kTropicalRange)
	{
		if (moistureValue <= kDuneMoisture)
			map.SetTileColor(gridPoint, kDunes);
		else
			map.SetTileColor(gridPoint, kDesert);
		return;
	}

	map.SetTileColor(gridPoint, kError);
}

void WorldGenerator::SaltFlora(TileMap& map, Exelius::Vector2f gridPoint)
{
	const float chance = m_rand.FRandomRange(0.0f, 1.0f);

	if (map.GetTileColor(gridPoint) == kGrassland.GetHex())
	{
		if (chance <= kSaltGrassToRockChance)
			map.SetTileColor(gridPoint, kRock);
		else if (chance <= kSaltGrassToTreeChance)
			map.SetTileColor(gridPoint, kForest);
	}

	else if (map.GetTileColor(gridPoint) == kSavanna.GetHex())
	{
		if (chance <= kSaltSavannaToRockChance)
			map.SetTileColor(gridPoint, kRock);
		else if (chance <= kSaltSavannaToCliffChance)
			map.SetTileColor(gridPoint, kCliff);
	}

	else if (map.GetTileColor(gridPoint) == kSnow.GetHex())
	{
		if (chance <= kSaltSnowtoRockChance)
			map.SetTileColor(gridPoint, kRock);
		else if (chance <= kSaltSnowtoTreeChance)
			map.SetTileColor(gridPoint, kCliff);
	}

	else if (map.GetTileColor(gridPoint) == kDesert.GetHex())
	{
		if (chance <= kSaltDesertToRockChance)
			map.SetTileColor(gridPoint, kRock);
	}

	else if (map.GetTileColor(gridPoint) == kGlacier.GetHex())
	{
		if (chance <= kSaltGlacierToRockChance)
			map.SetTileColor(gridPoint, kRock);
	}

	else if (map.GetTileColor(gridPoint) == kSwamp.GetHex())
	{
		if (chance <= kSaltSwamptoRockChance)
			map.SetTileColor(gridPoint, kRock);
	}
}

void WorldGenerator::GrowFlora(TileMap& map)
{
	for (size_t i = 0; i < map.GetTiles().size(); ++i)
	{
		if (map.GetTileColor(i) == kOcean.GetHex())
			continue;

		if (map.GetTileColor(i) == kForest.GetHex())
		{
			TryGrowForest(map, i);
		}
		else if (map.GetTileColor(i) == kRock.GetHex())
		{
			TryGrowRock(map, i);
		}
		else if (map.GetTileColor(i) == kCliff.GetHex())
		{
			TryGrowCliff(map, i);
		}
	}
}

void WorldGenerator::TryGrowForest(TileMap& map, size_t index)
{
	// This tile is a tree
	const auto& neighbors = map.GetTileNeighbors(index);

	for (auto tile : neighbors)
	{
		const float chance = m_rand.FRandomRange(0.0f, 1.0f);

		if (chance > kForestGrowthChance)
			continue;
		if (map.GetTileColor(tile) == kOcean.GetHex())
			continue;

		// This tile is a grass tile and it is going to become a tree
		if (chance <= kForestGrowthChance && map.GetTileColor(tile) == kGrassland.GetHex())
		{
			map.SetTileColor(tile, kForest);
		}
		else if (chance <= kForestGrowthChance && map.GetTileColor(tile) == kSnow.GetHex())
		{
			map.SetTileColor(tile, kForest);
		}
	}
}

void WorldGenerator::TryGrowRock(TileMap& map, size_t index)
{
	const auto& neighbors = map.GetTileNeighbors(index);
	const float chance = m_rand.FRandomRange(0.0f, 1.0f);
	if (chance > kRockGrowthChance)
		return;

	for (auto tile : neighbors)
	{
		if (map.GetTileColor(tile) == kOcean.GetHex())
			continue;

		if (map.GetTileColor(tile) == kGrassland.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
		else if (map.GetTileColor(tile) == kDesert.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
		else if (map.GetTileColor(tile) == kGlacier.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
		else if (map.GetTileColor(tile) == kSavanna.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
		else if (map.GetTileColor(tile) == kSwamp.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
		else if (map.GetTileColor(tile) == kSnow.GetHex())
		{
			map.SetTileColor(tile, kRock);
		}
	}
}

void WorldGenerator::TryGrowCliff(TileMap& map, size_t index)
{
	const auto& neighbors = map.GetTileNeighbors(index);
	const float chance = m_rand.FRandomRange(0.0f, 1.0f);

	if (chance > kCliffGrowthChance)
		return;

	for (auto tile : neighbors)
	{
		if (map.GetTileColor(tile) == kSavanna.GetHex())
		{
			map.SetTileColor(tile, kCliff);
		}
	}
}