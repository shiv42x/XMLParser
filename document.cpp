#include <iostream>
#include <fstream>
#include <streambuf>
#include <stack>
#include "document.h"

namespace XMLParser
{
	void XMLDocument::depth_first() const
	{
		std::shared_ptr<Node> current = m_root;
		std::stack<std::shared_ptr<Node>> child_stack;
		child_stack.push(m_root);

		while (!child_stack.empty())
		{
			std::cout << "TAG:\t" << current->m_tag_name << '\n';
			std::cout << "\tATTRIB(S):\t";
			if (current->has_attibutes())
			{
				for (auto& attrib : current->m_attributes)
					std::cout << attrib.m_name << "=" << attrib.m_value << '\t';
			}
			else
				std::cout << "NONE";
			std::cout << "\n\tINNERTEXT:\t";
			if (current->has_innertext())
				std::cout << current->m_inner_text;
			else
				std::cout << "NONE";
			std::cout << '\n';
			std::cout << '\n';

			for (auto i = current->m_children.rbegin(); i != current->m_children.rend(); i++)
				child_stack.push(*i);
			current = child_stack.top();
			child_stack.pop();
		}
	}

	void XMLParser::XMLDocument::attempt_tokenize_and_parse()
	{
		std::ifstream f(m_path);

		if (!f.is_open())
		{
			std::cout << "ERROR::Could not open file " << m_path << std::endl;
			return;
		}

		const std::string file_contents((std::istreambuf_iterator<char>(f)),
			std::istreambuf_iterator<char>());

		if (file_contents.empty())
		{
			std::cout << "ERROR::File " << m_path << " is empty" << std::endl;
			return;
		}

		Tokenizer tokenizer(file_contents);
		tokenizer.run();
		
		Parser parser;
		parser.run(tokenizer.m_tokens);
		m_root = parser.m_root;
	}
}