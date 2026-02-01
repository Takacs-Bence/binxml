#ifndef BXML_TYPES_H
#define BXML_TYPES_H

typedef enum {
    TYPE_INVALID = 0,

    TYPE_STRING  = 1,
    TYPE_INT32   = 2,
    TYPE_UINT32  = 3,
    TYPE_INT64   = 4,
    TYPE_FLOAT  = 5,
    TYPE_DOUBLE = 6,
    TYPE_BOOL   = 7,

    TYPE_COMPLEX = 100   // reference to another complex type
} type_id_t;

type_id_t map_xsd_type(const char *xsd_type);

const char *c_type_from_type_id(type_id_t t);

#endif // BXML_TYPES_H