#pragma once

#include <string>

namespace XMLParser 
{
	struct Attribute
	{
		std::string m_name;
		std::string m_value;

		Attribute(const std::string& name, const std::string& value)
			: m_name(name)
			, m_value(value) {}
	};

	class Token
	{
		friend class Tokenizer;
	public:
		enum class Type 
		{
			/* DOCTYPE_DECL, */
			StartTag,
			EndTag,
			SelfClosing,
			AttrName,
			AttrVal,
			TextContent,
			Whitespace,
			/*
			COMMENT,
			PROCESSING_INSTRUCTION,
			CHAR_ENTITY,
			CDATA_SEC,	
			*/
			EndOfFile,
		};

		//Type type() const { return m_type; }
		const std::string& data() const { return m_data; }

	private:
		Type m_type;
		std::string m_data;
	};
}