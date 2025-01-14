#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include "document.h"
int main()
{
	XMLParser::XMLDocument doc("test.xml");
	doc.parse();
	
	return 0;
}