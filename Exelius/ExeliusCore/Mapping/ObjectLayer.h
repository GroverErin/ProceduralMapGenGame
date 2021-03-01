#pragma once

#include "Layer.h"
#include "Property.h"
#include "MapObject.h"

namespace Exelius
{
	class ObjectLayer final
		: public Layer
	{
	public:
		enum class RenderOrder
		{
			NONE,
			INDEX,
			YPOSITION,
			MAX_RENDER_ORDER
		};

		ObjectLayer(Map* pOwner)
			: Layer(pOwner, LayerType::OBJECT)
			, m_renderOrder(RenderOrder::NONE)
		{
			//
		}

		virtual void ParseLayer(tinyxml2::XMLElement* pElement) final override;

		const std::vector<MapObject>& GetObjects() const { return m_objects; }

	private:

		//void ParseObject(ResourceFile* pResourceFile, tinyxml2::XMLElement* pObject, tinyxml2::XMLElement* pElement);
		RenderOrder m_renderOrder;
		std::vector<Property> m_properties;
		std::vector<MapObject> m_objects;
	};

	template <>
	inline ObjectLayer& Layer::GetLayerAsType<ObjectLayer>()
	{
		return *dynamic_cast<ObjectLayer*>(this);
	}
}