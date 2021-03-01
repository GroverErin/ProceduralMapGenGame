#include "MapObject.h"
#include "ApplicationLayer.h"

namespace Exelius
{
	MapObject::MapObject()
		: m_id(0)
		, m_objectShape(ObjectShape::NONE)
		, m_height(0)
		, m_width(0)
		, m_rotation(0.0f)
		, m_xPosition(0.0f)
		, m_yPosition(0.0f)
	{
	}

	void MapObject::ParseObject(tinyxml2::XMLElement* pElement)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//If there is no element node then bail.
		if (!pElement)
		{
			logger.LogSevere("No XMLElement to parse in Layer.");
			return;
		}

		std::string attribute = pElement->Name();
		if (attribute != "object")
		{
			logger.LogSevere("Node failed to parse: Not a object.");
			return;
		}

		if (pElement->Attribute("name"))
		{
			m_name = pElement->Attribute("name");
		}

		if (pElement->UnsignedAttribute("id"))
		{
			m_id = pElement->UnsignedAttribute("id");
		}
		else
		{
			logger.LogSevere("Node failed to parse: No id attribute found.");
			return;
		}

		//if (pElement->FloatAttribute("x"))
		m_xPosition = pElement->FloatAttribute("x");

		//if (pElement->FloatAttribute("y"))
		m_yPosition = pElement->FloatAttribute("y");

		float width = pElement->FloatAttribute("width");
		float height = pElement->FloatAttribute("height");
		if (width && height)
		{
			m_width = width;
			m_height = height;
		}

		if (pElement->FloatAttribute("rotation"))
			m_rotation = pElement->FloatAttribute("rotation");

		for (tinyxml2::XMLElement* pPropsElement = pElement->FirstChildElement(); pPropsElement; pPropsElement = pPropsElement->NextSiblingElement())
		{
			attribute = pPropsElement->Name();
			if (attribute == "properties")
			{
				for (tinyxml2::XMLElement* pProperty = pPropsElement->FirstChildElement(); pProperty; pProperty = pProperty->NextSiblingElement())
				{
					m_properties.emplace_back();
					m_properties.back().ParseProperty(pProperty);
				}
			}
			else if (attribute == "point")
			{
				m_objectShape = ObjectShape::POINT;
			}
		}
	}
}