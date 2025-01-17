#pragma once
#include <memory>
#include <iostream>

#include "tokenizer.h"

namespace XMLParser
{
	struct Attribute
	{
		std::string m_name;
		std::string m_value;

		Attribute(std::string name)
			: m_name(name) {}
		Attribute(std::string name, std::string value)
			: m_name(name)
			, m_value(value) {}
	};

	struct Node
	{
		std::string m_tag_name;
		std::string m_inner_text;

		std::vector<Attribute> m_attributes;

		std::shared_ptr<Node> m_parent;
		std::vector<std::shared_ptr<Node>> m_children;

		Node() {}
		Node(std::string tag_name)
			: m_tag_name(tag_name)
		{}

		void add_child(std::shared_ptr<Node> child)
		{
			m_children.push_back(child);
		}
	};
	
	class Parser
	{
	friend class XMLDocument;
	public:
		void run(std::vector<Token>& tokens);

	private:
		std::vector<Token> m_tokens;
		std::shared_ptr<Node> m_root;
		std::shared_ptr<Node> m_current_node;

		void handle_start_tag(const Token& token);
		void handle_end_tag(const Token& token);
		void handle_self_closing(const Token& token);
		void handle_attr_name(const Token& token);
		void handle_attr_val(const Token& token);
		void handle_text_content(const Token& token);
		void handle_whitespace(const Token& token);
		void handle_eof(const Token& token);
	};
}