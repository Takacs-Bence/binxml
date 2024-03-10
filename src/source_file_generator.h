#include <stdio.h>
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

typedef struct ComplexType {
	char* name;                 // name attribute of complexType e.g. <xs:complexType name="ExampleType">
	signed char is_sequence;    // 0 if the second level of the complexType is an xs:sequence element
	Element* elements;
	size_t element_count;
	struct ComplexType* next;           // linked list to get next type to process
	struct ComplexType* prev;			// make it doubly linked list
} ComplexType;

// complex_type - last element of the types linked list
// name - name of the type, can be NULL - then a name will be generated
// first_element - pointer to the elements that going to be fields
// 
// creates ComplexType from the params at hand
//
// returns pointer to the newly created ComplexType
ComplexType* new_complex_type(ComplexType* complex_type, char* name, xmlNodePtr first_element);

void generate_type_def_source_files(const char* const xsd_path);

void free_resources(ComplexType* complex_type);
