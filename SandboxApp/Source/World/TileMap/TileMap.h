#pragma once
#include <Managers/Graphics.h>
#include <Utilities/Vector2.h>
#include <Utilities/Color.h>

#include <vector>

class TileMap
{
	static constexpr uint32_t kDefaultTileColor = Exelius::Colors::White.GetHex();
	std::vector<uint32_t> m_tiles;
	unsigned int m_mapWidth;
	unsigned int m_mapHeight;
	unsigned int m_tileWidth;
	unsigned int m_tileHeight;
public:
	TileMap(unsigned int mapWidth, unsigned int mapHeight);

	void RenderMap() const;
	void ResetMap();

	std::vector<size_t> GetTileNeighbors(size_t tileIndex) const;
	std::vector<size_t> GetTilesInArea(Exelius::Rectangle area);
	std::vector<size_t> GetTilesOfColorInArea(Exelius::Rectangle area, Exelius::Color color);

	Exelius::Vector2f GetTilePosition(size_t tileIndex) const;
	size_t GetTileIndex(Exelius::Vector2f position) const;
	
	void SetTileColor(size_t tileIndex, Exelius::Color newColor);
	void SetTileColor(Exelius::Vector2f tilePosition, Exelius::Color newColor);
	uint32_t GetTileColor(Exelius::Vector2f tilePosition) { return m_tiles[GetTileIndex(tilePosition)]; }
	uint32_t GetTileColor(size_t tileIndex) { return m_tiles[tileIndex]; }

	const std::vector<uint32_t>& GetTiles() const { return m_tiles; }

	unsigned int GetMapWidth() const { return m_mapWidth; }
	unsigned int GetMapHeight() const { return m_mapHeight; }
	unsigned int GetTileWidth() const { return m_tileWidth; }
	unsigned int GetTileHeight() const { return m_tileHeight; }

private:
	bool IsInBounds(size_t indexToCheck) const;
};