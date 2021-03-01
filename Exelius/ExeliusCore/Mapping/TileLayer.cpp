#include "TileLayer.h"
#include "ApplicationLayer.h"
#include "Map.h"
#include "Components/TileSetComponent.h"
#include "TileSet.h"

#include <sstream>

namespace Exelius
{
	void TileLayer::ParseLayer(tinyxml2::XMLElement* pElement)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//If there is no element node then bail.
		if (!pElement)
		{
			logger.LogSevere("No XMLElement to parse in Layer.");
			return;
		}

		if (!ParseLayerProperties(pElement))
			return;

		for (tinyxml2::XMLElement* pNewElement = pElement->FirstChildElement(); pNewElement; pNewElement = pNewElement->NextSiblingElement())
		{
			std::string attribute = pNewElement->Name();
			if (attribute != "data")
				continue;

			m_encodingType = pNewElement->Attribute("encoding");
			if (m_encodingType == "base64")
			{
				//ParseBase64(pNewElement);
			}
			else if (m_encodingType == "csv")
			{
				ParseCSV(pNewElement);
			}
			else
			{
				logger.LogSevere("Unexpected encoding type. Skipping.");
			}
		}
	}

	bool TileLayer::ParseLayerProperties(tinyxml2::XMLElement* pElement)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		std::string attribute = pElement->Name();
		if (attribute != "layer")
		{
			logger.LogSevere("Layer failed to parse: Not a layer.");
			return false;
		}

		if (pElement->Attribute("id"))
		{
			m_layerID = pElement->UnsignedAttribute("id");
		}
		else
		{
			logger.LogSevere("Layer failed to parse: No ID attribute found.");
			return false;
		}

		if (pElement->Attribute("name"))
		{
			m_layerName = pElement->Attribute("name");
		}
		else
		{
			logger.LogSevere("Layer failed to parse: No name attribute found.");
			return false;
		}

		unsigned int width = pElement->UnsignedAttribute("width");
		unsigned int height = pElement->UnsignedAttribute("height");
		if (width && height)
		{
			m_width = width;
			m_height = height;
		}
		else
		{
			logger.LogSevere("Layer has invalid width or height.");
			return false;
		}
		return true;
	}

	void TileLayer::ParseCSV(tinyxml2::XMLElement* pElement)
	{
		std::stringstream textStream(pElement->GetText());
		std::string id;

		const auto& tileSets = m_pOwner->GetTileSets();
		auto pTileSetComponent = m_pOwner->GetMapActor()->GetComponent<TileSetComponent>();
		size_t tileIndex = 0;

		while (std::getline(textStream, id, ','))
		{
			unsigned int tileID = std::stoul(id);
			TileSetComponent::Tile tileData;

			for (const auto& tileSet : tileSets)
			{
				// If outside the "bounds" of this tileset...
				if (tileID < tileSet->GetFirstGID() - 1 || tileID >(tileSet->GetTileCount() + tileSet->GetFirstGID() - 1))
					continue;

				//If the tile is an "empty" tile...
				if (tileID <= 0)
					continue;

				tileData.m_sourceImage = tileSet->GetTexture();

				//This tile is part of this tileset so set it.
				tileSet->GetSourceAndSet(tileID, tileData.m_sourceX, tileData.m_sourceY);
				unsigned int x = (unsigned int)tileIndex % m_width;
				unsigned int y = (unsigned int)tileIndex / m_width;

				tileData.m_destinationX = x * tileSet->GetTileWidth();
				tileData.m_destinationY = y * tileSet->GetTileHeight();

				//Add it to the tileset component.
				pTileSetComponent->AddTile(tileData);

				//Exit this for loop.
				break;
			}
			++tileIndex;
		}
	}
}