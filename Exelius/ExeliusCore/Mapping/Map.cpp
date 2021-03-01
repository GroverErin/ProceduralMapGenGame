#include "Map.h"
#include "ApplicationLayer.h"
#include "ResourceManagement/Resource.h"
#include "ObjectLayer.h"
#include "TileLayer.h"
#include "TileSet.h"

#include "Components/TileSetComponent.h"

namespace Exelius
{
	Map::Map()
		: m_pMapActor(nullptr)
		, m_mapVersion("")
		, m_editorVersion("")
		, m_orientation(Orientation::NONE)
		, m_renderOrder(RenderOrder::NONE)
		, m_compressionLevel(0)
		, m_mapWidth(0)
		, m_mapHeight(0)
		, m_tileWidth(0)
		, m_tileHeight(0)
		, m_isInfinite(false)
		, m_nextLayerID(0)
		, m_nextObjectID(0)
		, m_backgroundColor({ 255, 255, 255, 255 })
	{
		//
	}

	Map::~Map()
	{
		ResetMap();
	}

	bool Map::ResetMap()
	{
		m_mapVersion = "";
		m_editorVersion = "";
		m_orientation = Orientation::NONE;
		m_renderOrder = RenderOrder::NONE;
		m_compressionLevel = 0;
		m_mapWidth = 0;
		m_mapHeight = 0;
		m_tileWidth = 0;
		m_tileHeight = 0;
		m_isInfinite = false;
		m_nextLayerID = 0;
		m_nextObjectID = 0;
		m_backgroundColor = { 255, 255, 255, 255 };

		m_tileSets.clear();
		m_objectLayers.clear();
		return false;
	}

	/// \todo Support animated frames.
	/// \todo May need some additional code for other map types here.
	/// \todo Fix Background Color parsing.
	bool Map::LoadTMXMap(ResourceFile* pResourceFile, std::string& tmxFilePath)
	{
		/// \todo Assert for filepath and pResourceFile.
		
		//Pull the map from the resource file.
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();
		logger.LogInfo("Attempting to load Tiled map: ", false);
		logger.LogInfo(tmxFilePath.c_str());
		auto pMap = pResourceFile->LoadResource(tmxFilePath);

		/// \todo Assert for resource loaded.

		//Guarentee that the map instance is clean.
		ResetMap();

		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.Parse(pMap->GetData().data(), pMap->GetData().size());
		if (error != tinyxml2::XML_SUCCESS)
		{
			logger.LogSevere("Failed to load Map from file: ", false);
			logger.LogSevere(tmxFilePath.c_str());
			return ResetMap();
		}

		tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
		if (strcmp(pRoot->Name(), "map") != 0)
		{
			logger.LogSevere("Failed to find 'map' node.");
			return ResetMap();
		}

		//------------------------------------------------------------------------------------------------------------------------
		// Get Basic Map Properties
		//------------------------------------------------------------------------------------------------------------------------

		ParseMapProperties(doc);

		//------------------------------------------------------------------------------------------------------------------------
		// Create Map Actor
		//------------------------------------------------------------------------------------------------------------------------

		CreateMapActor();

		//Now we can iterate through everything else.
		for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
		{
			std::string attribute = pElement->Name();
			if (attribute == "tileset")
			{
				m_tileSets.emplace_back(std::make_unique<TileSet>());
				m_tileSets.back()->ParseTileSet(pElement, pResourceFile, tmxFilePath);
			}
			else if (attribute == "objectgroup")
			{
				m_objectLayers.emplace_back(std::make_unique<ObjectLayer>(this));
				m_objectLayers.back()->ParseLayer(pElement);
			}
			else if (attribute == "properties") //Not sure if this exists?
			{
				for (tinyxml2::XMLElement* pPropsElement = pElement->FirstChildElement(); pPropsElement; pPropsElement = pPropsElement->NextSiblingElement())
				{
					m_properties.emplace_back();
					m_properties.back().ParseProperty(pPropsElement);
				}
			}
			else if (attribute == "layer" && !m_tileSets.empty())
			{
				//There is a layer containing tiles, add the tiles to the tileset component of the map actor.
				TileLayer tileLayer(this);
				tileLayer.ParseLayer(pElement);
			}
			// This is reached when there is tile layer data but no tile sets.
			// This *shouldn't* happen unless there are NO tiles OR tilesets.
			else if (attribute == "layer" && m_tileSets.empty())
			{
				logger.LogWarning("No tilesets detected: If there is tile data then this is an error!");
			}
			else
			{
				logger.LogWarning("Unexpected Child Type, Skipping to next Child...");
			}
		}

		return true;
	}

	bool Map::ParseMapProperties(tinyxml2::XMLDocument& doc)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
		if (strcmp(pRoot->Name(), "map") != 0)
		{
			logger.LogSevere("Failed to find 'map' node.");
			return ResetMap();
		}

