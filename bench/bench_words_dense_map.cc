#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../third_party/strhash/superfasthash.h"
#include "../third_party/data/words.h"
#include "util.h" //fast rand, timer

#include <cassert>
#include <functional>
#include <sparsehash/dense_hash_map>
using google::dense_hash_map;

#define SP_EMPTY_KEY (NULL)
#define SP_DEL_KEY (reinterpret_cast<const char *>(1))

static bool streqcmp(const char *key_1, const char *key_2) {
    if (key_1 == key_2) {
        return true;
    }
    else if (key_1 == SP_EMPTY_KEY || key_2 == SP_EMPTY_KEY || key_1 == SP_DEL_KEY || key_2 == SP_DEL_KEY) {
        //we need this, otherwise the thing segfaults
        return false;
    }
    return strcmp(key_1, key_2) == 0;
}

static uint32_t hashwrap(const char *s) {
    return SuperFastHash(s, strlen(s));
}

int main(void) {
    typedef dense_hash_map<const char *,
                        int,
                        std::function<uint32_t(const char *)>,
                        std::function<bool (const char *, const char *)> 
                        > maptype;
    maptype hashtable(0, hashwrap, streqcmp);
    hashtable.set_empty_key(SP_EMPTY_KEY);
    hashtable.set_deleted_key(SP_DEL_KEY);

    struct timer_info tm_init;
    struct timer_info tm_tmp;
    timer_begin(&tm_init);
    timer_begin(&tm_tmp);

    for (int i=0; i<nwords; i++) {
        hashtable.insert(std::make_pair(words[i], i));
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
        maptype::iterator it = hashtable.find(keycpy);
        assert(it != hashtable.end());
        assert(it->first == key);
        assert(it->second == idx);
    }
    printf("lookup time:    %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    for (int i=nwords-1; i>=0; i--) {
        const char *key = words[i];
        assert(strlen(key) < keybuff_sz);
        strcpy(keybuff, key);
        hashtable.erase(hashtable.find(keycpy));
    }
    printf("deletion time:  %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
}

