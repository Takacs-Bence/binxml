# binxml - a binary XML protocol implemented in C

Binary XML serialization protocol that uses binary structure tables to encode/decode data, allowing faster processing and smaller files sizes. Supports random-access querying and modification of data. <br>
Has a two-phase workflow, with protobuf-like compilation phase, where C files are being generated based on the XML schema. <br> 

## Advantages

XML is a standardized, highly popular, widespread tool for stroring and transporting data. <br>
Dealing with huge files it can be too verbose because of its character encoding mechanism, and processing can be done linearly. <br>

`binxml` allows random access querying and modification plus smaller network footprint. <br>

### Disadvantages
The XML schema has to be available for both parties upfront. <br> 
Comes with the usual problems of binary protocols, namely that if something does not function according to expectations, debugging not human readable formats are time consuming and hard. <br>

## Disclaimer
<strong>binxml is not producion ready!</strong>
The project is a work in progress and the initial scope has many limitations e.g. what parts of an XML schema is supported. <br>
At this point if the XSD has complexTypes or/and elements with nested anonymous complexTypes in it, it is going to work. Other components like simpleType, simpleContext, complexContext  are not supported yet. <br>
From an attribute point of view, attributes other than `name` and `type` are ignored.

`binxml` is not an EXI (Efficient Extensible Interchange) or BiM (Binary XML) implementation at the moment.

## How does it work?

// TBD

## Build and run

Being cross-platform is not a priority at this point. <br>

On a Unix-based system, build the project with clang with the `./build.sh` script. <br> 
Pre-generate C files with the `generate` commmand. See Usage. <br>

## Scopes

[X] Optional schema validation <br>
[X] Translate XSD (with limitations) to C types <br>
[X] Generate C files <br>
[ ] Add encoding functions to C files.
[ ] Add decoding functions to C files.
[ ] Add marshalling functions to C files.
[ ] Add unmarshalling functions to C files.

## Dependencies

The project relies heavily on libxml2 for XML and schema parsing/validation.

## Usage

`binxml generate <xsd_path> <output_dir>` for generating C files. <br>
Use these to write your logic and their encoding/decoding functions to handle .bin files, or their marshalling/unmarshalling functions for XML. <br>

XML files can be validated upon receiving or before sending. <br> The marshalling functions do not make validation checks. <br>

**TBD**
**not up-to-date**

Use ```binxml help``` for info on how to use the program. 
There are 4 commands at the moment, from valdidate and generate works, pack is WIP and unpack is TBD.

```
Commands:
         validate        args: <xml_path> <xsd_path>
         generate        args: <xsd_path> <output_dir>
         pack            args: <xml_path> <output_dir>
         unpack          args: <bin_path> <xsd_path> <output_dir>
```
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
