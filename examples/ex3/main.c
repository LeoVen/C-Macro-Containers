#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/macro_collections.h"

CONTAINER_GENERATE(LINKEDLIST, PUBLIC, ll, linked, static, , int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initliazie linked list and add some elements
    linked *ll = ll_new();
    for (int i = 0; i < 100; i++)
        ll_push_back(ll, i);

    // Initialize iterator
    linked_iter iter;
    ll_iter(&iter, ll);

    // Move cursor to the end of the list
    ll_iter_toend(&iter);
    // While there is a previous element on the list
    while (ll_iter_prev(&iter, &r, &i))
        printf("list[%d] = %d\n", i, r);

    // Move cursor to the start of the list
    ll_iter_tostart(&iter);
    // While there is a next element on the list
    while (ll_iter_next(&iter, &r, &i))
        printf("list[%d] = %d\n", i, r);

    ll_free(ll);

    return 0;
}
