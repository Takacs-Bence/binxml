#include <stdio.h>
#include "Library_types.h"
#include "complex_type.h"

int main() {
	
	ComplexType *complex_type = create_complex_type("./data/Library.xsd");
	printf("complex type %s", complex_type->name);
	complex_type_free(complex_type);

	printf("was successful\n");
	return 0;
}
