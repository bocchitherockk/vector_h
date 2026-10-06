# Vector.h

## Generic vector library in C

`Vector.h` is a generic, dynamically sized array implementation for C. The
element type is selected when a vector is initialized, while the vector stores
its element size and capacity in an internal header.

The library is distributed as a **single header**. There is no separate
`vector.c` file, library to build, or dependency directory.

## Installation

Copy [`vector.h`](./vector.h) into your project, or add this repository to your
include path. Define `VECTOR_IMPLEMENTATION` in exactly one source file before
including the header:

```c
#include <stdio.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"
```

Include the header normally in other source files:

```c
#include "vector.h"
```

For example, compile a program with GCC:

```sh
gcc -std=gnu11 -Wall -Wextra -I/path/to/vector -o example example.c
```

The implementation is emitted only in the translation unit that defines
`VECTOR_IMPLEMENTATION`, so do not define it in more than one source file.
There is no `-lvector` link step.

## Compiler support

The header detects the compiler and enables optional implementations for
statement expressions, `typeof`, nested functions, and built-in count-leading-
zero operations where available. GCC, Clang, TCC, MinGW, and Emscripten are
recognized. MSVC is recognized but does not provide the GNU extensions used by
some convenience macros.

When statement expressions or `typeof` are unavailable, affected operations use
an alternate form that accepts an output pointer and, where necessary, the
element type. Use the form supported by the compiler selected by
`vector.h`.

The header can also be included from C++, and uses `extern "C"` for its
functions. C++ does not support the optional designated initializer arguments
of `Vector_init`; set those options with the setter functions instead.

## Usage

### Initialization and destruction

Vectors are typed pointers. Pass a pointer to the vector (`&vec`) to
operations that may resize or inspect its metadata.

```c
#include <stdbool.h>
#include <stdio.h>

#define VECTOR_IMPLEMENTATION
#include "vector.h"

int main(void) {
    int *vec = Vector_init(int);

    Vector_push(&vec, 10);
    Vector_push(&vec, 20);

    printf("length: %zu\n", Vector_get_length(&vec));
    printf("first value: %d\n", vec[0]);

    Vector_destroy(&vec);
    return 0;
}
```

The default initial capacity is `VECTOR_DEFAULT_INITIAL_CAPACITY` (4). C
supports optional initialization parameters:

```c
int *vec = Vector_init(int, .initial_capacity = 16);
```

Other initialization options are `.free_fn` and
`.calculate_optimal_capacity_fn`. They can also be configured after
initialization with `Vector_set_initial_capacity`,
`Vector_set_free_fn`, and `Vector_set_calculate_optimal_capacity_fn`.

### Metadata and access

```c
size_t element_size = Vector_get_element_size(&vec);
size_t length       = Vector_get_length(&vec);
size_t capacity     = Vector_get_capacity(&vec);
size_t initial      = Vector_get_initial_capacity(&vec);
bool is_full        = Vector_is_full(&vec);
bool is_underfilled = Vector_is_underfilled(&vec);
bool is_empty       = Vector_is_empty(&vec);

int value = vec[0];
vec[0] = 15;
```

Pointers into the vector can become invalid after an operation that resizes
the vector. Do not retain an element pointer across `Vector_push`,
`Vector_insert_at`, or another operation that may resize the allocation.

### Adding and removing elements

```c
Vector_push(&vec, 30);
Vector_insert_at(&vec, 1, 15);

int removed = Vector_pop(&vec);
int removed_at = Vector_remove_at(&vec, 0);
int removed_unordered = Vector_remove_at_unordered(&vec, 0);

Vector_clear(&vec);
```

`Vector_remove_at` preserves the order of the remaining elements.
`Vector_remove_at_unordered` is faster but replaces the removed element with
the last element. Both return the removed value when the compiler supports the
expression form. On compilers without that support, pass an output pointer
instead.

Vectors can also be concatenated:

```c
Vector_concat(&vec, &other_vec);
```

### Callbacks and utility operations

Callbacks are ordinary C functions. For an `int` vector:

```c
static bool equals(int value, int wanted) {
    return value == wanted;
}

static int ascending(int left, int right) {
    return left - right;
}

static bool is_even(int value) {
    return value % 2 == 0;
}

static void double_value(int *value) {
    *value *= 2;
}

static int add(int accumulator, int value) {
    return accumulator + value;
}

size_t index = Vector_index_of(&vec, 10, equals);
size_t count = Vector_count(&vec, 10, equals);
Vector_sort(&vec, ascending);
size_t inserted = Vector_insert_sorted(&vec, 10, ascending);

int *copy = Vector_copy(&vec);
int *evens = Vector_filter(&vec, is_even);
Vector_foreach(&vec, double_value);
int sum = Vector_reduce(&vec, add, 0);
bool all_even = Vector_all(&vec, is_even);
bool any_even = Vector_any(&vec, is_even);
int *every_other = Vector_slice(&vec, 0, Vector_get_length(&vec), 2);
```

`Vector_map` creates a new vector and requires the result element type:

```c
static int add_two(int value) {
    return value + 2;
}

int *mapped = Vector_map(&vec, add_two, int);
```

Destroy every vector returned by `Vector_copy`, `Vector_filter`,
`Vector_map`, or `Vector_slice` when it is no longer needed:

```c
Vector_destroy(&copy);
Vector_destroy(&evens);
Vector_destroy(&every_other);
Vector_destroy(&mapped);
Vector_destroy(&vec);
```

See the [`demo/`](./demo/) directory for complete examples, including vectors
of pointers, custom initial capacities, custom cleanup functions, and
compiler-feature variants.
