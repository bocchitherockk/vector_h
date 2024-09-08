# Generic Vector Library

## Description
This Generic Vector Library provides a flexible and efficient way to manage dynamic arrays in C. It supports various operations such as initialization, insertion, removal, and sorting of elements, alongside specialized functions for filtering, mapping, and reducing vectors. The library is designed to be adaptable for vectors containing different types of data.

## Installation
1. To integrate the Generic Vector Library into your project, follow these steps:
2. Clone the repository or download the source files into your project directory.
3. Include the vector.h header file in your C files where you need to use the vector functions.

## Features
- **Vector Initialization and Deletion**
  - Initialize and destroy vectors dynamically.
- **Element Insertion and Removal**
  - Add elements to vectors, remove elements, and pop elements from vectors.
- **Utility Functions**
  - Functions to print vector details, find elements, count elements, and get the index of elements.
- **Sorting and Searching**
  - Sort vectors in ascending order and insert elements in a sorted vector.
- **Transformation and Reduction**
  - Apply functions to elements of a vector (e.g., doubling values), map functions (e.g., add a constant to values), and reduce functions (e.g., summing elements).
- **Filtering**
  - Filter vectors based on a predicate (e.g., even numbers).
- **Miscellaneous Operations**
  - Reverse vectors, check conditions across elements (any/all), and slice vectors with a specific step.

## Compilers
This program uses preprocessor directives to ensure compatibility with different C compilers by checking support for:
- Statement expressions
- Typeof keyword
- Built-in functions like `__builtin_clz`

@note: compiler features are defined in `system_env/system_env.h` but not all of them, if you have an **MSVC compiler** and you tested that it supports `typeof` for example, feel free to change that file and add it, I only tested those features on **gcc**, **tcc** and **clang**
## Compilation and Execution
```bash
gcc your_program.c vector.c -o your_program
```
## Usage
Compile the program with a C compiler that supports C99 or above. Ensure the vector library at `./vector.h` is correctly referenced. Run the executable to see a demonstration of vector operations with output logs explaining each step.
Below are several examples demonstrating how to use the vector library functions.

#### 1. Initialization
```c
#include "vector.h"
int main() {
    int *vec = Vector_init(int);
    // Now you can use `vec` with other vector functions
}
```
#### 2. Metadata
#### 2.1. Length
```c
    size_t length = Vector_length(vec);
```
#### 2.2. Element Size
```c
    size_t element_size = Vector_element_size(vec);
```
#### 2.3. Capacity
```c
    size_t capacity = Vector_capacity(vec);
```
#### 2.4. Is full
```c
    bool is_full = Vector_is_full(vec);
```
#### 2.5. Is underfilled
```c
    bool is_underfilled = Vector_is_underfilled(vec);
```
#### 2.6. Is empty
```c
    bool is_empty = Vector_is_empty(vec);
```
#### 3. Adding Elements
#### 3.1. Push last
```c
    Vector_push(&vec, 10); // Adds an element to the end of the vector
```
#### 3.2. Insert at certain position
```c
    Vector_insert_at(&vec1, 1, 100); // inserts 100 at index 1
```
#### 3.3. Insert sorted
```c
    int index = Vector_insert_sorted(&vec2, 5, int (int value_in_vec, int value_as_param) => { return value_in_vec - value_as_param; }); // inserts 5 in it's sorted position, considering that the vector is already sorted and returns the index inserted in
```
#### 3.3. Concat
```c
    Vector_concat(&vec1, &vec2); // pushes the elements in vec2 to the end of vec1
```

#### 4. Accessing Elements
```c
    int value = vec[0]; // Access the first element
    vec[3] = 15; // setting the value at index 3 to 15
    int *ptr = &vec[0]; // this is a wrong approach, this pointer will be pointing to a wrong address when the vector gets resized
```

#### 5. Removing Elements
#### 5.1. Pop last
```c
    int value = Vector_pop(&vec); // Removes the last element and returns it
```
#### 5.2. Remove at certain position
```c
    int removed = Vector_remove_at(&vec, 3); // Removes the value at the given index and retuens it
```
#### 5.3. Remove value
```c
    int index = Vector_remove_value(&vec, 10, bool (int value_in_vec, int value_as_param) => { return value_in_vec == value_as_param; }); // Removes a given value from the vector and returns it's index, a function is provided as an argument because costum data types do not get compared using ==, also this adds flexibility in terms of using other logic rather than a simple ==, if no value matches the callback function, an assertion error will be raised
```
#### 5.4. Clearing vector
```c
    Vector_clear(&vec); // Clears all values from the vector
```

#### 6. Utility Functions
#### 6.1. Index
```c
    Vector_index(&vec, 10, bool (int value_in_vec, int value_as_param) => { return value_in_vec == value_as_param; }) // returns the index of the first value that validates the compare function. If none, raise an assertion error
```
#### 6.2. Count
```c
    Vector_count(&vec, 10, bool (int value_in_vec, int value_as_param) => { return value_in_vec == value_as_param; }) // returns the number of elements that validate the compare function
```
#### 6.3. Copy
```c
    int *copy = Vector_copy(&vec) // returns a shallow copy of the vector
```
#### 6.4. Reverse
```c
    Vector_reverse(&vec) // reverses the vector in place
```
#### 6.5. Sort
```c
    Vector_sort(&vec, int (int value_in_vec, int value_as_param) => { return value_in_vec - value_as_param; }); // Sorts the vector in place according to the sorting function given using merge sort algorithm
```
#### 6.6. Filter
```c
    int *filtered = Vector_filter(&vec, bool (int value_in_vec, int value_as_param) => { return int value_in_vec == int value_as_param }); // returns a new filtered vector
```
#### 6.7. Foreach
```c
    Vector_foreach(&vec, void (int *value_in_vec_ptr) => { *value_in_vec_ptr *= 2; }); // multiplies each value in the vector by 2, modifies the vector in place
```
#### 6.8. Map
```c
    int *mapped = Vector_map(&vec, int (int value_in_vec) => { return value_in_vec + 2 }); // Returns a new vector with each value mapped by the mapper function
```
#### 6.9. Reduce
```c
    int sum = Vector_reduce(&vec, int (int accumulator, int value_in_vec) => { return accumulator + value_in_vec; }, 0); // Calculates the sum of elements
```
#### 6.10. Any/All
```c
    bool all = Vector_all(&vec, bool (int value_in_vec) => { return value_in_vec == 10 }); // checks to see if all values in the vector verify the callback function (the callback returns true)
    bool any = Vector_any(&vec, bool (int value_in_vec) => { return value_in_vec == 10 }); // checks to see if any of the values in the vector verify the callback function
```
#### 6.11. Slice
```c
    int *slice = Vector_slice(&vec, 0, Vector_length(vec), 2); // slices vec from index 0 to Vector_length(vec) by a step 2 and returns it
```

#### 7. Destruction
```c
    Vector_destroy(&vec); // frees the memory occupied by the vector
```