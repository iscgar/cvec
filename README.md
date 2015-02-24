# cvec #
A generic, type-safe `vector` implementation in ANSI C.

## Installation ##
Just drop the [cvec.c](cvec.c?raw=1) and [cvec.h](cvec.h?raw=1) files anywhere in your project
and compile them with it.

## Usage ##
To be able to use a vector of any type you need to declare it first. To do so, include the `cvec.h`
file and add a declaration in the following form outside of any function before the first use.
```c
/* `T` is the type to be stored in the vector.
 * Note the lack of a semicolon */
using_vec_type(T)
```

Any type can be used in the vector declaration, but due to the way things are handled internally
there's a certain limitation on the type's name: it should be a valid C identifier. So if -- for
example -- you wanted a vector of pointers to `unsigned int`, you would just `typedef` it as follows:
```c
typedef unsigned int* uint_ptr;

/* Now we can have a vector of pointers to unsigned int */
using_vec_type(uint_ptr)
```

A vector should be first initialized using the `vec_init()` function.
```c
vec_type(int) v;
vec_init(int)(&v);
vec_push(int)(&v, 42);
vec_push(int)(&v, 666);
```

To access elements in the vector the `vec_first()`, `vec_last()` and `vec_get()` functions can be used
```c
printf("The first element is %d, The last is %d, and the one at index 1 is %d\n",
       *vec_first(int)(&v),
       *vec_last(int)(&v),
       *vec_get(int)(&v, 1));
```

Assignment can be done implicitly using the pointer returned by the above functions, or explicitly
by calling the `vec_assign()` or `vec_assignptr()` functions
```c
/* Assigning a value */
*vec_first(int)(&v) = 162;
vec_assign(int)(&v, 1, 53);

/* Or using a pointer (useful for struct assignments) */
int a = 5;
vec_assignptr(int)(&v, vec_size(int)(&v) - 1, &a);
```

You can get the current size of the vector using the `vec_size()` function, or simply call the
`vec_empty()` function if you just want to check if the vector is empty.
```c
vec_type(int) *v_dynamic = (vec_type(int) *)malloc(sizeof(vec_type(int)));
vec_init(int)(v_dynamic);

if (vec_empty(int)(v_dynamic)) { /* true */
    vec_push(int)(v_dynamic, 123);

    if (vec_size(int)(v_dynamic) == 1) { /* true */
        vec_pop(int)(v_dynamic, NULL);
    }
}
```

When there's no more use for the vector, the `vec_clear()` function must be called on it to free
the dynamic memory allocated by the vector.
```c
int i;
vec_type(int) v;

vec_init(int)(&v);

for (i = 0; i < 10; ++i)
{
    vec_push(int)(&v, i + 1);
}

vec_clear(int)(&v);
```

## Functions
All vector functions are called through the macros defined int `cvec.h` and must be supplied with an
additional type parameter in parenthesis before the arguments list.

### `vec_type(T)`
A macro to get the type name of the vector.
```c
typedef FILE* fileptr;
using_vec_type(fileptr)

void foo() {
    vec_type(fileptr) filev;

    ...
}
```

### `vec\_init(T)(vec_ptr)`
Initializes a vector of type `T`. This function must be called before any other operation on the vector
or otherwise the other functions will either fail or your application will segfault. Returns FALSE if
passed a NULL pointer.

### `vec\_clear(T)(vec_ptr)`
Clears the elements in the vector and frees the memory allocated for them. To prevent memory leaks this
function must be called when there's no more use for the vector.

### `vec\_size(T)(vec_ptr)`
Return the size of the vector. 0 if empty or when passed an invalid vector pointer.

### `vec\_empty(T)(vec_ptr)`
Returns TRUE when the vector is empty or when passed an invalid vector pointer. FALSE otherwise.

### `vec\_capacity(T)(vec_ptr)`
Returns the current capacity of the vector. 0 if no memory is currently allocated or when passed an invalid
vector pointer.

### `vec\_reserve(T)(vec_ptr, n)`
Requests that the vector capacity be at least enough to contain `n` elements. If `n` is greater than the
current capacity, the function causes reallocation. Otherwise it does nothing. Returns TRUE if the reservation
succeeded. FALSE otherwise.
```c
vec_type(int) v;
vec_init(int)(&v);
uint capacity = vec_capacity(int)(&v); /* 0 */
if (vec_reserve(int)(&v, 25)) {
    capacity = vec_capacity(int)(&v); /* 25 */
}
```

