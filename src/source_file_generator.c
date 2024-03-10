#include <stdio.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "source_file_generator.h"

void generate_type_def_source_files(const char* const xsd_path) {
	xmlDocPtr schema;
	xmlNodePtr root, node;

	ComplexType* complex_type = NULL;

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
					// get a pointer to the first element
					xmlNode* first_element = child->children;
					complex_type = new_complex_type(complex_type, (char*) node->name, first_element);
				}
				child = child->next;
			}
		} else if (xmlStrcmp(node->name, (const xmlChar*)"simpleType") == 0) {
			// TOOD impl simpleType
		} else if  (xmlStrcmp(node->name, (const xmlChar*)"element") == 0) {
			// TODO check wether or not we have types inside of the element, process if we do
		}

	}

	printf("let see if we have complex_type saved, address: 0x%p\n", complex_type);
	free_resources(complex_type);
	xmlFreeDoc(schema);
}

ComplexType* new_complex_type(ComplexType* complex_type, char* name, xmlNodePtr element) {
	// TODO if the name is NULL generate a name

	// allocate mem for new type
	ComplexType*  new = malloc(sizeof(ComplexType));
	if (new== NULL) {
		printf("could not allocate memmory for complex type definition");
		exit(1);
	}
	// set defaults
	new->name = name;
	new->is_sequence = 0;

	// process properties
	while(element != NULL) {
		// only want to deal with elements here
		if (xmlStrcmp(element->name, (const xmlChar*) "element") == 0) {
			struct _xmlAttr* property = element->properties;
			while(property != NULL) {
				// alloc element
				Element* new_element = malloc(sizeof(Element));

				if (new_element == NULL) {
					printf("new element could not be allocated. exiting");
					exit(1);
				}

				char* name_str;
				char* type_str;
				// get the key-value pairs 
				// there are many optional attributes for an xs:element
				// the logic here does not take care anything other than name and type attributes
				// validity of the XML by the schema is checked in schema_validation.h	
				if (strncmp((char*) property->name, "type", 4) == 0) {

					int length = strlen((char*) property->children->content);


				} else if (strncmp((char*) property->name, "name", 4) == 0) {

					int length = strlen((char*) property->children->content);
					if (length == 0) {

					} else {
						name_str = malloc(sizeof(strlen((char*) property->children->content) + 1));
					}
					strcpy(name_str, (char*) property->children->content);
					new_element->name = name_str;


				}						

				property = property->next;
			}			
		}

		element = element->next;	
	}

	// link the ComplexType-s
	if (complex_type != NULL) {
		new->prev = complex_type;
		complex_type->next = new;
	}

	return new;
}

void free_resources(ComplexType* complex_type) {
	while(complex_type != NULL) {
		printf("freeing complex_tye 0x%p\n", complex_type);
		ComplexType* next = complex_type->next;
		Element* elements = complex_type->elements;
		for (size_t count = 0; count < complex_type->element_count; count++) {
			free(elements[count].name);
		}
		free(elements);
		free(complex_type);

		complex_type = next;
	}
}
