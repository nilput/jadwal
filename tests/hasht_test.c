//must define this in build system, otherwise the tests are useless #define HASHT_DBG

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
typedef int hasht_key_type; 
typedef int hasht_value_type; 

#ifdef HASHT_DATA_ARG
int mydata[] = {213123,2313123,664536,3423424,31231231};
void assert_udata_is_ok(void *udata) {
    for (int i=0;  i < (int)(sizeof mydata / sizeof mydata[0]); i++) {
        int *data = (int *) udata;
        assert(data[i] == mydata[i]);
    }
}
size_t hasht_hash(void *udata, hasht_key_type *key) {
    assert_udata_is_ok(udata);
    return *key;
}

bool hasht_key_eq_cmp(void *udata, hasht_key_type *key_1, hasht_key_type *key_2) {
    assert_udata_is_ok(udata);
    return *key_1 == *key_2 ? 0 : 1;
}
#else
size_t hasht_hash(hasht_key_type *key) {
    return *key;
}

bool hasht_key_eq_cmp(hasht_key_type *key_1, hasht_key_type *key_2) {
    return *key_1 == *key_2 ? 0 : 1;
}
#endif
#include "../src/hasht.h"

/*
vals=set()
import random
for i in range(100):
    val1 = random.randint(0, 10000)
    val2 = random.randint(0, 10000)
    while val1 in vals or val2 in vals or val1 == val2:
        val1 = random.randint(0, 10000)
        val2 = random.randint(0, 10000)
    vals.add(val1)
    vals.add(val2)
    print('{{ {}, {} }},'.format(val1, val2))
 * */



int values[][2] = {
{ 4935, 6475 },
{ 3728, 4317 },
{ 9374, 7993 },
{ 578, 9272 },
{ 4224, 4307 },
{ 4174, 6495 },
{ 2550, 9567 },
{ 2925, 4036 },
{ 7641, 4143 },
{ 2278, 4368 },
{ 3615, 5123 },
{ 3040, 501 },
{ 5511, 6136 },
{ 5194, 5837 },
{ 5489, 1208 },
{ 9341, 624 },
{ 9553, 6216 },
{ 362, 8564 },
{ 5010, 9802 },
{ 6154, 1388 },
{ 1418, 4110 },
{ 2928, 4017 },
{ 3479, 9837 },
{ 2396, 3897 },
{ 7783, 3251 },
{ 1119, 1722 },
{ 4655, 3578 },
{ 4961, 8021 },
{ 1056, 9399 },
{ 7067, 6195 },
{ 2429, 9929 },
{ 8004, 4830 },
{ 4641, 2556 },
{ 86, 7541 },
{ 5740, 7190 },
{ 9785, 7192 },
{ 4574, 7667 },
{ 8229, 5369 },
{ 3904, 5655 },
{ 3800, 3294 },
{ 4835, 6639 },
{ 6041, 3953 },
{ 6077, 1996 },
{ 8067, 6193 },
{ 7540, 731 },
{ 4055, 5743 },
{ 245, 1934 },
{ 1517, 8723 },
{ 929, 3503 },
{ 1165, 5190 },
{ 2736, 492 },
{ 7520, 3516 },
{ 7679, 6835 },
{ 8408, 2430 },
{ 2409, 4865 },
{ 7457, 264 },
{ 8464, 9897 },
{ 868, 5360 },
{ 5134, 4698 },
{ 739, 1292 },
{ 2037, 4154 },
{ 461, 8374 },
{ 253, 1665 },
{ 7567, 3936 },
{ 9284, 7204 },
{ 8221, 6232 },
{ 5426, 7648 },
{ 8927, 1772 },
{ 507, 207 },
{ 9963, 8602 },
{ 4132, 6587 },
{ 9586, 8234 },
{ 6428, 2061 },
{ 6022, 8517 },
{ 3048, 5806 },
{ 1298, 6108 },
{ 8597, 9241 },
{ 1708, 2878 },
{ 1546, 2816 },
{ 5879, 2427 },
{ 2834, 1948 },
{ 4350, 2522 },
{ 8260, 8415 },
{ 9423, 7003 },
{ 6858, 986 },
{ 6308, 9124 },
{ 6671, 659 },
{ 7535, 4650 },
{ 6706, 6670 },
{ 136, 1980 },
{ 6035, 9185 },
{ 2706, 8020 },
};
int values2[][2] = {
{ 5179, 8035 },
{ 4989, 6124 },
{ 8425, 5061 },
{ 6795, 3187 },
{ 9661, 7833 },
{ 435, 2382 },
{ 3606, 5321 },
{ 9585, 353 },
};

