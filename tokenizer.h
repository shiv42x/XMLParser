#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <set>
#include "token.h"

namespace XMLParser
{
	class Tokenizer
	{
	public:
		Tokenizer(const std::string& content)
			: m_input(content)
		{}
		void run();

	private:
		enum class State
		{
			Initial,
			TagOpen,
			EndTagOpen,
			TagName,
			SelfClosingStartTag,
			BeforeAttributeName,
			AttributeName,
			AfterAttributeName,
			BeforeAttributeValue,
			AttributeValue,
			AfterAttributeValue,
			Error
		};

		// tokenizer state
		State m_state{ State::Initial };

		// input and tracking
		size_t m_cursor{ 0 };
		std::string m_input;
		std::string m_error_msg;

		// tokens
		std::vector<Token> m_tokens;
		Token m_current_token;

		std::optional<uint32_t> next();
		std::optional<uint32_t> peek(size_t offset) const;
		bool next_few(const std::string& string) const;
		bool is_ascii_alpha(const std::optional<uint32_t> codepoint) const;
		void consume(const std::string& string);

		// bool is_cursor_on(uint32_t codepoint) const;
		void create_and_append_token();
		void create_token(Token::Type type);
	};
}