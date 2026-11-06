# C Functional Programming Library

A generic functional programming library implemented in **C**, built around reusable operations such as `for_each`, `map`, `filter`, and `reduce`.

The project explores how concepts usually associated with higher-level functional programming can be recreated in C using **function pointers**, **generic `void *` data**, **pointer arithmetic**, **variadic functions**, and explicit memory management.

The core idea is to work with arrays without depending on a specific data type. The same operations can therefore be used with integers, strings, structures, and other custom data types.

## Generic Array Representation

The project uses a custom `array_t` structure to represent generic arrays.

Each array stores:

- a pointer to the underlying memory
- the size of one element
- the number of elements
- an optional destructor function

The data itself is stored through a `void *`, which allows the array to contain values of different types.

Since the actual element type is not known by the generic functions, the implementation relies on the stored element size and pointer arithmetic to move through the array correctly.

The optional destructor makes it possible to work with more complex elements that contain dynamically allocated memory, such as strings or custom structures.

## Functional Operations

The library implements the main functional-style operations `for_each`, `map`, `filter`, and `reduce`.

`for_each` applies a callback function to every element of an array. The function receives a pointer to the current element, which makes the operation independent of the concrete type stored in the array.

`map` creates a new array by applying a transformation function to each element of the original array. The resulting array can contain elements of a completely different type and size from the input array.

For example, the test code uses `map` to transform an array of student structures into an array containing only their ages.

`filter` creates a new array containing only the elements that satisfy a predicate function. Elements that are not kept can also be cleaned up using the destructor associated with the original array.

`reduce` combines the elements of an array using an accumulator. This allows operations such as sums, aggregation, or other custom transformations to be implemented through callback functions.

## Multiple-Array Operations

The project also implements versions of the functional operations that work with multiple arrays at the same time.

These include `for_each_multiple`, `map_multiple`, and `reduce_multiple`.

The number of input arrays is not fixed in advance. Instead, the functions receive a variable number of arguments using the C variadic argument mechanism from `stdarg.h`.

The arrays are processed in parallel by index, and the iteration continues until the shortest input array is exhausted.

This allows operations such as combining several arrays into a new structure. One example in the project combines separate arrays containing integer and fractional parts into an array of custom `number_t` structures.

Another example uses several input arrays to create complete student structures from separate lists of names, ages, and grades.

## Memory Management

Memory management is an important part of the project because the generic arrays may contain both simple values and dynamically allocated objects.

The implementation uses `malloc`, `free`, and `memcpy` to create and transform arrays.

Each `array_t` can also store a destructor callback. This destructor is responsible for releasing resources owned by individual elements when needed.

This is particularly useful for arrays containing strings or structures that internally allocate memory.

Operations such as `map` and `filter` also manage the lifetime of the original arrays according to the expected ownership rules, which makes memory handling an important part of the overall design.

## Higher-Level Operations

The generic functional primitives are used to implement several higher-level tasks.

The project includes operations for reversing an array, creating structured numbers from separate integer arrays, extracting the names of students who passed a course, comparing the sums of nested integer arrays with reference values, selecting strings from even positions, and generating a square matrix.

These tasks demonstrate that the generic functions are not only isolated utilities, but can also be combined to implement more complex transformations.

For example, the passing-students operation first uses `filter` to keep only students with a grade of at least 5, then uses `map` to transform the remaining student structures into an array of names.

The project also works with nested arrays, custom structures, strings, integers, floating-point values, and dynamically allocated elements.

## Project Structure

- `functional.c` / `functional.h` — implementation and declarations of the generic functional operations
- `tasks.c` / `tasks.h` — higher-level operations built using the functional primitives
- `tests.c` / `tests.h` — test cases for the library and the task implementations
- `main.c` — selects and executes the requested test case
- `Makefile` — build configuration

## Build and Run

The project is built using **GCC** and **GNU Make**.

Running `make build` compiles the source files and creates the `functional` executable.

The compiler is configured with `-Wall`, `-Wextra`, and `-Werror`, so compiler warnings are treated as errors.

The executable reads the name of the test that should be executed and then runs the corresponding functionality.

Supported tests include the basic functional operations, the multiple-array versions, and the higher-level tasks implemented in `tasks.c`.

## Technologies and Concepts

- C
- Generic programming with `void *`
- Function pointers
- Callback functions
- Higher-order functions
- Variadic functions
- `stdarg`
- Pointer arithmetic
- Dynamic memory allocation
- Manual memory management
- Custom destructor functions
- `memcpy`
- Struct manipulation
- Nested arrays
- GCC
- GNU Make
