#pragma once
#include "World/TileMap/TileMap.h"

#include <Utilities/Random/Random.h>
#include <unordered_map>

class FireGenerator
{
	Exelius::Random m_rand;

	// This map represents all of the fire tiles in the world.
	// They are stored by thier index, and the value they contain
	// is thier lifetime remaining.
	std::unordered_map<size_t, float> m_fireTiles;
	std::vector<size_t> m_newFire;

	float m_currentFireTick;
	size_t m_flamableTileCount;
	size_t m_maxFlamableTiles;

	TileMap* m_pTileMap;

public:
	FireGenerator()
		: m_currentFireTick(0.5f)
		, m_flamableTileCount(0)
		, m_maxFlamableTiles(0)
		, m_pTileMap(nullptr)
	{
		//
	}

	void StartFire(TileMap& map);

	bool PropagateFire(float deltaTime);

	void ResetFireGenerator();

	float GetBurnPercentage()
	{
		return 1.0f - ((float)m_flamableTileCount / (float)m_maxFlamableTiles);
	}

	void ExtinguishTile(size_t tileIndex);

private:
	void IgniteTile(size_t index);
	void InternalExtinguishTile(size_t index);
	void TryIgniteNeighbor(size_t index);
};