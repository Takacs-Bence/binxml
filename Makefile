# Compiler and flags
CC = clang
CFLAGS = -Iinclude -Igenerated -I/src
LDFLAGS = -lxml2

# Source files
SRC = $(wildcard src/*.c) $(wildcard generated/*.c)
TESTS = tests/test.c

# Targets
all: binxml

# Ensure target directory exists
$(shell mkdir -p target)

# Binxml target (will build the binary)
binxml: $(SRC)
	$(CC) $(CFLAGS) -o target/binxml $^ $(LDFLAGS)

# Generate types from XSD after binxml is built
generate_types: binxml
	./generate.sh

# Test target (runs the tests after types are generated)
test: generate_types
	$(CC) $(CFLAGS) -o target/test $(TESTS) $(filter-out src/main.c, $(SRC)) $(LDFLAGS)
	./target/test

# Clean target
clean:
	rm -rf target/