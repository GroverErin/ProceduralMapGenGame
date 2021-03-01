#pragma once
#include "Utilities/Color.h"
#include "Game/Actor.h"
#include "Layer.h"
//#include "TileSet.h"
#include "Property.h"
#include <vector>

namespace Exelius
{
	class Layer;
	class TileSet;
	class Property;
	class ResourceFile;

	enum class Orientation
	{
		NONE,
		ORTHOGONAL,
		ISOMENTRIC,
		HEXAGONAL,
		STAGGERED,
		MAX_ORIENTATIONS
	};

	enum class RenderOrder
	{
		NONE,
		RIGHTDOWN,
		RIGHTUP,
		LEFTDOWN,
		LEFTUP,
		MAX_RENDERORDER
	};

	class Map
	{
	public:
		Map();
		~Map();

		bool ResetMap();

		/// <summary>
		/// Pull the XML formatted data from the *Tiled* ".tmx" file, and store that data into layers corresponding to the type of data.
		/// The ".tmx" file contains all the data needed to populate tiles inside the engine, so it is done inside this function.
		/// Objects and Actors stored in the ".tmx" file must be spawned outside of this function because Exelius does not have access
		/// to client defined classes, actors, and components.
		/// 
		/// \b Example:
		/// ~~~~~
		/// //Define the map filepath.
		/// std::string mapFile = "Levels/Map.tmx";
		/// 
		/// //Create map object.
		/// Exelius::Map map;
		/// if (!map.LoadTMXMap(&m_resourceFile, mapFile))
		/// {
		///		return 1; //Return failure code.
		/// }
		/// ~~~~~
		/// </summary>
		/// <param name="pResourceFile">The ".bin" file containing the compressed *Tiled* ".tmx" file.</param>
		/// <param name="tmxFilePath">The path of the *Tiled* ".tmx" file, used to pull from the ".bin" compressed file.</param>
		/// <returns>True on success, false on failure.</returns>
		bool LoadTMXMap(ResourceFile* pResourceFile, std::string& tmxFilePath);

		bool IsInfinite() { return m_isInfinite; }

		//Getter functions.
		std::shared_ptr<Actor> GetMapActor() const { return m_pMapActor; }
		const std::string& GetMapVersion() const { return m_mapVersion; }
		const std::string& GetEditorVersion() const { return m_editorVersion; }
		Orientation GetOrientation() const { return m_orientation; }
		RenderOrder GetRenderOrder() const { return m_renderOrder; }
		unsigned int GetMapWidth() { return m_mapWidth; }
		unsigned int GetMapHeight() { return m_mapHeight; }
		unsigned int GetTileWidth() { return m_tileWidth; }
		unsigned int GetTileHeight() { return m_tileHeight; }
		const Color& GetBackgroundColor() const { return m_backgroundColor; }
		const std::vector<std::unique_ptr<TileSet>>& GetTileSets() const { return m_tileSets; }
		const std::vector<Layer::LayerPtr>& GetObjectLayers() const { return m_objectLayers; }
		const std::vector<Property>& GetProperties() const { return m_properties; }

	private:

		bool ParseMapProperties(tinyxml2::XMLDocument& doc);
		void CreateMapActor();

		std::shared_ptr<Actor> m_pMapActor;
		std::vector<std::unique_ptr<TileSet>> m_tileSets;
		std::vector<Layer::LayerPtr> m_objectLayers;
		std::vector<Property> m_properties;

		//-------------------------------------------------------------------------------------------------------
		// Basic Map Properties
		//-------------------------------------------------------------------------------------------------------
		std::string m_mapVersion;
		std::string m_editorVersion;

		Orientation m_orientation;
		RenderOrder m_renderOrder;

		unsigned int m_compressionLevel;

		unsigned int m_mapWidth;
		unsigned int m_mapHeight;
		unsigned int m_tileWidth;
		unsigned int m_tileHeight;

		bool m_isInfinite;

		Color m_backgroundColor;

		unsigned int m_nextLayerID;
		unsigned int m_nextObjectID;
	};
}