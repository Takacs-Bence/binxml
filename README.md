# POC for XML binary serialization

This project is at a fairly early stage of development and is very incomplete. <br> This is my first project implemented in C. 

## Description and goals
Protobuf-like binary serialization and deserialization tool with generated type files. <br> The goal is to produce smaller files with more efficient encoding to make communication faster between systems. <br>
The idea is, if for whatever reason an external component communicates with XML, we can internally process faster and then send back changes.

## Build and run
On a Unix-based system, run ./build.sh and ./run.sh

## Scopes

[X] Optional schema validation <br>
[ wip ] Translate XSD (with limitations, more info later) to C types <br>
[ ] Generate C files <br>
[ ] Marshall XML data to C <br> 
[ ] Serialize file as from memory to .bin <br>
[ ] Deserialize file from .bin back to XML <br>

## Dependencies

The project relies heavily on libxml2 for XML and schema parsing/validation.
