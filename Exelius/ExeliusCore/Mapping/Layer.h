#pragma once
#include <ThirdParty/TinyXML2/tinyxml2.h>
#include <memory>
#include <string>

namespace Exelius
{
	class Map;

	class Layer
	{
	public:
		using LayerPtr = std::unique_ptr<Layer>;

		enum class LayerType
		{
			NONE,
			TILE,
			OBJECT,
			IMAGE,
			GROUP,
			MAX_LAYER_TYPE
		};

		Layer(Map* pOwner, LayerType type)
			: m_pOwner(pOwner)
			, m_layerID(0)
			, m_layerType(type)
		{
			//
		}

		virtual ~Layer() = default;

		LayerType GetLayerType() { return m_layerType; }

		template <typename Type>
		Type& GetLayerAsType();

		template <typename Type>
		const Type& GetLayerAsType() const { return GetLayerAsType<Type>(); }

		virtual void ParseLayer(tinyxml2::XMLElement* pElement) = 0;

		const std::string& GetName() const { return m_layerName; }
		unsigned int GetID() const { return m_layerID; }

	protected:
		Map* m_pOwner;
		unsigned int m_layerID;
		std::string m_layerName;
		LayerType m_layerType;
	};
}