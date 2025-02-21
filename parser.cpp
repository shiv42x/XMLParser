#include "parser.h"

namespace XMLParser
{
	void XMLParser::Parser::run(std::vector<Token>& tokens)
	{
		for (const auto& token : tokens)
		{
			switch (token.type())
			{
			case Token::Type::StartTag:
				handle_start_tag(token);
				break;
			case Token::Type::EndTag:
				handle_end_tag(token);
				break;
			case Token::Type::SelfClosing:
				handle_self_closing(token);
				break;
			case Token::Type::AttrName:
				handle_attr_name(token);
				break;
			case Token::Type::AttrVal:
				handle_attr_val(token);
				break;
			case Token::Type::TextContent:
				handle_text_content(token);
				break;
			case Token::Type::Whitespace:
				handle_whitespace(token);
				break;
			case Token::Type::EndOfFile:
				handle_eof(token);
				break;
			default:
				break;
			}
		}
	}
	void Parser::handle_start_tag(const Token& token)
	{
		auto new_node = std::make_shared<Node>(token.data());
		
		if (!m_root)
		{
			m_root = new_node;
		}

		if (m_current_node)
		{
			new_node->m_parent = m_current_node;
			m_current_node->add_child(new_node);
		}

		m_current_node = new_node;
	}
	void Parser::handle_end_tag(const Token& token)
	{
		if (m_current_node && m_current_node->m_tag_name == token.data())
		{
			m_current_node = m_current_node->m_parent;
		}
		else
			std::cerr << "PARSER::ERROR::Mismatched tags: " << m_current_node->m_tag_name.data() 
			<< " and " << token.data() << std::endl;
	}
	void Parser::handle_self_closing(const Token& token)
	{
		auto new_node = std::make_shared<Node>(token.data());
		if (m_current_node)
		{
			m_current_node->m_children.push_back(new_node);
		}
	}
	void Parser::handle_attr_name(const Token& token)
	{
		if (m_current_node)
		{
			Attribute attribute(token.data());
			m_current_node->m_attributes.push_back(attribute);
		}
	}
	void Parser::handle_attr_val(const Token& token)
	{
		if (m_current_node)
		{
			m_current_node->m_attributes.back().m_value = token.data();
		}
	
	}
	void Parser::handle_text_content(const Token& token)
	{
		if (m_current_node)
		{
			m_current_node->m_inner_text = token.data();
		}
	}
	void Parser::handle_whitespace(const Token& token)
	{}
	void Parser::handle_eof(const Token& token)
	{}
}