#pragma once

#include <string>
#include <vector>

#include "tokenizer.h"
#include "parser.h"

namespace XMLParser
{
	class XMLDocument
	{
	public:
		XMLDocument(const std::string& path)
			: m_path(path)
		{
			tokenize_and_parse();
		}

	private:
		const std::string m_path;

		void tokenize_and_parse();
	};
}