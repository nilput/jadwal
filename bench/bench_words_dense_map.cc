#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../strhash/superfasthash.h"
#include "../data/words.h"
#include "../util.h" //fast rand
#include "timer.h" //only works in linux

#include <cassert>
#include <functional>
#include <sparsehash/dense_hash_map>
using google::dense_hash_map;

constexpr const char *empty_key = NULL;
constexpr const char *del_key = reinterpret_cast<const char *>(1);

static bool streqcmp(const char *key_1, const char *key_2) {
    if (key_1 == key_2)
        return true;
    else if (key_1 == empty_key || key_2 == empty_key || key_1 == del_key || key_2 == del_key) //we need this, otherwise the thing segfaults
        return false;
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
    hashtable.set_empty_key(NULL);
    hashtable.set_deleted_key(reinterpret_cast<const char *>(1));

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
    for (int i=0; i<nwords; i++) {
        int idx = xorshf96() % nwords;
        const char *key = words[idx];
        maptype::iterator it = hashtable.find(key);
        assert(it != hashtable.end());
        assert(it->first == key);
        assert(it->second == idx);
    }
    printf("lookup time:    %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);
    for (int i=nwords-1; i>=0; i--) {
        const char *key = words[i];
        hashtable.erase(hashtable.find(key));
    }
    printf("deletion time:  %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
}

