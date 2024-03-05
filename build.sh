#!/bin/sh

set -xe

clang -o binxml src/main.c src/schema_validation.c -lxml2
