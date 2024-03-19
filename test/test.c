#include <stdio.h>
#include "Library_types.h"

int main() {
	BookType my_book = {
		.Empty = "",
		.Title = "My Book Title",
		.Author = "The Famous Author",
		.PublicationYear = 2014,
		.Genre = "Mystical Fantasy",
	};

	printf("was successful\n");
	return 0;
}
