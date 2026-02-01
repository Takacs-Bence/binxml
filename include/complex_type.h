#ifndef COMPLEX_TYPE_H
#define COMPLEX_TYPE_H

#include "libxml/tree.h"
#include "bxml_types.h"

#define ELEMENTS_INITIAL_CAPACITY 5

typedef struct
{
	char *name;	 					// name="MyElementType"
	char *type;	 					// type-"xs:string" or xs:integer or complex/simpleType reference
	size_t size; 					// size of the element in bytes TODO populate
} Element;
} ct_element_t;

typedef struct complex_type_t
{
	char *name; 					// name attribute of complexType e.g. ExampleType in <xs:complexType name="ExampleType">
	ct_element_t *elements;
	size_t element_count;
	struct complex_type_t *next; 		// linked list to get next type to process
	struct complex_type_t *prev; 		// make it doubly linked list
} complex_type_t;


complex_type_t *create_complex_type(const char *const xsd_path);

// complex_type - last element of the types linked list
// name - only give name, if the complexType is buried under an element
// - then it is likely it won't have a 'name' property and the param will be helpful generating the name
// complex_element - the complexType node
//
// recursively creates as many ComplexTypes as the element has, including embedded types
//
// returns the last created complex_type_t
// no guarantee which type it will be in the doubly linked list
// use complex_type_iterate_to_first_link to get the first element
complex_type_t *complex_type_create(complex_type_t *complex_type, const char *const name, xmlNodePtr complex_element);

// returns the number of bytes this complex type tree will take to encode in a header section of the binary file
// that will help to count the offsets for the data section on the binary file
size_t complex_type_calc_header_size(complex_type_t *complex_type);

void complex_type_free(complex_type_t *complex_type);

#endif // COMPLEX_TYPE_H
