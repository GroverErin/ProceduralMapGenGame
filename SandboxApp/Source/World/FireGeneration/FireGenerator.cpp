#include "FireGenerator.h"
#include "World/GenertionSettings/GeneratorConfig.h"

void FireGenerator::StartFire(TileMap& map)
{
	m_pTileMap = &map;
	for (size_t i = 0; i < m_pTileMap->GetTiles().size(); ++i)
	{
		const float chance = m_rand.FRandomRange(0.0f, 1.0f);
		auto tile = map.GetTileColor(i);

		if (tile == kGrassland.GetHex()
			|| tile == kForest.GetHex()
			|| tile == kSavanna.GetHex()
			|| tile == kSwamp.GetHex())
		{
			++m_flamableTileCount;
		}
		else
		{
			continue;
		}

		if (chance <= kGrasslandCombustionChance)
		{
			IgniteTile(i);
		}
		else if (chance <= kForestCombustionChance)
		{
			IgniteTile(i);
		}
		else if (chance <= kSavannaCombustionChance)
		{
			IgniteTile(i);
		}
		else if (chance <= kSwampCombustionChance)
		{
			IgniteTile(i);
		}
	}

	m_maxFlamableTiles = m_flamableTileCount;
}

bool FireGenerator::PropagateFire(float deltaTime)
{
	m_currentFireTick -= deltaTime;
	if (m_currentFireTick > 0.0f)
		return false;


	if (m_fireTiles.empty() && m_newFire.empty())
		return true;

	for (auto tile : m_newFire)
	{
		m_fireTiles.emplace(tile, kFireLifetime);
	}

	m_newFire.clear();

	for (auto it = m_fireTiles.begin(); it != m_fireTiles.end();)
	{
		it->second -= deltaTime;

		if (it->second <= 0.0f)
		{
			InternalExtinguishTile(it->first);
			it = m_fireTiles.erase(it);
		}
		else
		{
			TryIgniteNeighbor(it->first);
			++it;
		}
	}

	m_currentFireTick = kFireTickTime;
	return false;
}

void FireGenerator::ResetFireGenerator()
{
	m_flamableTileCount = 0;
	m_maxFlamableTiles = 0;
	m_currentFireTick = 0.5f;

	m_newFire.clear();
	m_fireTiles.clear();

	m_pTileMap = nullptr;
}

void FireGenerator::ExtinguishTile(size_t tileIndex)
{
	InternalExtinguishTile(tileIndex);
	m_fireTiles.erase(tileIndex);

	auto it = std::find(m_newFire.begin(), m_newFire.end(), tileIndex);

	if (it != m_newFire.end())
		m_newFire.erase(it);
}

void FireGenerator::IgniteTile(size_t index)
{
	m_pTileMap->SetTileColor(index, Exelius::Colors::Flame);
	m_newFire.emplace_back(index);
	--m_flamableTileCount;
}

void FireGenerator::InternalExtinguishTile(size_t index)
{
	m_pTileMap->SetTileColor(index, kBlackScorch);
}

void FireGenerator::TryIgniteNeighbor(size_t index)
{
	const auto& neighbors = m_pTileMap->GetTileNeighbors(index);

	for (auto tile : neighbors)
	{
		const float chance = m_rand.FRandomRange(0.0f, 1.0f);
		auto tileColor = m_pTileMap->GetTileColor(tile);

		if (tileColor == kOcean.GetHex())
			continue;
		if (tileColor == Exelius::Colors::Flame.GetHex())
			continue;

		if (tileColor == kGrassland.GetHex() && chance <= kGrasslandBurnChance)
		{
			IgniteTile(tile);
		}
		else if (tileColor == kForest.GetHex() && chance <= kForestBurnChance)
		{
			IgniteTile(tile);
		}
		else if (tileColor == kSavanna.GetHex() && chance <= kSavannaBurnChance)
		{
			IgniteTile(tile);
		}
		else if (tileColor == kSwamp.GetHex() && chance <= kSwampBurnChance)
		{
			IgniteTile(tile);
		}
	}
}
