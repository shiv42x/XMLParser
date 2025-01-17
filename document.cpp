#include <iostream>
#include <fstream>
#include <streambuf>
#include "document.h"

namespace XMLParser
{
	void XMLDocument::tokenize_and_parse()
	{
		std::ifstream f(m_path);

		if (!f.is_open())
		{
			std::cout << "ERROR:: Could not open file " << m_path << std::endl;
			return;
		}

		const std::string file_contents((std::istreambuf_iterator<char>(f)),
			std::istreambuf_iterator<char>());

		if (file_contents.empty())
		{
			std::cout << "ERROR:: File " << m_path << " is empty" << std::endl;
			return;
		}

		Tokenizer tokenizer(file_contents);
		tokenizer.run();
		std::cout << tokenizer.m_tokens.size() << " tokens found.\n";
		
		Parser parser;
		parser.run(tokenizer.m_tokens);
	}
}