void *xmalloc(size_t sz) {
    void *m = malloc(sz);
    if (!m)
        assert(0);
    return m;
}
void test_insert_all_arr2(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        int rv = hasht_insert(ht, &arr[i][0], &arr[i][1]);
        assert(rv == HASHT_OK);
    }
}
void test_insert_all_arr2_expect_duplicate(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        int rv = hasht_insert(ht, &arr[i][0], &arr[i][1]);
        assert(rv == HASHT_DUPLICATE_KEY);
    }
}
void test_delete_all_arr2(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        int rv = hasht_remove(ht, &arr[i][0]);
        assert(rv == HASHT_OK);
    }
}
void test_delete_all_arr2_expect_not_found(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        int rv = hasht_remove(ht, &arr[i][0]);
        assert(rv == HASHT_NOT_FOUND);
    }
}
void test_find_all_arr2(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        struct hasht_iter iter;
        int rv = hasht_find(ht, &arr[i][0], &iter);
        assert(rv == HASHT_OK);
        assert(iter.pair);
        assert(iter.pair->key == arr[i][0]);
        assert(iter.pair->value == arr[i][1]);
    }
}

long linear_find(int arr[][2], int nelems, long key) {
    for (int i=0; i<nelems; i++) {
        if (arr[i][0] == key)
            return i;
    }
    return -1;
}
long xlinear_find(int arr[][2], int nelems, long key) {
    long idx = linear_find(arr, nelems, key);
    assert(idx >= 0 && idx < nelems);
    return idx;
}

void test_iter_expect_seen(struct hasht *ht, int arr[][2], int nelems, long expected_len) {
    bool *seen = xmalloc(sizeof(bool) * nelems);
    memset(seen, 0, sizeof(bool) * nelems);
    struct hasht_iter iter;
    hasht_begin_iterator(ht, &iter);
    long len = 0;
    for (; hasht_iter_check(&iter); hasht_iter_next(ht, &iter)) {
        assert(iter.pair);
        bool *seen_entry = seen + xlinear_find(arr, nelems, iter.pair->key);
        assert(!*seen_entry);
        *seen_entry = 1;
        len++;
    }
    assert(len == expected_len);
    //redundant check?
    long seen_count = 0;
    for (int i=0; i<nelems; i++) {
        if (seen[i])
            seen_count++;
    }
    assert(seen_count == expected_len);
    free(seen);
}

void test_iter_expect_count(struct hasht *ht, long expected_len) {
    struct hasht_iter iter;
    hasht_begin_iterator(ht, &iter);
    long len = 0;
    for (; hasht_iter_check(&iter); hasht_iter_next(ht, &iter)) {
        assert(iter.pair);
        len++;
    }
    assert(len == expected_len);
}

void test_find_all_arr2_expect_not_found(struct hasht *ht, int arr[][2], int nelems) {
    for (int i=0; i<nelems; i++) {
        struct hasht_iter iter;
        int rv = hasht_find(ht, &arr[i][0], &iter);
        assert(rv == HASHT_NOT_FOUND);
    }
}
void test_init_add_arrays_find(void) {
    struct hasht ht;
    int rv = hasht_init(&ht, 0);
    assert(rv == HASHT_OK);

#ifdef HASHT_DATA_ARG
    ht.userdata = mydata;
#endif

    int arr1_sz = sizeof values / sizeof values[0];
    int arr2_sz = sizeof values2 / sizeof values2[0];
    test_insert_all_arr2(&ht, values, arr1_sz);
    test_find_all_arr2(&ht, values, arr1_sz);
    test_delete_all_arr2(&ht, values, arr1_sz);
    test_insert_all_arr2(&ht, values, arr1_sz);
    test_iter_expect_seen(&ht, values, arr1_sz, arr1_sz);
    test_iter_expect_count(&ht, arr1_sz);
    test_find_all_arr2(&ht, values, arr1_sz);
    test_delete_all_arr2(&ht, values, arr1_sz);
    test_delete_all_arr2_expect_not_found(&ht, values, arr1_sz);
    test_insert_all_arr2(&ht, values, arr1_sz);
    test_insert_all_arr2(&ht, values2, arr2_sz);
    test_iter_expect_count(&ht, arr1_sz + arr2_sz);
    test_find_all_arr2(&ht, values, arr1_sz);
    test_find_all_arr2(&ht, values2, arr2_sz);
    test_delete_all_arr2(&ht, values, arr1_sz);
    test_delete_all_arr2_expect_not_found(&ht, values, arr1_sz);
    test_find_all_arr2_expect_not_found(&ht, values, arr1_sz);
    test_find_all_arr2(&ht, values2, arr2_sz);
    test_insert_all_arr2(&ht, values, arr1_sz);
    test_find_all_arr2(&ht, values, arr1_sz);

    test_find_all_arr2(&ht, values, arr1_sz);
    hasht_deinit(&ht);
}

int main(void) {
    test_init_add_arrays_find();
    printf("success\n");
}
