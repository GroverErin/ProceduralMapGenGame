#include "TileMap.h"

#include <ApplicationLayer.h>
#include <iostream>

TileMap::TileMap(unsigned int mapWidth, unsigned int mapHeight)
	: m_tiles(((size_t)mapWidth * (size_t)mapHeight), kDefaultTileColor)
	, m_mapWidth(mapWidth)
	, m_mapHeight(mapHeight)
	, m_tileWidth(0)
	, m_tileHeight(0)
{
	auto windowDimensions = Exelius::IApplicationLayer::GetInstance()->GetWindow()->GetWindowDimensions();
	m_tileWidth = windowDimensions.x / mapWidth;
	m_tileHeight = windowDimensions.y / mapHeight;

	if (m_tileWidth <= 0 || m_tileHeight <= 0)
	{
		m_tileWidth = 1;
		m_tileHeight = 1;
		std::cout << "WARNING: Invalid width or height for tilemap. Setting to 1.\n";
	}

	auto& graphics = Exelius::IApplicationLayer::GetInstance()->GetGraphicsRef();

	graphics->DrawPixelMap(m_tiles, true, m_mapWidth, m_mapHeight, m_mapWidth * 4);
}

void TileMap::ResetMap()
{
	// Fill in the tile data with the value for a white tile.
	std::fill(m_tiles.begin(), m_tiles.end(), kDefaultTileColor);
}

void TileMap::RenderMap() const
{
	auto& graphics = Exelius::IApplicationLayer::GetInstance()->GetGraphicsRef();
	auto map = graphics->GetTextureFromPixels(m_tiles, m_mapWidth, m_mapHeight, m_mapWidth * 4);

	graphics->DrawTexture(map.get(), 0, 0, 0, 0);
}

// Returning by value because there should be a compiler optimization here,
// and also there is a warning otherwise.
std::vector<size_t> TileMap::GetTileNeighbors(size_t tileIndex) const
{
	std::vector<size_t> neighbors(4, 0x0000'0000);

	//If the tile is not on the left edge of the map...
	if (tileIndex % (size_t)m_mapWidth != 0)
		neighbors[0] = (tileIndex - 1); //Add the left neighbor.

	//If the tile is not on the right edge of the map...
	if (tileIndex % (size_t)m_mapWidth != ((size_t)m_mapWidth - 1))
		neighbors[1] = (tileIndex + 1); //Add the right neighbor.

	//If the tile is not on the top edge of the map...
	if (tileIndex / (size_t)m_mapWidth != 0)
		neighbors[2] = (tileIndex - (size_t)m_mapWidth);//Add the top neighbor.

	//If the tile is not on the bottom edge of the map...
	if (tileIndex / (size_t)m_mapWidth != ((size_t)m_mapHeight - 1))
		neighbors[3] = (tileIndex + (size_t)m_mapWidth); //Add the bottom neighbor.

	return neighbors;
}

std::vector<size_t> TileMap::GetTilesInArea(Exelius::Rectangle area)
{
	std::vector<size_t> tiles;
	for (int x = area.x; x < area.x + area.w; x += m_tileWidth)
	{
		for (int y = area.y; y < area.y + area.h; y += m_tileHeight)
		{
			tiles.emplace_back(GetTileIndex({ (float)x,(float)y }));
		}
	}
	return tiles;
}

std::vector<size_t> TileMap::GetTilesOfColorInArea(Exelius::Rectangle area, Exelius::Color color)
{
	std::vector<size_t> tiles;
	for (int x = area.x; x < area.x + area.w; x += m_tileWidth)
	{
		for (int y = area.y; y < area.y + area.h; y += m_tileHeight)
		{
			if (GetTileColor({ (float)x,(float)y }) == color.GetHex())
				tiles.emplace_back(GetTileIndex({ (float)x,(float)y }));
		}
	}
	return tiles;
}

size_t TileMap::GetTileIndex(Exelius::Vector2f tilePos) const
{
	return ((size_t)(tilePos.y / (size_t)m_tileHeight) * (size_t)m_mapWidth + ((size_t)tilePos.x / (size_t)m_tileWidth));
}

Exelius::Vector2f TileMap::GetTilePosition(size_t tileIndex) const
{
	if (!IsInBounds(tileIndex))
		return { 0.0f , 0.0f };

	return { (float)((static_cast<int>(tileIndex) % m_mapWidth) * m_tileWidth), (float)((static_cast<int>(tileIndex) / m_mapWidth) * m_tileHeight) };
}

void TileMap::SetTileColor(size_t tileIndex, Exelius::Color newColor)
{
	if (IsInBounds(tileIndex))
		m_tiles[tileIndex] = newColor.GetHex();
}

void TileMap::SetTileColor(Exelius::Vector2f tilePosition, Exelius::Color newColor)
{
	SetTileColor(GetTileIndex(tilePosition), newColor);
}

bool TileMap::IsInBounds(size_t indexToCheck) const
{
	if (indexToCheck >= 0 && indexToCheck < m_tiles.size())
		return true;
	return false;
}