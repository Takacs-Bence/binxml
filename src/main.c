#include <stdio.h>
#include "schema_validation.h"


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

	return 0;
}
