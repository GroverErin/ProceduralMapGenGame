#pragma once

#include <ThirdParty/TinyXML2/tinyxml2.h>
#include <string>
#include <memory>

namespace Exelius
{
	class ITexture;
	class ResourceFile;

	class TileSet
	{
	public:
		TileSet()
			: m_firstGID(0)
			, m_tileWidth(0)
			, m_tileHeight(0)
			, m_columns(0)
			, m_tileCount(0)
		{

		}

		void ParseTileSet(tinyxml2::XMLElement* pElement, ResourceFile* pResourceFile, std::string& mapFilepath);
//
		unsigned int GetFirstGID() const { return m_firstGID; }
		unsigned int GetTileCount() const { return m_tileCount; }
		std::shared_ptr<ITexture> GetTexture() const { return m_pTexture; }
		unsigned int GetTileWidth() const { return m_tileWidth; }
		unsigned int GetTileHeight() const { return m_tileHeight; }
		void GetSourceAndSet(unsigned int tileindex, unsigned int& sourceX, unsigned int& sourceY);
//
	private:
		unsigned int m_firstGID;
		std::string m_tileSetPath;

		unsigned int m_tileWidth;
		unsigned int m_tileHeight;
		unsigned int m_columns;
		unsigned int m_tileCount;
		std::shared_ptr<ITexture> m_pTexture;
	};
}