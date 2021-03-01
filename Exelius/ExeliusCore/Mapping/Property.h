#pragma once
#include "Utilities/Color.h"

#include <ThirdParty/TinyXML2/tinyxml2.h>
#include <string>

namespace Exelius
{
	class Property
	{
	public:
		enum class Type
		{
			NONE,
			BOOLEAN,
			FLOAT,
			INTEGER,
			STRING,
			COLOR,
			FILE,
			UNDEFINED,
			MAX_TYPES
		};

		Property();
		~Property() = default;

		void ParseProperty(tinyxml2::XMLElement* pElement);
		const std::string& GetName() const { return m_name; }
		const std::string& GetStringValue() const { return m_stringValue; }

	private:
		Type m_propertyType;
		std::string m_name;

		bool m_boolValue;
		float m_floatValue;
		int m_intValue;
		std::string m_stringValue;
		Color m_colorValue;
	};
}