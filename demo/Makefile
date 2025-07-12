# Compiler definitions
CC = gcc
CXX = g++

# Directories
BIN_DIR = bin

# Find all source files (excluding vector.c)
C_SOURCES   := $(shell find . -name "*.c")
CPP_SOURCES := $(shell find . -name "*.cpp")

# Generate binary names (remove ./ and extension, put in bin/)
C_BINARIES   := $(patsubst ./%, $(BIN_DIR)/%, $(C_SOURCES:.c=))
CPP_BINARIES := $(patsubst ./%, $(BIN_DIR)/%, $(CPP_SOURCES:.cpp=))

# All binaries
BINARIES := $(C_BINARIES) $(CPP_BINARIES)

# Default target
.PHONY: compile run clean

compile: $(BIN_DIR) $(BINARIES)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile C files
$(BIN_DIR)/%: %.c
	$(CC) $< ../vector.c -o $@

# Compile C++ files
$(BIN_DIR)/%: %.cpp
	$(CXX) $< ../vector.c -o $@

run: compile
	@for bin in $(BINARIES); do \
		echo "Running $$bin..."; \
		./$$bin; \
	done

clean:
	rm -rf $(BIN_DIR)
