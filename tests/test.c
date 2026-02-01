#include <stdio.h>
#include <string.h>
#include "Library_types.h"
#include "bxml_protocol.h"
#include "complex_type.h"

static void parse_xml_node(xmlNode *node, complex_type_t *complex_type)
{
	while (node != NULL)
	{
		if (node->type == XML_ELEMENT_NODE)
		{
			printf("ct_element_t name: %s\n", node->name);

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

static void calc_el_cnt(complex_type_t *complex_type, file_header_t *file_header) {
	size_t el_cnt = 0;

	complex_type_t *iter = complex_type;

	while (iter != NULL) {
		el_cnt += iter->element_count;
		printf("calc el cnt: %zu\n", iter->element_count);
		iter = iter->next;
	}

	file_header->elmnt_cnt = el_cnt;
}

static int serialize_complex_type(complex_type_t *complex_type) {
	file_header_t file_header;
	
	file_header.magic = 0xfb;
	file_header.version = 0x1;
	printf("file header magic number: %hu\n", file_header.magic);
	printf("file header version number: %hu\n", file_header.version);

	calc_el_cnt(complex_type, &file_header);
	printf("file header element count: %zu\n", file_header.elmnt_cnt);

	file_header.dir_offset = sizeof(file_header_t);
	printf("file header element dir offset: %lu\n", file_header.dir_offset);
	file_header.data_offset = file_header.dir_offset + (file_header.elmnt_cnt * sizeof(element_entry_t));
	printf("file header data section offset: %lu\n", file_header.data_offset);

	// TODO write header to file

	// offset is relative to data_offset not file start
	unsigned long curr_offset = 0L;
	size_t idx = 0;

	element_entry_t element_entry;
	memset(&element_entry, 0, sizeof(element_entry));

	for (complex_type_t *ct = complex_type; ct != NULL; ct = ct->next) {
    	for (size_t e = 0; e < ct->element_count; e++) {

			ct_element_t *el = &ct->elements[e];
			size_t sz = el->size;

        	element_entry.offset = curr_offset;
			element_entry.size = sz;

			// TODO write entry to file
			// TODO write data to file
   	     	
			curr_offset += sz;
   		}
	}

	return 0;
}

int main()
{

	complex_type_t *complex_type = create_complex_type("./data/Library.xsd");
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

	if (serialize_complex_type(complex_type) == -1) {
		printf("Failed to serialize complex type %s\n", complex_type->name);
	}

	complex_type_free(complex_type);

	xmlFreeDoc(xml_doc);
	xmlCleanupParser();

	return 0;
}
