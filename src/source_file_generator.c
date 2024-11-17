#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libxml/parser.h"

#include "complex_type.h"
#include "source_file_generator.h"

typedef enum
{
	FILE_TYPE_H,
	FILE_TYPE_C
} FileType;

static void output_header_file(ComplexType *complex_type, FILE *output)
{
	if (!output)
	{
		perror("output file does not exist\n");
		exit(1);
	}

	// first define the typedefs first, so the order does not matter in case of interdependencies
	ComplexType *ct = complex_type;
	while (ct != NULL)
	{
		fprintf(output, "typedef struct %s %s;\n", ct->name, ct->name);

		ct = ct->prev;
	}

	// print one extra empty line to separate typedefs from structs
	fprintf(output, "\n");

	// do the iteration again, now print the structs
	ct = complex_type;
	while (ct != NULL)
	{

		fprintf(output, "struct %s {\n", ct->name);

		for (size_t i = 0; i < ct->element_count; ++i)
		{
			Element element = ct->elements[i];

			char *type;

			// handle predefined xsd types
			if (strcmp(element.type, "xs:string") == 0)
			{
				type = "char*";
			}
			else if (strcmp(element.type, "xs:integer") == 0)
			{
				type = "int";
			}
			else if (strcmp(element.type, "xs:float") == 0)
			{
				type = "float";
			}
			else if (strcmp(element.type, "xs:double") == 0)
			{
				type = "double";
			}
			else if (strcmp(element.type, "xs:boolean") == 0)
			{
				type = "int";
			}
			else
			{
				type = NULL;
			}

			if (type != NULL)
			{

				// print predefined
				fprintf(output, "\t%s %s;\n", type, element.name);
			}
			else
			{
				// handle complex types
				size_t element_type_len = strlen(element.type);

				// add a counter element, because for the pointer member that is follows, needs it
				char count_suffix[] = "_count;";
				size_t type_len = strlen(count_suffix) + element_type_len + 1;
				char count_str[type_len];
				strcpy(count_str, element.type);
				strlcat(count_str, count_suffix, type_len);
				// the size element count will be determined on the actual XML data
				fprintf(output, "\tsize_t %s\n", count_str);

				// add the pointer type derived from complex type
				char type_suffix[2] = "*";
				type_len = strlen(type_suffix) + element_type_len + 1;
				char type_val[type_len];
				strncpy(type_val, element.type, type_len);
				strlcat(type_val, type_suffix, type_len);

				fprintf(output, "\t%s %s;\n", type_val, element.name);
			}
		}

		fprintf(output, "};\n\n");

		ct = ct->prev;
	}

	// after processing the complex types, add encode/decode function definitions
	fprintf(output, "void encode(const char* const xml_path, const char* const output_dir);\n");
	fprintf(output, "void decode(const char* const xml_path, const char* const output_dir);\n");
}

/*
 * How encoding works:
 * Given the complex type instance it goes th
 *
 */

static char *encode_block(ComplexType *complex_type)
{
	// Initial allocation with space for the initial string and null terminator
	size_t initial_size = 128;
	char *code_block = calloc(initial_size, sizeof(char));
	if (code_block == NULL)
	{
		perror("Failed to allocate memory");
		exit(1);
	}
	// Starting content
	snprintf(code_block, initial_size, "\treturn;\n");

	// Example of appending more content dynamically
	const char *additional_line = "\t// Encoding logic goes here\n";
	size_t new_length = strlen(code_block) + strlen(additional_line) + 1;
	if (new_length > initial_size)
	{
		// Reallocate with a larger size and initialize new space with zero
		char *temp = realloc(code_block, new_length);
		if (temp == NULL)
		{
			free(code_block);
			perror("Failed to reallocate memory");
			exit(1);
		}
		code_block = temp;
		// No need to zero-initialize with `realloc` because it retains existing data
	}
	strcat(code_block, additional_line);

	return code_block;
}

static void output_impl_file(ComplexType *complex_type, FILE *output)
{
	if (!output)
	{
		perror("output file does not exist\n");
		exit(1);
	}

	// add imports
	fprintf(output, "#include <stdlib.h>\n");
	fprintf(output, "#include <stdio.h>\n");
	fprintf(output, "#include \"Library_types.h\"\n\n");

	size_t header_size = complex_type_calc_header_size(complex_type);
	fprintf(output, "#define\t\tHEADER_SIZE\t\t%zx\n\n", header_size);

	// add encode function
	fprintf(output, "void encode(const char* const xml_path, const char* const output_dir)\n");
	char *encoded_block = encode_block(complex_type);
	fprintf(output, "{\n%s}\n\n", encoded_block);
	free(encoded_block);

	// add decode function
	fprintf(output, "void decode(const char* const xml_path, const char* const output_dir)\n");
	fprintf(output, "{\n\treturn;\n}\n\n");
}

