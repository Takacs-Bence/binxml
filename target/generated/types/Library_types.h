typedef struct LibraryType LibraryType;
typedef struct PublishingCompanyType PublishingCompanyType;
typedef struct Books Books;
typedef struct BookType BookType;

struct LibraryType {
	size_t PublishingCompanyType_count;
	PublishingCompanyType* PublishingCompany;
};

struct PublishingCompanyType {
	char* CompanyName;
	size_t Books_count;
	Books* Books;
};

struct Books {
	size_t BookType_count;
	BookType* Book;
};

struct BookType {
	char* Empty;
	char* Title;
	char* Author;
	int PublicationYear;
	char* Genre;
};

