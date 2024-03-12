#include <stdio.h>
#include <stdlib.h>
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
	output_type_defs(complex_type);

	free_resources(complex_type);
	xmlFreeDoc(schema);
}

ComplexType* new_complex_type(ComplexType* complex_type, char* name, xmlNodePtr element) {

	// allocate mem for new type
	ComplexType* new_type = malloc(sizeof(ComplexType));
	if (new_type == NULL) {
		printf("could not allocate memmory for complex type definition");
		exit(1);
	}
	// set defaults
	new_type->name = NULL;
	new_type->is_sequence = 0;
	new_type->element_count = 0;
	new_type->elements = NULL;
	new_type->next = NULL;
	new_type->prev = NULL;

	// set the name
	// HINT: if the name would be not set, here is the place to generate one
	new_type->name = malloc(strlen(name) + 1);
	if (new_type->name == NULL) {
		printf("allocating complextype name was not successful");
		exit(1);
	}
	strncpy(new_type->name, name, strlen(name));

	//create a dyanmic array for elements
	size_t element_count = 0;
	size_t elements_arr_size = ELEMENTS_INITIAL_CAPACITY;
	Element* elements = malloc(sizeof(Element) * elements_arr_size);

	new_type->elements = elements;

	if (elements == NULL) {
		printf("could not allocate for elements");
		exit(1);
	}

	// initialize element default values
	for (size_t i = 0; i < elements_arr_size; ++i) {
		elements[i].name = NULL;
		elements[i].type = NULL;
	}

	// process properties
	while(element != NULL) {


		// only want to deal with elements here
		if (xmlStrcmp(element->name, (const xmlChar*) "element") == 0) {
			struct _xmlAttr* property = element->properties;

			while(property != NULL) {

				// resize the element array if full 
				if (element_count >= elements_arr_size) {
					elements_arr_size = elements_arr_size * 2;
					Element* new_elements = realloc(elements, elements_arr_size * sizeof(Element));

					// initialize element default values
					//for (size_t i = 0; i < elements_arr_size; ++i) {
					//	new_elements[i].name = NULL;
					//	new_elements[i].type = NULL;
					//}

					// have to update the pointer in the ComplexType because it was reassigned
					new_type->elements = new_elements;				
					elements = new_elements;

					if (elements == NULL) {
						printf("could not allocate for elements");
						exit(1);
					}
				}

				// get the new element pointer and increment count
				element_count++;
				Element* new_element = &elements[element_count - 1];	

				/* get the key-value pairs 
				 * there are many optional attributes for an xs:element
				 * the logic here does not take care anything other than name and type attributes
				 * validity of the XML by the schema is checked in schema_validation.h */	

				/* HINT: this would be the place to put type/name generating logic,
				 * when those attributes are missing */
				if (strncmp((char*) property->name, "type", 4) == 0) {
					new_element->type = malloc(strlen((char*) property->children->content) + 1);
					if (new_element->type == NULL) {
						printf("could not allocate for type_str");
						exit(1);
					}
					strcpy(new_element->type, (char*) property->children->content);

				} else if (strncmp((char*) property->name, "name", 4) == 0) {
					new_element->name = malloc(strlen((char*) property->children->content) + 1);
					if (new_element->name == NULL) {
						printf("could not allocate for type_str");
						exit(1);
					}
					strcpy(new_element->name, (char*) property->children->content);
				}						

				property = property->next;
			}			
		}
		new_type->element_count = element_count; 

		element = element->next;	
	}



	// link the Complextype-s
	if (complex_type != NULL) {
		new_type->prev = complex_type;
		complex_type->next = new_type;
	}

	return new_type;
}

void output_type_defs(ComplexType* complex_type) {
	ComplexType* ct = complex_type;
	while (ct != NULL) {	
		printf("starting to print with: %s\n", ct->name);
		ct= ct->next;
	}
}


void free_resources(ComplexType* complex_type) {
	while(complex_type != NULL) {
		printf("freeing complex_tye 0x%p\n", complex_type);
		ComplexType* next = complex_type->next;
		Element* elements = complex_type->elements;
		for (size_t count = 0; count < complex_type->element_count; count++) {
			free(elements[count].name);
			free(elements[count].type);
		}
		free(complex_type->elements);
		complex_type->element_count = 0;
		complex_type->is_sequence = 0;
		complex_type->elements = NULL;

		free(complex_type->name);
		complex_type->name = NULL;

		complex_type->prev = NULL;
		complex_type->next = NULL;
		free(complex_type);

		complex_type = next;
	}
}

