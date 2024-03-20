# POC for XML binary serialization

Binary XML serialization protocol that uses binary tables to encode/decode data, written in C. Serialized files are more compact to send over network and faster to process. <br> 
Resembles to protobuf with generated C header files for types to allow modifications and integration into code-base. <br>
Is only a POC, not the whole XML spec is implemented. <br>

## Story and goal
Coming from a Java Enterprise background I had experience with XML, protobuf and binary table structures. <br> 
Having developed an interest for lower level programming, I developed this idea to encode an XML by restrainted by its schema definition. <br>
Being my first real C project, I am also exploring how to write C code. <br>

Two weeks into development, I have found that BiM (Binary MPEG format for XML) standard and EXI (Efficient XML Interchange) do already exist. <br>
My goal is not to write the protocol for the whole XML specification, or to compete with these implementations, but rather to finish my initial idea in a reasonable time frame. 

## Disadvantages
The XML schema has to match between communicating parties. <br> 
Versioning is not part of the protocol. <br>
Debugging binary format is time consuming and not human-friendly without tools. <br>

## Build and run
On a Unix-based system, run ```./build.sh``` . It uses clang. <br> 
There are some convinience scripts at project root to try out different command programs. <br> 
E.g. ```./validate.sh``` or ```./generate.sh``` <br>
Also see Usage section.
There is going to be a few test apps in ./test directory.

## Scopes

[X] Optional schema validation <br>
[X] Translate XSD (with limitations) to C types <br>
[X] Generate C files <br>
[ ] Serialize file from memory to .bin <br>
[ ] Deserialize file from .bin back to memory 
[ ] Marshall from memory to XML <br>

## Dependencies

The project relies heavily on libxml2 for XML and schema parsing/validation.

## Usage

There are some test XML and connected schema in the /data dir. <br>
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
