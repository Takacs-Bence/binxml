#include <string.h>

#include "complex_type.h"


ComplexType* complex_type_create(ComplexType* complex_type, const char* const name, xmlNodePtr complex_element) {
	// allocate mem for new type
	ComplexType* new_type = malloc(sizeof(ComplexType));
	if (new_type == NULL) {
		printf("could not allocate memmory for complex type definition");
		exit(1);
	}
	// set defaults
	new_type->name = NULL;
	new_type->element_count = 0;
	new_type->elements = NULL;
	new_type->next = NULL;
	new_type->prev = NULL;

	// try setting complex_type name first by its name attribute
	// that is usually the case when it is not inside an xs:element
	xmlChar* name_value = xmlGetProp(complex_element, (const xmlChar*)"name");

	if (name_value != NULL) {
		// set the name
		new_type->name = malloc(strlen((char*) name_value) + 1);
		if (new_type->name == NULL) {
			printf("allocating complextype name was not successful");
			exit(1);
		}
		strncpy(new_type->name, (char*) name_value, strlen((char*) name_value) + 1);

		// free the prop
		xmlFree(name_value);

		// there was no name element, hopefully the name param was set
	} else if (name != NULL) {
		// generate based upon the element name param
		//size_t name_length = strlen("ComplexType_") + strlen(name) + 1;
		size_t name_length = strlen(name) + 1;

		new_type->name = malloc(name_length);
		if (new_type->name == NULL) {
			printf("allocating complextype name was not successful");
			exit(1);
		}
		//strcpy(new_type->name, "ComplexType_");
		//strncat(new_type->name, name, name_length - strlen(new_type->name) - 1);
		strlcpy(new_type->name, name, name_length);

	} else {
		printf("complex type name could not be determined. if complexType is wrapped with an element, pass the element's name\n");
		exit(1);
	}


	/* these children should be either xs:sequence, xs:choice or xs:all 
	 * or text (which here will be empty, since the complex type doesn't have content */
	xmlNode* child = complex_element->children;

	// iterate over the children - we will find only one that is useful (the one that is not text)
	while (child != NULL)  {

		// "choice" is left out here on purpose, that would need a bit different union-like logic
		if (xmlStrcmp(child->name, (const xmlChar*)"all") == 0 ||
				xmlStrcmp(child->name, (const xmlChar*)"sequence") == 0) {

			xmlNode* element_node = child->children;

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
			while(element_node != NULL) {

				// only want to deal with elements here
				if (xmlStrcmp(element_node->name, (const xmlChar*) "element") == 0) {
					struct _xmlAttr* property = element_node->properties;

					// resize the element array if full 
					if (element_count >= elements_arr_size) {
						// double the size and reallocate memory
						size_t old_size = elements_arr_size;
						elements_arr_size = elements_arr_size * 2;
						Element* new_elements = realloc(elements, elements_arr_size * sizeof(Element));

						// have to update the pointer in the ComplexType because it was reassigned
						new_type->elements = new_elements;				
						elements = new_elements;

						if (elements == NULL) {
							printf("could not allocate for elements");
							exit(1);
						}

						// set defaults for newly allocated memory slots
						for (size_t i = old_size; i < elements_arr_size; ++i) {
							elements[i].name = NULL;
							elements[i].type = NULL;
						}
					}

					// get the new element pointer and increment count
					element_count++;
					Element* new_element = &elements[element_count - 1];	

					// a property represents an attribute
					// property->name will be the name of the attribute
					// property->children->content will be the value of the attr
					while(property != NULL) {

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
					// if the name is not empty but the type is
					// checking if it is a default type, or there is a complexType underneath
					if (new_element->name != NULL && new_element->type == NULL) {
						// get children 
						xmlNodePtr embedded = element_node->children;
						if (embedded == NULL) {
							// default type, nothing embedded here
							new_element->type = malloc(strlen("xs:string") + 1);
							if (new_element->type == NULL) {
								printf("could not allocate string for default type");
								exit(1);
							}
							strcpy(new_element->type, "xs:string");
						} else {
							// there are some extra 'text' elements, but find the complex type
							while (embedded != NULL) {

								if (xmlStrcmp(embedded->name, (const xmlChar*)"complexType") == 0) {
									// complexType found, recursively append this type to the type defs
									complex_type = complex_type_create(complex_type, new_element->name, embedded);	
									// get the type from the newly appended complex type
									new_element->type = malloc(strlen(complex_type->name + 1));
									if (new_element->type == NULL) {
										printf("could not allocate string for element type");
										exit(1);
									}
									strcpy(new_element->type, complex_type->name);
								}

								embedded = embedded->next;
							}
						}
					}
				}	
				new_type->element_count = element_count; 

				element_node = element_node->next;	
			}

		}

		child = child->next;

	}

	// link the Complextype-s
	if (complex_type != NULL) {
		new_type->prev = complex_type;
		complex_type->next = new_type;
	}

	return new_type;
}

void complex_type_free(ComplexType* complex_type) {
	while(complex_type != NULL) {
		ComplexType* next = complex_type->prev;
		Element* elements = complex_type->elements;
		for (size_t count = 0; count < complex_type->element_count; count++) {
			free(elements[count].name);
			free(elements[count].type);
		}
		free(complex_type->elements);
		complex_type->element_count = 0;
		complex_type->elements = NULL;

		free(complex_type->name);
		complex_type->name = NULL;

		complex_type->prev = NULL;
		complex_type->next = NULL;
		free(complex_type);

		complex_type = next;
	}
}
