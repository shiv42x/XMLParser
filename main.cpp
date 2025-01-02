#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include "tokenizer.h"

int main()
{

	//TODO: change tokenizer constructor to take file path instead
	std::ifstream f("test.xml");
	std::string str((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());

	XMLParser::Tokenizer tok(str);
	tok.run();

	return 0;
}