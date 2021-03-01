#include "ObjectLayer.h"
#include "ApplicationLayer.h"

namespace Exelius
{
	void ObjectLayer::ParseLayer(tinyxml2::XMLElement* pElement)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		//If there is no element node then bail.
		if (!pElement)
		{
			logger.LogSevere("No XMLElement to parse in Layer.");
			return;
		}

		std::string attribute = pElement->Name();
		if (attribute != "objectgroup")
		{
			logger.LogSevere("ObjectGroup failed to parse: Not an ObjectGroup.");
			return;
		}

		if (pElement->Attribute("id"))
		{
			m_layerID = pElement->UnsignedAttribute("id");
		}
		else
		{
			logger.LogSevere("ObjectGroup failed to parse: No ID attribute found.");
			return;
		}

		if (pElement->Attribute("name"))
		{
			m_layerName = pElement->Attribute("name");
		}
		else
		{
			logger.LogSevere("ObjectGroup failed to parse: No name attribute found.");
			return;
		}

		if (pElement->Attribute("draworder") && (attribute = pElement->Attribute("draworder")) == "index")
		{
			m_renderOrder = RenderOrder::INDEX;
		}
		else
		{
			m_renderOrder = RenderOrder::YPOSITION;
		}

		for (tinyxml2::XMLElement* pObjGrpElement = pElement->FirstChildElement(); pObjGrpElement; pObjGrpElement = pObjGrpElement->NextSiblingElement())
		{
			attribute = pObjGrpElement->Name();
			if (attribute == "properties")
			{
				for (tinyxml2::XMLElement* pPropsElement = pObjGrpElement->FirstChildElement(); pPropsElement; pPropsElement = pPropsElement->NextSiblingElement())
				{
					m_properties.emplace_back();
					m_properties.back().ParseProperty(pPropsElement);
				}
			}
			else if (attribute == "object")
			{
				m_objects.emplace_back();
				m_objects.back().ParseObject(pObjGrpElement);
			}
		}
	}
}