#include "Property.h"
#include "ApplicationLayer.h"

namespace Exelius
{
	Property::Property()
		: m_propertyType(Type::NONE)
		, m_boolValue(false)
		, m_floatValue(0.0f)
		, m_intValue(0)
		, m_colorValue({255, 255, 255, 255})
	{
		//
	}

	void Property::ParseProperty(tinyxml2::XMLElement* pElement)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//If there is no element node then bail.
		if (!pElement)
		{
			logger.LogSevere("No XMLElement to parse in Layer.");
			return;
		}

		std::string attribute = pElement->Name();
		if (attribute != "property")
		{
			logger.LogSevere("Node failed to parse: Not a property.");
			return;
		}

		if (pElement->Attribute("name"))
		{
			m_name = pElement->Attribute("name");
		}
		else
		{
			logger.LogSevere("Node failed to parse: No name attribute found.");
			return;
		}

		if (pElement->Attribute("type"))
		{
			attribute = pElement->Attribute("type");
		}

		if (attribute == "bool")
		{
			attribute = pElement->Attribute("value");
			m_boolValue = (attribute == "true");
			m_propertyType = Type::BOOLEAN;
			return;
		}
		else if (attribute == "int")
		{
			m_intValue = pElement->IntAttribute("value");
			m_propertyType = Type::INTEGER;
			return;
		}
		else if (attribute == "float")
		{
			m_floatValue = pElement->FloatAttribute("value");
			m_propertyType = Type::FLOAT;
			return;
		}
		else if (attribute == "string")
		{
			m_stringValue = pElement->Attribute("value");

			//Get multiline string.
			if (m_stringValue.empty())
				m_stringValue = pElement->FirstChildElement()->Value();

			m_propertyType = Type::STRING;
			return;
		}
		else if (attribute == "color")
		{
			m_colorValue.r = 255;
			m_colorValue.g = 255;
			m_colorValue.b = 255;
			m_colorValue.a = 255;
			m_propertyType = Type::COLOR;
			return;
		}
		else if (attribute == "file")
		{
			m_stringValue = pElement->Attribute("value");
			m_propertyType = Type::FILE;
			return;
		}
		else
		{
			m_stringValue = pElement->Attribute("value");

			//Get multiline string.
			if (m_stringValue.empty())
				m_stringValue = pElement->FirstChildElement()->Value();

			m_propertyType = Type::UNDEFINED;
			return;
		}
	}
}