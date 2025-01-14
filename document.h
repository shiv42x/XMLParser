#pragma once

#include <string>
#include <vector>
#include "tokenizer.h"

namespace XMLParser
{
	struct Attribute
	{
		Token m_name;
		Token m_value;

		Attribute(Token name, Token value)
			: m_name(name)
			, m_value(value) {}
	};

	struct Node
	{
		Token m_tag;
		Token m_innerText;
		
		std::vector<Attribute> m_attributes;

		Node* m_parent;
		std::vector<Node> m_children;
		
		Node() {}
		Node(Token tag, Token innerText, Node* parent)
			: m_tag(tag)
			, m_innerText(innerText)
			, m_parent(parent)
		{}

		void add_child(Node child)
		{
			m_children.push_back(child);
		}
	};

	class XMLDocument
	{
	public:
		XMLDocument(const std::string& path)
			: m_path(path)
		{}

		void parse();

	private:
		const std::string m_path;
		Node m_root;
	};
}