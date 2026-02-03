#ifndef BXML_PROTOCOL_H
#define BXML_PROTOCOL_H

#include <stdio.h>
#include "bxml_types.h"

typedef struct FileHeader {
    unsigned int magic;
    unsigned short version;      
    size_t elmnt_cnt;
    unsigned long dir_offset;   // where ElementDirectory starts
    unsigned long data_offset;  // where DataSection starts
} file_header_t;

typedef struct ElementEntry {
    unsigned int name_hash;    // or string table index later
    type_id_t type_id;         // enum for xs:string, xs:int, etc
    unsigned long offset;      // offset into DataSection
    size_t size;               // size of stored data
} element_entry_t;


#endif // BXML_PROTOCOL_H