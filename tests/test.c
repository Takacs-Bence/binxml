#include <stdio.h>
#include <string.h>
#include "Library_types.h"
#include "complex_type.h"

static void parse_xml_node(xmlNode *node, ComplexType *complex_type)
{
	while (node != NULL)
	{
		if (node->type == XML_ELEMENT_NODE)
		{
			printf("Element name: %s\n", node->name);

			// Match node name with schema
			for (size_t i = 0; i < complex_type->element_count; i++)
			{
				if (strcmp((const char *)node->name, complex_type->elements[i].name) == 0)
				{
					printf("Matched element: %s\n", complex_type->elements[i].name);

					// Populate element data
					if (complex_type->elements[i].type != NULL &&
						strcmp(complex_type->elements[i].type, "xs:string") == 0)
					{
						complex_type->elements[i].name = (char *)xmlNodeGetContent(node);
						printf("Content: %s\n", complex_type->elements[i].name);
					}
				}
			}
		}
		// Recurse into children
		parse_xml_node(node->children, complex_type);

		// Move to the next sibling
		node = node->next;
	}
}

int main()
{

	ComplexType *complex_type = create_complex_type("./data/Library.xsd");
	printf("complex type %s\n", complex_type->name);

	char *xml_path = "./data/Library.xml";
	xmlDocPtr xml_doc = xmlReadFile(xml_path, NULL, 0);
	if (xml_doc == NULL)
	{
		printf("Failed to parse XML file at path %s\n", xml_path);
		return 1;
	}

	xmlNode *root = xmlDocGetRootElement(xml_doc);
	parse_xml_node(root, complex_type);

	complex_type_free(complex_type);

	xmlFreeDoc(xml_doc);
	xmlCleanupParser();

	return 0;
}
