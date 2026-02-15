#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Library_types.h"
#include "bxml_protocol.h"
#include "complex_type.h"

static void parse_xml_node(xmlNode *node, bxml_comp_t *complex_type)
{
	while (node != NULL)
	{
		if (node->type == XML_ELEMENT_NODE)
		{
			printf("bxml_elem_t name: %s\n", node->name);

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

static void calc_el_cnt(bxml_comp_t *complex_type, file_header_t *file_header) {
	size_t el_cnt = 0;

	bxml_comp_t *iter = complex_type;

	while (iter != NULL) {
		el_cnt += iter->element_count;
		iter = iter->next;
	}

	file_header->elmnt_cnt = el_cnt;
}

static int serialize_complex_type(bxml_comp_t *complex_type, const char* output_path) {

	FILE *bin_file = fopen(output_path, "wb");
	if (bin_file == NULL) {
		printf("could not create file %s with err %s\n", output_path, strerror(errno));
		return -1;
	}

	file_header_t file_header;
	
	file_header.magic = 0x42584D4C; // 'BXML'
	file_header.version = 1;
	printf("file header magic number: %hu\n", file_header.magic);
	printf("file header version number: %hu\n", file_header.version);

	calc_el_cnt(complex_type, &file_header);
	printf("file header element count: %zu\n", file_header.elmnt_cnt);

	file_header.dir_offset = sizeof(file_header_t);
	printf("file header element dir offset: %lu\n", file_header.dir_offset);
	file_header.data_offset = file_header.dir_offset + (file_header.elmnt_cnt * sizeof(element_entry_t));
	printf("file header data section offset: %lu\n", file_header.data_offset);

	// write header to file
	fwrite(&file_header, sizeof(file_header), 1, bin_file);

	// offset is relative to data_offset not file start
	unsigned long curr_offset = 0L;
	size_t idx = 0;

	element_entry_t element_entry;
	memset(&element_entry, 0, sizeof(element_entry));

	for (bxml_comp_t *ct = complex_type; ct != NULL; ct = ct->next) {
    	for (size_t e = 0; e < ct->element_count; e++) {

			bxml_elem_t *el = &ct->elements[e];
			size_t sz = el->data_size;

        	element_entry.offset = curr_offset;
			element_entry.data_size = sz;

			// TODO write entry to file
			// TODO write data to file
   	     	
			curr_offset += sz;
   		}
	}

	if (fclose(bin_file) != 0) printf("could not close file %s with err %s\n", output_path, strerror(errno));

	return 0;
}

int main()
{
	char *xml_path = "./data/Library.xml";
	char *out_path = "./generated/bin/library_v1.bin";

	bxml_comp_t *complex_type = create_complex_type("./data/Library.xsd");
	printf("complex type %s\n", complex_type->name);

	xmlDocPtr xml_doc = xmlReadFile(xml_path, NULL, 0);
	if (xml_doc == NULL)
	{
		printf("Failed to parse XML file at path %s\n", xml_path);
		return 1;
	}

	xmlNode *root = xmlDocGetRootElement(xml_doc);
	parse_xml_node(root, complex_type);

	if (serialize_complex_type(complex_type, out_path) == -1) {
		printf("Failed to serialize complex type %s\n", complex_type->name);
	}

	complex_type_free(complex_type);

	xmlFreeDoc(xml_doc);
	xmlCleanupParser();

	return 0;
}
