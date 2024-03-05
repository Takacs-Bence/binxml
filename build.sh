#!/bin/sh

set -xe

clang -o binxml main.c -lxml2
