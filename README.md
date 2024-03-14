# POC for XML binary serialization

This project is at a fairly early stage of development and is very incomplete. <br> This is my first project implemented in C. 

## Description and goals
Protobuf-like binary serialization and deserialization tool with generated type files. <br> The goal is to produce smaller files with more efficient encoding to make communication faster between systems. <br>
The idea is, if for whatever reason an external component communicates with XML, we can internally process faster and then send back changes.

## Build and run
On a Unix-based system, run ./build.sh and ./run.sh

## Scopes

[X] Optional schema validation <br>
[X] Translate XSD (with limitations) to C types <br>
[ ] Generate C files <br>
[ ] Marshall XML data to C <br> 
[ ] Serialize file as from memory to .bin <br>
[ ] Deserialize file from .bin back to XML <br>

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
LibraryType complex type has 1 elements
element name: PublishingCompany  element type PublishingCompanyType
PublishingCompanyType complex type has 2 elements
element name: CompanyName  element type xs:string
element name: Books  element type ComplexType\_Books
ComplexType\_Books complex type has 1 elements
element name: Book  element type BookType
BookType complex type has 5 elements
element name: Empty  element type xs:string
element name: Title  element type xs:string
element name: Author  element type xs:string
element name: PublicationYear  element type xs:integer
element name: Genre  element type xs:string
```


If I make a deliberate mistake in the XML - e.g. add not supported simpleType; <br>
Then I get an error for the validity of the XML with the error description.  <br>

```
./data/Library.xsd:21: element element: Schemas parser error : element decl. 'Genre', attribute 'type': The QName value 'GenreType' does not resolve to a(n) type definition.
Failed to parse XSD schema.
XML is not valid against the schema.
```
