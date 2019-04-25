#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../third_party/strhash/superfasthash.h"
#include "../third_party/data/words.h"
#include "util.h" //fast rand, timer, (timer only works in linux)


typedef const char * hasht_key_type; 
typedef int hasht_value_type; 

static size_t hasht_hash(hasht_key_type *key) {
    //key is passed as const char **
    return SuperFastHash(*key, strlen(*key));
}

//must return zero when equal
static int hasht_key_eq_cmp(hasht_key_type *key_1, hasht_key_type *key_2) {
    //we are passed const char **
    if (*key_1 == *key_2)
        return 0; //equal
    return strcmp(*key_1, *key_2);
}

#include "../src/hasht.h"

int main(void) {
    struct hasht ht;
    int rv = hasht_init(&ht, 0);

    struct timer_info tm_init;
    struct timer_info tm_tmp;
    timer_begin(&tm_init);
    timer_begin(&tm_tmp);

    assert(rv == HASHT_OK);
    for (int i=0; i<nwords; i++) {
        int rv = hasht_insert(&ht, &words[i], &i);
        assert(rv == HASHT_OK);
    }
    printf("insertion time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    xorshf96_srand(0xfeedbeef);
    for (int i=0; i<nwords; i++) {
        int idx = xorshf96() % nwords;
        const char *key = words[idx];
        struct hasht_iter iter;
        int rv = hasht_find(&ht, &key, &iter);
        assert(rv == HASHT_OK);
        assert(iter.pair->key == key);
        assert(iter.pair->value == idx);
    }
    printf("lookup time:    %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    for (int i=nwords-1; i>=0; i--) {
        const char *key = words[i];
        int rv = hasht_remove(&ht, &key);
        assert(rv == HASHT_OK);
    }
    printf("deletion time:  %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
    hasht_deinit(&ht);
}

