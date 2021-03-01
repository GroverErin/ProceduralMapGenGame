#include "TileSet.h"
#include "ApplicationLayer.h"
#include "ResourceManagement/Resource.h"

#include <algorithm>

namespace Exelius
{
	void TileSet::ParseTileSet(tinyxml2::XMLElement* pElement, ResourceFile* pResourceFile, std::string& mapFilepath)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//If there is no element node then bail.
		if (!pElement)
		{
			logger.LogSevere("No XMLElement to parse in TileSet.");
			return;
		}

		std::string attribute = pElement->Name();
		if (attribute != "tileset")
		{
			logger.LogSevere("Tileset failed to parse: Not a tileset.");
			return;
		}

		if (pElement->Attribute("firstgid"))
		{
			m_firstGID = pElement->UnsignedAttribute("firstgid");
		}
		else
		{
			logger.LogSevere("TileSet failed to parse: No GID attribute found.");
			return;
		}

		if (pElement->Attribute("source"))
		{
			m_tileSetPath = pElement->Attribute("source");
		}
		else
		{
			logger.LogSevere("TileSet failed to parse: No source file found.");
			return;
		}

		//Get our current directory so relative paths work.
		std::replace(mapFilepath.begin(), mapFilepath.end(), '\\', '/');
		size_t seperator = mapFilepath.find_last_of('/');
		if (seperator != std::string::npos)
		{
			mapFilepath = mapFilepath.substr(0, seperator + 1);
		}
		std::string filepath = mapFilepath + m_tileSetPath;

		auto pTileSet = pResourceFile->LoadResource(filepath);

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.Parse(pTileSet->GetData().data(), pTileSet->GetData().size());
		if (error != tinyxml2::XML_SUCCESS)
		{
			logger.LogSevere("Failed to load tileset from file: ", false);
			logger.LogSevere(filepath.c_str());
			return;
		}

		tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
		if (strcmp(pRoot->Name(), "tileset") != 0)
		{
			logger.LogSevere("Failed to find 'tileset' node.");
			return;
		}

		unsigned int columns = pRoot->UnsignedAttribute("columns");
		unsigned int width = pRoot->UnsignedAttribute("tilewidth");
		unsigned int height = pRoot->UnsignedAttribute("tileheight");
		unsigned int count = pRoot->UnsignedAttribute("tilecount");

		if (width && height && columns && count)
		{
			m_tileCount = count;
			m_columns = columns;
			m_tileWidth = width;
			m_tileHeight = height;
		}
		else
		{
			logger.LogSevere("Invalid 'columns', 'tilewidth', 'tilecount', or 'tileheight'.");
			return;
		}

		for (tinyxml2::XMLElement* pNewElement = pRoot->FirstChildElement(); pNewElement; pNewElement = pNewElement->NextSiblingElement())
		{
			attribute = pNewElement->Name();
			if (attribute == "image")
			{
				if (pNewElement->Attribute("source"))
				{
					const char* imageSource = pNewElement->Attribute("source");
					filepath = mapFilepath + imageSource;
					auto pImage = pResourceFile->LoadResource(filepath);

					m_pTexture = IApplicationLayer::GetInstance()->GetGraphics()->LoadTexture(pImage);
				}
				else
				{
					logger.LogSevere("Image node is missing a 'source'. Skipping.");
				}
			}
			else
			{
				logger.LogSevere("Unexpected Child Node in TileSet. Skipping.");
			}
		}
	}

	void TileSet::GetSourceAndSet(unsigned int tileID, unsigned int& sourceX, unsigned int& sourceY)
	{
		unsigned int index = tileID - m_firstGID; //0 means no tile.
		unsigned int x = index % m_columns;
		unsigned int y = index / m_columns;
		sourceX = x * m_tileWidth;
		sourceY = y * m_tileHeight;
	}
}