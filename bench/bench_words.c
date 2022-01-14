#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../third_party/strhash/superfasthash.h"
#include "../third_party/data/words.h"
#include "util.h" //fast rand, timer


typedef const char * jadwal_key_type; 
typedef int jadwal_value_type; 

static size_t jadwal_hash(jadwal_key_type *key) {
    //key is passed as const char **
    return SuperFastHash(*key, strlen(*key));
}

//must return zero when equal
static int jadwal_key_eq_cmp(jadwal_key_type *key_1, jadwal_key_type *key_2) {
    //we are passed const char **
    if (*key_1 == *key_2)
        return 0; //equal
    return strcmp(*key_1, *key_2);
}

#include "../src/jadwal.h"

int main(void) {
    struct jadwal ht;
    int rv = jadwal_init(&ht, 0);
    assert(rv == JADWAL_OK);

    struct timer_info tm_init;
    struct timer_info tm_tmp;
    timer_begin(&tm_init);
    timer_begin(&tm_tmp);

    for (int i=0; i<nwords; i++) {
        int rv = jadwal_insert(&ht, &words[i], &i);
        assert(rv == JADWAL_OK);
    }
    printf("insertion time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    xorshf96_srand(0xfeedbeef);

    char keybuff[256];
    size_t keybuff_sz = 256;
    const char *keycpy = keybuff;

    for (int i=0; i<nwords; i++) {
        int idx = xorshf96() % nwords;
        const char *key = words[idx];
        assert(strlen(key) < keybuff_sz);
        strcpy(keybuff, key);
        struct jadwal_iter iter;
        int rv = jadwal_find(&ht, &keycpy, &iter);
        assert(rv == JADWAL_OK);
        assert(iter.pair->key == key);
        assert(iter.pair->value == idx);
    }
    printf("lookup time:    %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    for (int i=nwords-1; i>=0; i--) {
        const char *key = words[i];
        assert(strlen(key) < keybuff_sz);
        strcpy(keybuff, key);

        int rv = jadwal_remove(&ht, &keycpy);
        assert(rv == JADWAL_OK);
    }
    printf("deletion time:  %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
    jadwal_deinit(&ht);
}

