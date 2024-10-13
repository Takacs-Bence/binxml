#ifndef SOURCE_FILE_GENERATOR_H
#define SOURCE_FILE_GENERATOR_H

#include "complex_type.h"

// xsd path must be pointing to a valid schema
// will generate C header file with type to output dir based upon xsd_path if successful
// output_dir_path can be NULL - in this case the generated code is printed to stdout
// otherwise it should be a valid directory, where the files will be generated
// if a file there with the same type has been generated, its content will be overriden 
void generate_type_def_source_files(const char* const xsd_path, const char* const output_dir_path);

#endif // SOURCE_FILE_GENERATOR_H
