#include <stdio.h>
#include "schema_validation.h"
#include "source_file_generator.h"

int main(int argc, char** argv) {
	// whether or not we are going to check the XML against the XSD
	int validate_flag = 0;

	char* xml_path = NULL; 
	char* xsd_path = NULL;
	char* output_dir = NULL;

	if (argc < 3 || argc > 5) {
		printf("Usage: %s <XML_path> <XSD_path> <validate_flag> <generated_output_dir>\n", argv[0]);
		return 1;
	} else if (argc == 4) {
		validate_flag = atoi(argv[3]);

	} else if (argc == 5) {
		output_dir = argv[4];
	}

	xml_path = argv[1];
	xsd_path = argv[2];

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
	//generate_type_def_source_files(xsd_path, NULL);
	generate_type_def_source_files(xsd_path, output_dir);


	return 0;
}