### `vec_push(T)(vec_ptr, v)`
Pushes a value `v` to the end of the vector. Return TRUE if `vec_ptr` points to a valid vector structure and push
succeeded. FALSE otherwise.

### `vec_pushptr(T)(vec_ptr, v_ptr)`
Pushes a value passed by the reference `v_ptr` to the end of the vector. Return TRUE if `vec_ptr` points to a valid
vector structure, `v_ptr` is not NULL and push succeeded. FALSE otherwise.

### `vec_shift(T)(vec_ptr, v)`
Shifts a value `v` to the beginning of the vector. Return TRUE if `vec_ptr` points to a valid vector structure and
shift succeeded. FALSE otherwise.

### `vec_shiftptr(T)(vec_ptr, v_ptr)`
Shifts a value passed by the reference `v_ptr` to the beginning of the vector. Return TRUE if `vec_ptr` points to
a valid vector structure, `v_ptr` is not NULL and shift succeeded. FALSE otherwise.

### `vec_insert(T)(vec_ptr, v, idx)`
Inserts a value `v` at the specified `idx` in the vector. Returns TRUE if `vec_ptr` points to a valid vector structure,
`idx` is not greater than the vector's size and insertion suceeded. FALSE otherwise.

### `vec_insertptr(T)(vec_ptr, v_ptr, idx)`
Inserts a value passed by the reference `v_ptr` at the specified `idx` in the vector. Returns TRUE if `vec_ptr` points
to a valid vector structure, `v_ptr` is not NULL, `idx` is not greater than the vector's size and insertion suceeded.
FALSE otherwise.

### `vec_get(T)(vec_ptr, idx)`
Returns a pointer to the element at `idx`. NULL if `vec_ptr` points to an invalid vector structure or `idx` is outside
the vector's bounds. Note that this pointer is invalid after a call to any of `vec_reserve()`, `vec_push()`, `vec_pushptr()`,
`vec_shift()`, `vec_shiftptr()`, `vec_insert()`, `vec_insertptr()`, `vec_pop()`, `vec_unshift()`, `vec_erase()` and `vec_clear()`.

### `vec_first(T)(vec_ptr)`
Returns a pointer to the first element in the vector. NULL if `vec_ptr` points to an invalid vector structure or if it's empty.
Note that this pointer is invalid after a call to any of `vec_reserve()`, `vec_push()`, `vec_pushptr()`, `vec_shift()`,
`vec_shiftptr()`, `vec_insert()`, `vec_insertptr()`, `vec_pop()`, `vec_unshift()`, `vec_erase()` and `vec_clear()`.

### `vec_last(T)(vec_ptr)`
Returns a pointer to the last element in the vector. NULL if `vec_ptr` points to an invalid vector structure or if it's empty.
Note that this pointer is invalid after a call to any of `vec_reserve()`, `vec_push()`, `vec_pushptr()`, `vec_shift()`,
`vec_shiftptr()`, `vec_insert()`, `vec_insertptr()`, `vec_pop()`, `vec_unshift()`, `vec_erase()` and `vec_clear()`.

### `vec_assign(T)(vec_ptr, idx, v)`
Assigns a value 'v' at 'idx'. If `idx` equls the size of the vector, the behavior is the same as `vec_push()`. Returns TRUE
if `vec_ptr` points to a valid vector structure and `idx` is not greater than the vector's size. FALSE otherwise.

### `vec_assignptr(T)(vec_ptr, idx, v_ptr)`
Assigns a value passed by the reference 'v_ptr' at 'idx'. If `idx` equls the size of the vector, the behavior is the same as
`vec_pushptr()`. Returns TRUE if `vec_ptr` points to a valid vector structure, `idx` is not greater than the vector's size and
`v_ptr` is not NULL. FALSE otherwise.

### `vec_pop(T)(vec_ptr, v_ptr)`
Removes an element from the end of the vector and stores it in `v_ptr` if it's not NULL. Returns TRUE if `vec_ptr` point to a
valid vector structure that is not empty. FALSE otherwise.

### `vec_unshift(T)(vec_ptr, v_ptr)`
Removes an element from the beginning of the vector and stores it in `v_ptr` if it's not NULL. Returns TRUE if `vec_ptr` point
to a valid vector structure that is not empty. FALSE otherwise.

### `vec_erase(T)(vec_ptr, idx, v_ptr)`
Removes the element at 'idx' from the vector and stores it in `v_ptr` if it's not NULL. Returns TRUE if `vec_ptr` point to a
valid vector structure that is not empty and 'idx' is inside the vector's bounds. FALSE otherwise.

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
