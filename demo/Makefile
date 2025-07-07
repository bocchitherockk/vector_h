# Compiler definitions
CC = gcc
CXX = g++

# Directories
BIN_DIR = bin

# Dependency
VECTOR_OBJ = ../vector.o

# Find all source files (excluding vector.c)
C_SOURCES := $(filter-out ./$(notdir ../vector.c), $(shell find . -name "*.c"))
CPP_SOURCES := $(shell find . -name "*.cpp")

# Generate binary names (remove ./ and extension, put in bin/)
C_BINARIES := $(patsubst ./%, $(BIN_DIR)/%, $(C_SOURCES:.c=))
CPP_BINARIES := $(patsubst ./%, $(BIN_DIR)/%, $(CPP_SOURCES:.cpp=))

# All binaries
BINARIES := $(C_BINARIES) $(CPP_BINARIES)

# Default target
.PHONY: compile run clean

compile: $(BIN_DIR) $(VECTOR_OBJ) $(BINARIES)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile vector.c into an object file
$(VECTOR_OBJ): ../vector.c
	$(CC) -c $< -o $@

# Compile C files and link with vector.o
$(BIN_DIR)/%: %.c $(VECTOR_OBJ)
	$(CC) $< $(VECTOR_OBJ) -o $@

# Compile C++ files and link with vector.o
$(BIN_DIR)/%: %.cpp $(VECTOR_OBJ)
	$(CXX) $< $(VECTOR_OBJ) -o $@

run: compile
	@for bin in $(BINARIES); do \
		echo "Running $$bin..."; \
		./$$bin; \
	done

clean:
	rm -rf $(BIN_DIR) $(VECTOR_OBJ)
