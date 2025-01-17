#include <iostream>
#include "tokenizer.h"

#define IF_ON(character)	\
	if (current_character.has_value() && current_character.value() == character)

#define IF_ON_WHITESPACE	\
	if (current_character.has_value() && isspace((char)current_character.value()))

#define IF_ON_ASCII_ALPHA	\
	if (current_character.has_value() && is_ascii_alpha((char)current_character.value()))

#define IF_ON_EOF	\
	if (!current_character.has_value())

#define ANYTHING_ELSE	\
	if (1)

#define STATE(state)	\
	state:				\
	case Tokenizer::State::state:

#define SWITCH_TO(next_state)		\
	m_state = State::next_state;	\
	goto next_state;

#define CONSUME_CURRENT_CHAR	\
	current_character = next();

#define CONSUME_WHITESPACE																\
	while (current_character.has_value() && isspace((char)current_character.value()))	\
	{																					\
		current_character = next();														\
	}		

#define RECONSUME	\
	m_cursor--;

#define APPEND_CHAR_TO_TOKEN_DATA									\
	if (current_character.has_value())								\
		m_current_token.m_data += (char)current_character.value();	

#define LOG(x)									\
	std::cout << (char)x.value() << std::endl;	

//#define DEBUG

namespace XMLParser
{
	std::optional<uint32_t> Tokenizer::next()
	{
		if (m_cursor >= m_input.length())
			return std::nullopt;
		return m_input[m_cursor++];
	}

	std::optional<uint32_t> Tokenizer::peek(size_t offset) const
	{
		if (m_cursor + offset >= m_input.length())
			return 0;
		// - 1 because m_cursor is 1 ahead of current
		return m_input[m_cursor + offset - 1];
	}

	bool Tokenizer::next_few(const std::string& string) const
	{
		for (size_t i = 0; i < string.length(); ++i)
		{
			auto codepoint = peek(i);
			if (!codepoint.has_value())
				return false;

			if (codepoint != string[i])
				return false;
		}
		return true;
	}

	bool Tokenizer::is_ascii_alpha(std::optional<uint32_t> codepoint) const
	{
		return (codepoint >= 'A' && codepoint <= 'Z') || (codepoint >= 'a' && codepoint <= 'z');
	}

	void Tokenizer::consume(const std::string& string)
	{
		if (next_few(string))
		{
			m_cursor += string.length();
			return;
		}
	}

	void Tokenizer::create_and_append_token()
	{
#ifdef DEBUG
		switch (m_current_token.m_type)
		{
		case Token::Type::StartTag:
		{
			std::cout << "START_TAG: ";
		}
		break;
		case Token::Type::EndTag:
		{
			std::cout << "END_TAG: ";
		}
		break;
		case Token::Type::SelfClosing:
		{
			std::cout << "SELF_CLOSING: ";
		}
		break;
		case Token::Type::AttrName:
		{
			std::cout << "ATTR_NAME: ";
		}
		break;
		case Token::Type::AttrVal:
		{
			std::cout << "ATTR_VAL: ";
		}
		break;
		case Token::Type::TextContent:
		{
			std::cout << "TEXTCONTENT: ";
		}
		break;
		case Token::Type::Whitespace:
		{
			std::cout << "WHITESPACE: ";
		}
		break;
		case Token::Type::EndOfFile:
		{
			std::cout << "EOF reached.\n";
		}
		break;
		default:
		{
			std::cout << "Unknown:";
		}
		}
		std::cout << m_current_token.m_data << std::endl;
#endif

		m_tokens.push_back(m_current_token);
		m_current_token = {};
	}

