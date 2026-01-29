#ifndef BIN_PROTOCOL.H
#define BIN_PROTOCOL_H

#include <stdio.h>

typedef struct FileHeader {
    unsigned short magic;        // 'BXML'
    unsigned short version;      
    size_t elmnt_cnt;
    unsigned long dir_offset;   // where ElementDirectory starts
    unsigned long data_offset;  // where DataSection starts
} file_header;

typedef struct ElementEntry {
    unsigned int name_hash;    // or string table index later
    unsigned char type_id;     // enum for xs:string, xs:int, etc
    unsigned long offset;      // offset into DataSection
    size_t size;               // size of stored data
} elmnt_entry;


#endif // BIN_PROTOCOL_H