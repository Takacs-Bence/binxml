#!/bin/sh

set -xe

clang -o ./target/binxml src/*.c -I./include -lxml2
