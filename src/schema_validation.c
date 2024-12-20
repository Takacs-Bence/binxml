#include <stdio.h>
#include "libxml/parser.h"
#include "libxml/xmlschemas.h"

void error_handler(void *userData, xmlErrorPtr error)
{
	fprintf(stderr, "Error: %s at line %d\n", error->message, error->line);
}

int validate_xml_against_xsd(const char *xml_path, const char *xsd_path)
{

	xmlDocPtr xml_doc = xmlReadFile(xml_path, NULL, 0);
	if (xml_doc == NULL)
	{
		printf("Failed to parse XML file at path %s\n", xml_path);
		return 1;
	}

	xmlDocPtr xsd_doc = xmlReadFile(xsd_path, NULL, 0);
	if (xsd_doc == NULL)
	{
		printf("Failed to parse XSD file at path %s\n", xsd_path);
		xmlFreeDoc(xml_doc);
		return 1;
	}

	// create XML schema parser context
	xmlSchemaParserCtxtPtr xsd_parser = xmlSchemaNewDocParserCtxt(xsd_doc);
	if (xsd_parser == NULL)
	{
		printf("Failed to create XML schema parser context.\n");
		xmlFreeDoc(xml_doc);
		xmlFreeDoc(xsd_doc);
		return 1;
	}

	// parse the schema
	xmlSchemaPtr xsd = xmlSchemaParse(xsd_parser);
	if (xsd == NULL)
	{
		printf("Failed to parse XSD schema.\n");
		xmlFreeDoc(xml_doc);
		xmlFreeDoc(xsd_doc);
		xmlSchemaFreeParserCtxt(xsd_parser);
		return 1;
	}

	// create a validation context
	xmlSchemaValidCtxtPtr validation_ctx = xmlSchemaNewValidCtxt(xsd);
	if (validation_ctx == NULL)
	{
		printf("Failed to create validation context.\n");
		xmlFreeDoc(xml_doc);
		xmlFreeDoc(xsd_doc);
		xmlSchemaFree(xsd);
		xmlSchemaFreeParserCtxt(xsd_parser);
		return 1;
	}

	// listen on errors and warnings for validation
	xmlSchemaSetValidErrors(validation_ctx, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);

	// validate the XML against the schema
	// if is_valid = 0, the schema is valid
	int is_valid = xmlSchemaValidateDoc(validation_ctx, xml_doc);

	xmlSchemaFreeValidCtxt(validation_ctx);
	xmlFreeDoc(xml_doc);
	xmlFreeDoc(xsd_doc);
	xmlSchemaFree(xsd);
	xmlSchemaFreeParserCtxt(xsd_parser);

	return is_valid;
}