static ComplexType *create_complex_type(const char *const xsd_path)
{
	xmlDocPtr schema;
	xmlNodePtr root, node;

	ComplexType *complex_type = NULL;

	schema = xmlReadFile(xsd_path, NULL, 0);

	if (schema == NULL)
	{
		printf("Failed to parse XSD file at path: %s\n", xsd_path);
		exit(1);
	}

	// root is the schema
	root = xmlDocGetRootElement(schema);

	if (root == NULL)
	{
		perror("Schema is empty\n");
		xmlFreeDoc(schema);
		exit(1);
	}

	/* under schema there are elements and simple/complex types listed.
	 * the elements either point to types or they contain embedded
	 * only complexType is supported (not simpleType or simpleContent or complexContent) */
	for (node = root->children; node != NULL; node = node->next)
	{

		// complex type declaration, that has a name by which it can be referenced throughout the XML
		if (xmlStrcmp(node->name, (const xmlChar *)"complexType") == 0)
		{

			complex_type = complex_type_create(complex_type, NULL, node);
			// an element which can contain a complex type that only belongs to this element
		}
		else if (xmlStrcmp(node->name, (const xmlChar *)"element") == 0)
		{
			// there should be a complex type among the children
			xmlNode *child = node->children;

			while (child != NULL)
			{
				// there are some extra 'text' elements, but find the complex type
				if (xmlStrcmp(child->name, (const xmlChar *)"complexType") == 0)
				{
					// complexType found, recursively append this type to the type defs
					complex_type = complex_type_create(complex_type, (char *)node->name, child);
				}
				child = child->next;
			}
		}
	}
	xmlFreeDoc(schema);
	return complex_type;
}

/*
 * generates file name based on inputs and returns the file handle or exits program.
 * if the file already exists it will try and truncate its content.
 */
static FILE *create_file_handle(const char *const xsd_path, const char *const output_dir_path, FileType fileType)
{
	// check if output_dir_path is a valid directory
	struct stat path_stat;

	// use stat to get file information
	if (stat(output_dir_path, &path_stat) != 0)
	{
		// error in checking file info, at the stat syscall
		perror("stat");
		exit(1);
	}

	// if not a directory, exit program
	if (!S_ISDIR(path_stat.st_mode))
	{
		perror("output_dir_path is not a directory\n");
		exit(1);
	}

	// get generated file name from xsd
	char generated_file_name[256];

	// get last slash that is just before the file name
	const char *last_slash = strrchr(xsd_path, '/');
	if (!last_slash)
	{
		// no slash, we are in the directory already
		last_slash = xsd_path;
	}
	else
	{
		// increment on the last slash, to get the start of file name
		last_slash++;
	}

	// get the last dot, that is before the extension
	const char *last_dot = strrchr(xsd_path, '.');
	// no support for silent extensions
	if (!last_dot)
	{
		perror("could not find extension of xsd_path\n");
		exit(1);
	}

	// get xsd file name length
	size_t file_name_length = last_dot - last_slash;
	const char *suffix = (fileType == FILE_TYPE_H) ? "_types.h" : "_types.c";
	// check if the file name length fits in the generated_file_name
	if (file_name_length <= 0 || sizeof(generated_file_name) <= (strlen(output_dir_path) + 1 + strlen(suffix) + sizeof(file_name_length)))
	{
		perror("path to output dir + xsd file name without extension + appended suffix '_types.h' must fit into 512 -1 bytes\n");
		exit(1);
	}

	/* create the output file path */

	char naked_xsd_file_name[file_name_length + 1];
	strlcpy(naked_xsd_file_name, last_slash, sizeof(naked_xsd_file_name));
	// concat the output dir
	strlcpy(generated_file_name, output_dir_path, sizeof(generated_file_name));
	// add an optional '/' char, if it's missing from end
	if (generated_file_name[strlen(generated_file_name) - 1] != '/')
	{
		strlcat(generated_file_name, "/", sizeof(generated_file_name));
	}
	// the xsd file name without the extension
	strlcat(generated_file_name, naked_xsd_file_name, sizeof(generated_file_name));
	// and some suffix
	strlcat(generated_file_name, suffix, sizeof(generated_file_name));

	// open and truncate content or create if does not exist
	FILE *file_handle = fopen(generated_file_name, "w+");
	if (file_handle == NULL)
	{
		perror("could not get handle to generated type file\n");
		exit(1);
	}
	return file_handle;
}

void generate_type_def_source_files(const char *const xsd_path, const char *const output_dir_path)
{
	// create object list and get root
	ComplexType *complex_type = create_complex_type(xsd_path);

	// if output_dir is not specified, print to stdout
	if (output_dir_path == NULL)
	{
		output_header_file(complex_type, stdout);
		output_impl_file(complex_type, stdout);
	}
	else
	{

		FILE *file_handle_h = create_file_handle(xsd_path, output_dir_path, FILE_TYPE_H);
		// generate type defs to header file
		output_header_file(complex_type, file_handle_h);
		fclose(file_handle_h);

		FILE *file_handle_c = create_file_handle(xsd_path, output_dir_path, FILE_TYPE_C);
		// generate c file implementing previously generated header file
		output_impl_file(complex_type, file_handle_c);
		fclose(file_handle_c);
	}

	printf("C files has been generated from schema %s to directory %s\n", xsd_path, output_dir_path);
	complex_type_free(complex_type);
}