	void Tokenizer::run()
	{
		std::optional<uint32_t> current_character;
		switch (m_state)
		{
			STATE(Initial)
			{
				CONSUME_CURRENT_CHAR
				IF_ON('<')
				{
					SWITCH_TO(TagOpen)
				}
				//IF_NULL
				IF_ON_EOF
				{
					m_current_token.m_data = "EOF";
					m_current_token.m_type = Token::Type::EndOfFile;
					create_and_append_token();
					return;
				}
					IF_ON_WHITESPACE // whitespace normalization?
				{
					m_current_token.m_type = Token::Type::Whitespace;
					m_current_token.m_data += " ";
					create_and_append_token();
					SWITCH_TO(Initial)
				}
					ANYTHING_ELSE
				{
					// accumulate all inner text content then create token
					// 60 = '<'
					while (peek(1).has_value() && peek(1).value() != 60)
					{
						// if on any whitespace except space, ignore
						if (current_character.value() >= 9 && current_character.value() <= 13)
						{
							CONSUME_CURRENT_CHAR
							continue;
						}
						APPEND_CHAR_TO_TOKEN_DATA
						CONSUME_CURRENT_CHAR
					}
		
					APPEND_CHAR_TO_TOKEN_DATA
					m_current_token.m_type = Token::Type::TextContent;
;					create_and_append_token();
					SWITCH_TO(Initial)
				}
				return;
			}
			STATE(TagOpen)
			{
				CONSUME_CURRENT_CHAR
				IF_ON('!')
				{
					std::cout << "Markup declaration state.\n";
					return;
				}
				IF_ON('/')
				{
					SWITCH_TO(EndTagOpen)
				}
				IF_ON_ASCII_ALPHA
				{
					m_current_token = {};
					m_current_token.m_type = Token::Type::StartTag;
					RECONSUME
					SWITCH_TO(TagName)
				}
				IF_ON_EOF
				{
					m_error_msg = "EOF before tag name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					m_error_msg = "Invalid first character in start-tag name.\n";
					SWITCH_TO(Error)
				}
			}
			STATE(EndTagOpen)
			{
				CONSUME_CURRENT_CHAR
				IF_ON_ASCII_ALPHA
				{
					m_current_token = {};
					m_current_token.m_type = Token::Type::EndTag;
					RECONSUME
					SWITCH_TO(TagName)
				}
				IF_ON('>')
				{
					m_error_msg = "Missing end-tag name.\n";
					SWITCH_TO(Error)
				}
				IF_ON_EOF
				{
					m_error_msg = "EOF before end-tag name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					m_error_msg = "Invalid first character in end-tag name.\n";
					SWITCH_TO(Error)
				}
			}
			STATE(TagName)
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					create_and_append_token();
					SWITCH_TO(BeforeAttributeName)
				}
				IF_ON('/')
				{
					// self-closing start tag
				}
				IF_ON('>')
				{
					create_and_append_token();
					SWITCH_TO(Initial)
				}
				// IF_ASCII_UPPER_ALPHA
				// IF_NULL
				IF_ON_EOF
				{
					m_error_msg = "Unexpected EOF in tag name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					APPEND_CHAR_TO_TOKEN_DATA
					SWITCH_TO(TagName)
				}
			}
			STATE(SelfClosingStartTag)
			{

			}
			STATE(BeforeAttributeName)
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					SWITCH_TO(BeforeAttributeName)
				}
				IF_ON('/')
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('>')
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON_EOF
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('=')
				{
					m_error_msg = "Unexpected '=' before attribute name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					m_current_token = {};
					m_current_token.m_type = Token::Type::AttrName;
					RECONSUME
					SWITCH_TO(AttributeName)
				}
			}
			STATE(AttributeName)
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					create_and_append_token();

					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('/')
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('>')
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON_EOF
				{
					RECONSUME
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('=')
				{
					create_and_append_token();
					SWITCH_TO(BeforeAttributeValue)
				}
				// IF_UPPER_ASCII
				// IF_NULL
				IF_ON('"')
				{
					m_error_msg = "Unexpected character in attribute name.\n";
					SWITCH_TO(Error)
				}
				// IF_ON('\'')
				IF_ON('/')
				{
					m_error_msg = "Unexpected character in attribute name.\n";
					SWITCH_TO(Error)
				}
				IF_ON('<')
				{
					m_error_msg = "Unexpected character in attribute name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					APPEND_CHAR_TO_TOKEN_DATA
					SWITCH_TO(AttributeName)
				}
			}
			STATE(AfterAttributeName)
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					SWITCH_TO(AfterAttributeName)
				}
				IF_ON('/')
				{
					SWITCH_TO(SelfClosingStartTag)
				}
				IF_ON('=')
				{
					SWITCH_TO(BeforeAttributeValue)
				}
				IF_ON('>')
				{
					SWITCH_TO(Initial)
				}
				IF_ON_EOF
				{
					m_error_msg = "Unexpected EOF in attribute name.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					//m_error_msg = "Attribute name must be followed by '='.\n";
					//SWITCH_TO(Error)
					create_and_append_token();

					m_current_token = {};
					m_current_token.m_type = Token::Type::AttrName;
					RECONSUME
					SWITCH_TO(AttributeName)
				}
			}
			STATE(BeforeAttributeValue) 
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					SWITCH_TO(BeforeAttributeValue)
				}
				IF_ON('"')
				{
					m_current_token = {};
					m_current_token.m_type = Token::Type::AttrVal;
					SWITCH_TO(AttributeValue)
				}
				// IF_ON('\'') switch to single-quote attr value
				IF_ON('>')
				{
					m_error_msg = "Missing attribute value.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					m_error_msg = "Missing opening quote for attribute value.\n";
					SWITCH_TO(Error)
				}
			}
			STATE(AttributeValue) 
			{
				CONSUME_CURRENT_CHAR
				IF_ON('"')
				{
					create_and_append_token();
					SWITCH_TO(AfterAttributeValue)
				}
				// IF_NULL
				IF_ON_EOF
				{
					m_error_msg = "Unexpected EOF in attribute value.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					APPEND_CHAR_TO_TOKEN_DATA
					SWITCH_TO(AttributeValue)
				}
			}
			STATE(AfterAttributeValue) 
			{
				CONSUME_CURRENT_CHAR
				IF_ON_WHITESPACE
				{
					SWITCH_TO(BeforeAttributeName)
				}
				IF_ON('/')
				{
					SWITCH_TO(SelfClosingStartTag)
				}
				IF_ON('>')
				{
					//emit_current_token();
					SWITCH_TO(Initial)
				}
				IF_ON_EOF
				{
					m_error_msg = "Unexpected EOF after attribute value.\n";
					SWITCH_TO(Error)
				}
				ANYTHING_ELSE
				{
					m_error_msg = "Missing whitespace between attributes.\n";
					SWITCH_TO(Error)
				}
			}
			STATE(Error)
			{
				std::cout << "TOKENIZER::ERROR::" << m_error_msg;
				return;
			}
		}
	}
}