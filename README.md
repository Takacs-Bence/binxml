# POC for XML binary serialization

This project is at a fairly early stage of development and is very incomplete. <br> This is my first project implemented in C. 

## Description and goals
Protobuf-like binary serialization and deserialization tool with generated type files. <br> The goal is to produce smaller files with more binary encoding rather than character based XML serialization, in case it matters. <br>
The idea is, if for whatever reason our system receives an XML as an input, there is an opportunity to persist or change the data internally using fewer resources (hopefully), benchmarking yet to come after the implementation is done. <br>

## Disadvantages
The XML schema has to match between communicating parties. <br> Versioning is not part of the protocol (yet). <br>
Debugging binary format is time consuming and stressful. <br>

## Build and run
On a Unix-based system, run ./build.sh and ./run.sh
There is going to be a few test apps in ./test directory.

## Scopes

[X] Optional schema validation <br>
[X] Translate XSD (with limitations) to C types <br>
[X] Generate C files <br>
[ ] Serialize file from memory to .bin <br>
[ ] Deserialize file from .bin back to memory and possibly to XML <br>

## Dependencies

The project relies heavily on libxml2 for XML and schema parsing/validation.


## Usage

There are some test XML and connected schema in the /data dir. <br>
E.g. Library.xsd contains no simpleTypes, no simple/complexContent or any types.<br>
<br>
Basically complexTypes, nested anonym complexTypes in elements are supported at this point.<br>

Running the ./run script this schema will be validated (optional with flag) and parsed. <br>

Example output: <br>

```
XML is valid against the schema!
C header file has been generated with the schema types at ./target/generated/Library_types.h
```

The content then of the header file is: <br>

```
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
```

If I make a deliberate mistake in the XML - e.g. add not supported simpleType; <br>
Then I get an error for the validity of the XML with the error description.  <br>

```
./data/Library.xsd:21: element element: Schemas parser error : element decl. 'Genre', attribute 'type': The QName value 'GenreType' does not resolve to a(n) type definition.
Failed to parse XSD schema.
XML is not valid against the schema.
```