		m_mapVersion = pRoot->Attribute("version");

		if (m_mapVersion.empty())
		{
			logger.LogSevere("Invalid map version.");
			return ResetMap();
		}

		m_editorVersion = pRoot->Attribute("tiledversion");

		if (m_editorVersion.empty())
		{
			logger.LogSevere("Invalid editor version.");
			return ResetMap();
		}

		std::string attribute = pRoot->Attribute("orientation");
		if (attribute == "orthogonal")
		{
			m_orientation = Orientation::ORTHOGONAL;
		}
		else if (attribute == "isometric")
		{
			m_orientation = Orientation::ISOMENTRIC;
		}
		else if (attribute == "hexagonal")
		{
			m_orientation = Orientation::HEXAGONAL;
		}
		else if (attribute == "staggered")
		{
			m_orientation = Orientation::STAGGERED;
		}
		else
		{
			m_orientation = Orientation::NONE;
			logger.LogSevere(attribute.c_str(), false);
			logger.LogSevere(": Not a supported orientation.");
			return ResetMap();
		}

		attribute = pRoot->Attribute("renderorder");
		if (attribute == "right-down")
		{
			m_renderOrder = RenderOrder::RIGHTDOWN;
		}
		else if (attribute == "right-up")
		{
			m_renderOrder = RenderOrder::RIGHTUP;
		}
		else if (attribute == "left-down")
		{
			m_renderOrder = RenderOrder::LEFTDOWN;
		}
		else if (attribute == "left-up")
		{
			m_renderOrder = RenderOrder::LEFTUP;
		}
		else
		{
			m_renderOrder = RenderOrder::NONE;
			logger.LogSevere(attribute.c_str(), false);
			logger.LogSevere(": Not a supported render order.");
			return ResetMap();
		}

		if (pRoot->Attribute("compressionlevel"))
			m_compressionLevel = pRoot->UnsignedAttribute("compressionlevel");

		unsigned int width = pRoot->UnsignedAttribute("width");
		unsigned int height = pRoot->UnsignedAttribute("height");
		if (width && height)
		{
			m_mapWidth = width;
			m_mapHeight = height;
		}
		else
		{
			logger.LogSevere("Invalid 'map to tile' width or height.");
			return ResetMap();
		}

		width = pRoot->UnsignedAttribute("tilewidth");
		height = pRoot->UnsignedAttribute("tileheight");
		if (width && height)
		{
			m_tileWidth = width;
			m_tileHeight = height;
		}
		else
		{
			logger.LogSevere("Invalid tile width or height.");
			return ResetMap();
		}

		if (pRoot->Attribute("infinite"))
			m_isInfinite = pRoot->IntAttribute("infinite") != 0;

		/// \todo Getting *Tiled* backgroundcolor propertie will crash if not present.

		//BG color will be in the format "#RRGGBB" or "#AARRGGBB" depending on if an alpha is present.
		//BG color will not be in the file if it is default.
		/*attribute = pRoot->Attribute("backgroundcolor");
		if (!attribute.empty())
		{
			m_backgroundColor.a = 255;
			m_backgroundColor.r = 0;
			m_backgroundColor.g = 0;
			m_backgroundColor.b = 0;
		}*/

		unsigned int nextLayerID = pRoot->UnsignedAttribute("nextlayerid");
		unsigned int nextObjectID = pRoot->UnsignedAttribute("nextobjectid");
		if (nextLayerID && nextObjectID)
		{
			m_nextLayerID = nextLayerID;
			m_nextObjectID = nextObjectID;
		}
		else
		{
			logger.LogSevere("Invalid 'next' ID's.");
			return ResetMap();
		}

		return true;
	}

	void Map::CreateMapActor()
	{
		//Get tools for map actor creation.
		auto pGameLayer = Exelius::IApplicationLayer::GetInstance()->GetGameLayer();
		auto& pActorFactory = pGameLayer->GetActorFactory();

		//Create the actor representing the map and give it a TileSetComponent to populate.
		m_pMapActor = std::make_shared<Exelius::Actor>(pActorFactory.GetNextActorId());
		pGameLayer->AddActor(m_pMapActor->GetId(), m_pMapActor);

		size_t totalTiles = (size_t)m_mapWidth * (size_t)m_mapHeight;
		if (totalTiles <= 0)
			return;

		//Create the tileset component that will hold the tiles.
		auto pTileSetComponent = std::make_unique<Exelius::TileSetComponent>(m_pMapActor.get());
		pTileSetComponent->Initialize(totalTiles, m_tileWidth, m_tileHeight);

		m_pMapActor->AddComponent(std::move(pTileSetComponent));
	}
}