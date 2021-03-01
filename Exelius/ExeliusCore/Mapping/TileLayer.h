#pragma once

#include "Layer.h"

namespace Exelius
{
	class TileLayer final
		: public Layer
	{
	public:
		TileLayer(Map* pOwner)
			: Layer(pOwner, LayerType::TILE)
			, m_width(0)
			, m_height(0)
		{
			//
		}

		~TileLayer() = default;

		virtual void ParseLayer(tinyxml2::XMLElement* pElement) final override;

	protected:
		bool ParseLayerProperties(tinyxml2::XMLElement* pElement);

		//void ParseBase64(tinyxml2::XMLElement* pElement);
		void ParseCSV(tinyxml2::XMLElement* pElement);

		unsigned int m_width;
		unsigned int m_height;
		std::string m_encodingType;
	};

	template<>
	inline TileLayer& Layer::GetLayerAsType<TileLayer>()
	{
		return *dynamic_cast<TileLayer*>(this);
	}
}