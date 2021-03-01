#include "TileSetComponent.h"
#include "Managers/Graphics.h"

namespace Exelius
{
	bool TileSetComponent::Initialize(size_t numTiles, uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
		m_tiles.reserve(numTiles);
		return true;
	}

	void TileSetComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		for (auto& tile : m_tiles)
		{
			graphics->DrawTexture(tile.m_sourceImage.get(), tile.m_destinationX, tile.m_destinationY, m_width, m_height, tile.m_sourceX, tile.m_sourceY, m_width, m_height);
		}
	}

}