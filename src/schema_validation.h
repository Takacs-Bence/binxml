#ifndef SCHEMA_VALIDATION_H
#define SCHEMA_VALIDATION_H

#include "libxml/parser.h"
#include "libxml/xmlschemas.h"

// Parses both files and validates the XML against the XSD.
// Return 0 if validation succeeded, 1 otherwise.
// Will print validation errors on stdout
int validate_xml_against_xsd(const char* xml_path, const char* xsd_path);

#endif // SCHEMA_VALIDATION_H
