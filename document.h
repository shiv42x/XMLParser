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
			attempt_tokenize_and_parse();
		}

		std::shared_ptr<XMLParser::Node> m_root;

		void depth_first() const;

	private:
		const std::string m_path;

		void attempt_tokenize_and_parse();
	};
}