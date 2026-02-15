#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

// case insensitive string compare
int str_case_cmp(const char *s1, const char *s2);

/*
 * START STRING_LIST
 */

typedef struct {
	char** array;
	size_t data_size;
} StringList;

StringList* new_string_list();

void append(StringList* string_list, const char* string, size_t length);

void free_list(StringList* string_list);

/*
 * END STRING_LIST
 */


#endif // UTIL_H
