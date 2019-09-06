#include "utl/assert.h"
#include "utl/test.h"
#include "../src/treeset.c"

#include "utl.c"

CMC_CREATE_UNIT(treeset_test, true, {
    CMC_CREATE_TEST(new, {
        treeset *set = ts_new(cmp);

        cmc_assert_not_equals(ptr, NULL, set);

        ts_free(set);
    });

    CMC_CREATE_TEST(clear[count], {
        treeset *set = ts_new(cmp);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            ts_insert(set, i);

        cmc_assert_equals(size_t, 50, ts_count(set));

        ts_clear(set);

        cmc_assert_equals(size_t, 0, ts_count(set));
        cmc_assert_equals(ptr, NULL, set->root);

        ts_free(set);
    });

});
