#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class ITexture;
	class TileSetComponent
		: public IActorComponent
	{
	public:
		DEFINE_COMPONENT(TileSetComponent)

			struct Tile
		{
			std::shared_ptr<ITexture> m_sourceImage = nullptr;
			unsigned int m_sourceX = 0;
			unsigned int m_sourceY = 0;
			unsigned int m_destinationX = 0;
			unsigned int m_destinationY = 0;
		};

		TileSetComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_width(0)
			, m_height(0)
		{
			//
		}

		virtual ~TileSetComponent() = default;

		virtual bool Initialize([[maybe_unused]] tinyxml2::XMLElement* pData) final override { return true; }

		bool Initialize(size_t numTiles, uint32_t width, uint32_t height);

		void AddTile(const Tile& tile) { m_tiles.emplace_back(tile); }

		virtual void Render(const std::unique_ptr<Exelius::IGraphics>& graphics) final override;

	private:
		std::shared_ptr<ITexture> m_pTexture;
		std::vector<Tile> m_tiles;
		uint32_t m_width;
		uint32_t m_height;
	};
}