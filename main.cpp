#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include "document.h"
int main()
{
	XMLParser::XMLDocument doc("test.xml");
	doc.depth_first();

	return 0;
}