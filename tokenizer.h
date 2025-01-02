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

			BeforeAttributeName,
			AttributeName,
			Error
		};

		// tokenizer state
		State m_state{ State::Initial };
		std::string m_error_msg;
		bool b_opening_quote = false;
		bool b_tokenizing_end_tag = false;

		// input and tracking
		size_t m_cursor{ 0 };
		std::string m_input;
		
		// tokens
		Token m_current_token;
		std::vector<Token> m_tokens;

		std::optional<uint32_t> next();
		std::optional<uint32_t> peek(size_t offset) const;
		bool next_few(const std::string& string) const;
		bool is_ascii_alpha(const uint32_t codepoint) const;
		void consume(const std::string& string);

		// bool is_cursor_on(uint32_t codepoint) const;
		void emit_current_token();
		void create_token(Token::Type type);
		void toggle_opening_quote();
	};
}