#include <string.h>
#include "bxml_types.h"

type_id_t map_xsd_type(const char *xsd_type)
{
    if (xsd_type == NULL || strnlen(xsd_type, 0))
        return TYPE_INVALID;

    if (strcmp(xsd_type, "xs:string") == 0)
        return TYPE_STRING;

    if (strcmp(xsd_type, "xs:int") == 0 ||
        strcmp(xsd_type, "xs:integer") == 0)
        return TYPE_INT32;

    if (strcmp(xsd_type, "xs:long") == 0)
        return TYPE_INT64;

    if (strcmp(xsd_type, "xs:boolean") == 0)
        return TYPE_BOOL;

    if (strcmp(xsd_type, "xs:float") == 0)
        return TYPE_FLOAT;

    if (strcmp(xsd_type, "xs:double") == 0)
        return TYPE_DOUBLE;

    // fallback for named complex types
    return TYPE_COMPLEX;
}

const char *c_type_from_type_id(type_id_t t)
{
    switch (t)
    {
        case TYPE_STRING:  return "char*";
        case TYPE_INT32:   return "int";
        case TYPE_INT64:   return "long";
        case TYPE_FLOAT:   return "float";
        case TYPE_DOUBLE:  return "double";
        case TYPE_BOOL:    return "int";
        default:           return NULL;   // complex or unknown
    }
}