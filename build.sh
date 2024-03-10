#!/bin/sh

set -xe

clang -o ./target/binxml src/*.c -lxml2
