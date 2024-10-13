#include <stdio.h>
#include <string.h>

#include "schema_validation.h"
#include "source_file_generator.h"
#include "util.h"
#include "binxml_encode.h"

int main(int argc, char** argv) {
	// the only valid no arg command is help
	if (argc == 2 && str_case_cmp(argv[1], "help") == 0) {
		goto explain_usage;
		return 0;
	} else if (argc < 3) {
		goto explain_usage;
		return 1;
	}

	char* xml_path = NULL; 
	char* xsd_path = NULL;
	char* output_dir = NULL;

	char* command = argv[1];

	// parse args based upon command	
	if (str_case_cmp(command, "validate") == 0) {
		// need xml path and xsd path args
		if (argc != 4) {
			printf("binxml validate accepts exactly two parameters: <xml_path> and <xsd_path>\n");
			exit(1);
		}

		xml_path = argv[2];
		xsd_path = argv[3];

		printf("validating...\n");
		// 0 is valid
		int is_valid = validate_xml_against_xsd(xml_path, xsd_path);

		if (is_valid == 0) {
			printf("XML is valid against the schema!\n");
		} else {
			printf("XML is not valid against the schema.\n");
		}
	} else if (str_case_cmp(command, "generate") == 0) {
		// need an xsd path and an output dir
		if (argc != 4) {
			printf("binxml generate accepts exactly two parameters: <xsd_path> and <output_dir>\n");
			exit(1);
		}

		xsd_path = argv[2];
		output_dir = argv[3];

		// will generate C header file with type to output dir based upon xsd_path if successful
		generate_type_def_source_files(xsd_path, output_dir);

		binxml_encode(xml_path, output_dir);

	} else if (str_case_cmp(command, "pack") == 0) {
		// need an xml path and an output dir
		if (argc != 5) {
			printf("binxml pack accepts exactly two parameters: <xml_path> <c_types_path> and <output_dir>\n");
			exit(1);
		}

		xml_path = argv[2];
		char* c_types_path = argv[3];
		output_dir = argv[4];

	} else if (str_case_cmp(command, "unpack") == 0) {
		printf("xmlbin unpack is not yet implemented\n");		
		exit(1);
	} else {
		goto explain_usage;
		exit(1);
	}

	return 0;

explain_usage:
	// explain usage by writing commands and their arguments
	printf("Commands:\n");
	printf("\t validate \t args: <xml_path> <xsd_path>\n");
	printf("\t generate \t args: <xsd_path> <output_dir>\n");
	printf("\t pack \t\t args: <xml_path> <c_types_path> <output_dir>\n");
	printf("\t unpack \t args: <bin_path> <xsd_path> <output_dir>\n");
}

