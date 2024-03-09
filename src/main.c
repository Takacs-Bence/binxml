#include <stdio.h>
#include "schema_validation.h"
#include "libxml/parser.h"
#include "libxml/tree.h"

typedef enum {
	STRING,
	INTEGER
} ElementType;

typedef struct {
	char* name;
	ElementType type;
} Element;

typedef struct {
	char* name;					// name attribute of complexType e.g. <xs:complexType name="ExampleType">
	signed char is_sequence;	// 0 if the second level of the complexType is an xs:sequence element
	Element* elements;			
	size_t element_count;
} ComplexType;


void parse_xsd(const char* const xsd_path);

int main(int argc, char** argv) {
	// whether or not we are going to check the XML against the XSD
	int validate_flag = 0;

	if (argc == 4) {
		validate_flag = atoi(argv[3]);

	} else if (argc != 3) {
		printf("Usage: %s <XML_path> <XSD_path> <validate_flag>\n", argv[0]);
		return 1;
	}

	const char* xml_path = argv[1];
	const char* xsd_path = argv[2];

	if (validate_flag) {

		// 0 is valid
		int is_valid = validate_xml_against_xsd(xml_path, xsd_path);

		if (is_valid == 0) {
			printf("XML is valid against the schema!\n");
		} else {
			printf("XML is not valid against the schema.\n");
		}
	} else {
		printf("XML validation against the XSD will be skipped.\n");
	}	
	
	// for now we are going to read the file again, TODO refactor, so it is not done twice
	// parse the XSD 
	parse_xsd(xsd_path);
	

	return 0;
}

void parse_xsd(const char* const xsd_path) {
	xmlDocPtr schema;
	xmlNodePtr root, node;

	schema = xmlReadFile(xsd_path, NULL, 0);

	if (schema == NULL) {
		printf("Failed to parse XSD file at path: %s\n", xsd_path);	
		return;
	}
	
	root = xmlDocGetRootElement(schema);
	
	if (root == NULL) {
		printf("Schema is empty\n");
		xmlFreeDoc(schema);
		return;
	}
	
	// Starting from the root level, search for the type definitions
	// These can be found one level below of the root
	// Only complexType is supported (so not simpleType or simpleContent or complexContent
    for (node = root->children; node != NULL; node = node->next) {	
		// if the current node is a complexType then process it
        if (xmlStrcmp(node->name, (const xmlChar*)"complexType") == 0) {
			xmlNode* child = node->children;
			while (child != NULL)  {
				// there are junk lines that are not relevant, here we are trying to target xs:sequence and similar
				if (child->type == XML_ELEMENT_NODE || child->type == XML_ENTITY_NODE) {	
					printf("name: %s content: %s line:%d\n", child->name, child->content, child->line);
				}
				child = child->next;
			}
        } else if (xmlStrcmp(node->name, (const xmlChar*)"simpleType") == 0) {
			// TOOD impl simpleType
		}	

	}

	xmlFreeDoc(schema);

}
