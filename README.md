# binxml - a binary XML protocol implemented in C

Binary XML serialization protocol that uses binary structure tables to encode/decode data, allowing faster processing and smaller files sizes. Supports random-access querying and modification of data. <br>
Has a two-phase workflow, with protobuf-like compilation phase, where C files are being generated based on the XML schema. <br> 

## Advantages

> XML is a standardized, highly popular, widespread tool for stroring and transporting data.
> 
> [Refsnes Data], "[Introduction to XML]," [https://www.w3schools.com/xml/xml\_whatis.asp], [2024].


### Disadvantages
The XML schema has to be available for both parties upfront. <br> 
Comes with the usual problems of binary protocols, namely that if something does not function according to expectations, debugging not human readable formats are time consuming and hard. <br>

## Disclaimer
<strong>binxml is not producion ready!</strong>
The project is a work in progress and the initial scope has many limitations e.g. what parts of an XML schema is supported. <br>
At this point if the XSD has complexTypes or/and elements with nested anonymous complexTypes in it, it is going to work. Other components like simpleType, simpleContext, complexContext  are not supported yet. <br>
From an attribute point of view, attributes other than `name` and `type` are ignored.

## How does it work?


## Build and run

Being cross-platform is not a priority at this point. <br>


On a Unix-based system, run ```./build.sh``` . It uses clang. <br> 
There are some convinience scripts at project root to try out different command programs. <br> 
E.g. ```./validate.sh``` or ```./generate.sh``` <br>
Also see Usage section.
There is going to be a few test apps in ./test directory.

## Scopes

[X] Optional schema validation <br>
[X] Translate XSD (with limitations) to C types <br>
[X] Generate C files <br>
[ ] Serialize file to .bin <br>
[ ] Deserialize file from .bin 
[ ] Marshall back to XML <br>

## Dependencies

The project relies heavily on libxml2 for XML and schema parsing/validation.

## Usage

There are some test XMLs and connected schema in the /data dir. <br>
E.g. Library.xsd contains no simpleTypes, no simple/complexContent or any types.<br>
<br>

Basically complexTypes, nested anonym complexTypes in elements are supported at this point.<br>

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
