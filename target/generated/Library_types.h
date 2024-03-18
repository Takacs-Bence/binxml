typedef struct {
	size_t PublishingCompanyType_count;
	PublishingCompanyType* PublishingCompany;
} LibraryType;

typedef struct {
	char* CompanyName;
	size_t Books_count;
	Books* Books;
} PublishingCompanyType;

typedef struct {
	size_t BookType_count;
	BookType* Book;
} Books;

typedef struct {
	char* Empty;
	char* Title;
	char* Author;
	int PublicationYear;
	char* Genre;
} BookType;

