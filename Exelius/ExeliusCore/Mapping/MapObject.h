#pragma once
#include "Property.h"

#include <vector>

namespace Exelius
{
	class MapObject
	{
	public:
		enum class ObjectShape
		{
			NONE,
			RECTANGLE,
			POINT,
			MAX_OBJECT_SHAPES
		};

		MapObject();

		void ParseObject(tinyxml2::XMLElement* pElement);

		const std::vector<Property>& GetProperties() const { return m_properties; }
		float GetXPosition() const { return m_xPosition; }
		float GetYPosition() const { return m_yPosition; }

		float GetWidth() const { return m_width; }
		float GetHeight() const { return m_height; }

	private:
		unsigned int m_id;
		std::string m_name;

		float m_xPosition;
		float m_yPosition;
		float m_width;
		float m_height;
		float m_rotation;

		ObjectShape m_objectShape;

		std::vector<Property> m_properties;
	};
}