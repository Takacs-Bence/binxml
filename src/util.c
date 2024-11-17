#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

int str_case_cmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
		if (diff != 0)
		{
			return diff;
		}
		s1++;
		s2++;
	}

	return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

StringList *new_string_list()
{
	StringList *list = calloc(1, sizeof(StringList));
	if (list == NULL)
	{
		printf("could not allocate new_string_list");
		exit(1);
	}
	return list;
}

void append(StringList *string_list, const char *string, size_t length)
{
	size_t new_size;

	if (string_list == NULL || string == NULL)
	{
		printf("string_list or string is NULL, will not append");
		return;
	}

	// incrementing on the list size
	new_size = string_list->size + 1;
	// reallocate the array inside the list
	char **new_array = realloc(string_list->array, new_size * sizeof(char *));

	if (new_array == NULL)
	{
		printf("could not allocate/reallocate memory for string. append not successful");
		return;
	}

	// update the data inside string_list
	string_list->size = new_size;
	string_list->array = new_array;

	// copy string into the incremented array index
	string_list->array[new_size - 1] = strdup(string);
	return;
}

void free_list(StringList *string_list)
{
	if (string_list == NULL)
	{
		printf("NULL pointer, cannot free");
		return;
	}

	size_t size = string_list->size;
	// go through the char pointers and free them all
	for (size_t i = 0; i < size; i++)
	{
		free(string_list->array[i]);
	}

	// take care of the freeing and resetting of struct values
	free(string_list->array);
	string_list->array = NULL;
	string_list->size = 0;
}
