# Minimal Example

(TODO this example is currently outdated)

Lets create a list of type `int`, add some elements to it and get the sum of all the elements printed on the screen.

### Include headers

```c
#include <cmc/list.h>
```

First you need to include only the necessary headers. It is not recommended that you include everything with `macro_collections.h`, only in cases where you might be using almost all features from the library.

### Generating a list

```c
// CMC_GENERATE_LIST(PFX, SNAME, V)
CMC_GENERATE_LIST(il, int_list, int)
```

The List is also known as `vector`, `ArrayList` and dynamic array. In fact the latter is the best description for this collection. It is a dynamic array that can grow and shrink as needed. To generate it we can simply call the macro `CMC_GENERATE_LIST` after our includes. It has three parameters:

| Parameter name | Purpose |
| :------------: | ------- |
| __PFX__        | Functions prefix (namespace) |
| __SNAME__      | Struct name |
| __V__          | The list value type (can be any valid data type) |

<aside class="notice">
All collections can be generated by using the macro CMC_GENERATE_ and then the name of the include (in this case list.h) but in uppercase without the `.h`.
</aside>

### Initializing and adding elements

```c
// Allocate an int_list with an initial capacity of 100
int_list *list = il_new(100);

// Adding elements to the back of the list
for (int i = 0; i <= 1000; i++)
{
    if (!il_push_back(list, i))
    {
        // Something went wrong (check out documentation)
    }
}
```

To initialize any collection we call a function `_new`. Since the `PFX` parameter is set to `il` all functions related to our list of integer will be prefixed by `il`, so `_new` becomes `il_new`.

The list name is `int_list` as defined by `SNAME`. This type is a typedef like:

`typedef struct SNAME##_s SNAME;`

Which expands to:

`typedef struct int_list_s int_list`

To add elements to the back of a list we can use `il_push_back`. This function will first check if the list has enough space for a new element. If not, reallocate the internal buffer with doubled size and then add the new element.

### Iteration

> It is recommended that the iterator is allocated on the stack

```c
// Resulting sum will be stored here
int sum = 0;

// Declare our iterator (allocating it on the stack)
int_list_iter it;

// Loop for each element
for (il_iter_init(&it, list); !il_iter_end(&it); il_iter_next(&it))
{
    // Accessing the value
    int elem = il_iter_value(&it);

    sum += elem;
}

printf("%d\n", sum);
```

There are 6 (2 optional) steps to iterate over the elements of any collection:

| Step | Description | Function |
| :--: | ----------- | -------- |
| 1    | Allocate the iterator (optional) | `il_iter_new`   |
| 2    | Initialize the iterator given a target list | `il_iter_init`  |
| 3    | Access the iterator's elements (value, index, etc) | `il_iter_value` |
| 4    | Go to the next element | `il_iter_next`  |
| 5    | Check if we haven't reached the end of the list | `il_iter_end`   |
| 6    | Free the iterator if it was allocated on the heap | `il_iter_free`  |

Note that all iterator functions are prefixed by the user defined prefix and then by `iter` (`il + iter = il_iter`). Also, the steps __1__ and __6__ are optional so `il_iter_new` and `il_iter_free` are not used in the example.

By the end of the example we have the variable `sum` with the sum of all integers inside our list. We can then print it and this example is almost done.

### Freeing resources

```c
// void il_free(int_list *_list_, void(*deallocator)(int))
il_free(list, NULL);
```

The list is currently allocated in memory. To deallocate it we call `il_free`. This function takes a pointer to the allocated list and a pointer to a `deallocator` function that will be responsible for deallocating each element in the list. This last parameter is optional and we won't be using it since our data type `int` is not allocated in the heap. This optional parameter can be set to `NULL` to be ignored by the `il_free` function.

### Compile and run

> source.c

```c
#include <cmc/list.h>

CMC_GENERATE_LIST(il, int_list, int)

int main(void)
{
    int_list *list = il_new(100);

    for (int i = 0; i <= 1000; i++)
    {
        if (!il_push_back(list, i))
        {
            // Something went wrong (check out documentation)
        }
    }

    int sum = 0;

    int_list_iter it;

    for (il_iter_init(&it, list); !il_iter_end(&it); il_iter_next(&it))
    {
        int elem = il_iter_value(&it);
        sum += elem;
    }

    printf("%d\n", sum);

    il_free(list, NULL);
}
```

If the source file is `source.c`, all you have to do is to include the `src` folder to the include path and compile. The following example uses `gcc`:

`gcc source.c -I path/to/library/src`

### Conclusion

The C Macro Collections Library is prepared to accept any data type and is highly customizable. It is also very easy to integrate and no previous compilation is required. The library also comes with many utilities in the `utl` folder that are designed to work well with all collections.