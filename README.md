# XMLParser

- Parses an XML document into a DOM-tree. 
- Will probably improve the API in the future to:
  - Edit & write to a (new) XML file.
  - Search for elements and get their attributes/inner text/children.
  - Be more performant (`string_view`, less copies, etc.)
  - ...and more as needed.

## Usage
- A `shared_ptr` to the root node exists in an `XMLDocument` object (`m_root`).
- `depth_first()` will print the node's content with pre-order depth-first traversal.
```c++
#include "document.h"
int main()
{
	XMLParser::XMLDocument doc("test.xml");
	std::cout << doc.m_root->m_tag_name << std::endl;
	doc.depth_first();

	return 0;
}
```
### Example (`test.xml`):
- Probably hurts to look at, but couldn't think of a better naming convention for children tags.
```xml
<root root="root">
	<child-0 c-0="c-0">
		<child-0-0 c-0-0="c-0-0">
			0-0's text!
			<child-0-0-0 c-0-0-0="c-0-0-0" bla="bla">
				0-0-0's text!
			</child-0-0-0>
		</child-0-0>
	</child-0>
	
	<child-1 c-1="c-1">
		<child-1-0 c-1-0="c-1-0">
			1-0's text!
		</child-1-0>
	</child-1>
	<child-2></child-2>
	<child-3></child-3>
	<child-4></child-4>
</root>
```
#### Console output:
```
TAG:    root
        ATTRIB(S):      root=root
        INNERTEXT:      NONE

TAG:    child-0
        ATTRIB(S):      c-0=c-0
        INNERTEXT:      NONE

TAG:    child-0-0
        ATTRIB(S):      c-0-0=c-0-0
        INNERTEXT:      0-0's text!

TAG:    child-0-0-0
        ATTRIB(S):      c-0-0-0=c-0-0-0 bla=bla
        INNERTEXT:      0-0-0's text!

TAG:    child-1
        ATTRIB(S):      c-1=c-1
        INNERTEXT:      NONE

TAG:    child-1-0
        ATTRIB(S):      c-1-0=c-1-0
        INNERTEXT:      1-0's text!

TAG:    child-2
        ATTRIB(S):      NONE
        INNERTEXT:      NONE

TAG:    child-3
        ATTRIB(S):      NONE
        INNERTEXT:      NONE

TAG:    child-4
        ATTRIB(S):      NONE
        INNERTEXT:      NONE
```
