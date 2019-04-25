/*
 * in this bench we do the following:
 *  for each word1 in the huge word list:
 *      generate a random sentence formed by: word_1 as a prefix, and concatenate two random words
 *      word2, word3, seperated by white space
 *  for each word_1 in the huge word list:
 *      find the sentence of the word:
 *          if it exists:
 *              delete the entries of word2 and word3
 *  then for each entry in the hashtable (where the lookup order is defined by the words list):
 *      print the result to the file output_sentence<BENCH POSTFIX>.txt
 *      deallocate the memory of the result
    deallocate the table
 */

#define OUTPUT_FNAME "bench_sentence.txt"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../third_party/strhash/superfasthash.h"
#include "../third_party/data/words.h"
#include "util.h" //fast rand, timer


typedef const char * hasht_key_type; 
typedef char * hasht_value_type; 

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

void assertx(int cond) {
    if (!cond) {
        abort();
    }
}

void *xmalloc(size_t sz) {
    void *m = malloc(sz);
    assertx(!!m);
    return m;
}

void sep_word(const char *sentence, const char **word2) {
    const char *space = strchr(sentence, ' ');
    if (space)
        *word2 = space + 1;
    else
        *word2 = NULL;
}

int main(void) {
    struct hasht ht;
    int rv = hasht_init(&ht, 0);
    assert(rv == HASHT_OK);

    struct timer_info tm_init;
    struct timer_info tm_tmp;
    timer_begin(&tm_init);
    timer_begin(&tm_tmp);

    xorshf96_srand(0xfafafaf);

    for (int i=0; i<nwords; i++) {
        //pick two random words
        int words_idx[3] = {i, xorshf96() % nwords, xorshf96() % nwords};
        while (words_idx[0] == words_idx[1] || words_idx[1] == words_idx[2] || words_idx[0] == words_idx[2]) {
            words_idx[1] = xorshf96() % nwords;
            words_idx[2] = xorshf96() % nwords;
        }
        const char *word = words[words_idx[0]];
        int sentence_len = 0;
        int sentence_cur = 0;
        for (int i=0; i<3; i++) 
            sentence_len += strlen(words[words_idx[i]]) + 1;
        char *sentence = xmalloc(sentence_len);
        for (int i=0; i<3; i++) {
            const char *word = words[words_idx[i]];
            memcpy(sentence + sentence_cur, word, strlen(word));
            sentence_cur += strlen(word);
            if (i < 2)
                sentence[sentence_cur] = ' ';
            else
                sentence[sentence_cur] = '\0';
            sentence_cur++;
        }
        assert((int)strlen(sentence) == sentence_len - 1);
        int rv = hasht_insert(&ht, &word, &sentence);
        assert(rv == HASHT_OK);
    }
    printf("insertion time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);

    for (int i=0; i<nwords; i++) {
        struct hasht_iter iter;
        const char *word = words[i];
        int rv = hasht_find(&ht, &word, &iter);
        if (rv != HASHT_OK) {
            assert(rv == HASHT_NOT_FOUND);
            continue;
        }
        assert(iter.pair);
        assert(iter.pair->value);
        const char *sentence = iter.pair->value;
        const char *next_word = sentence;
        for (int j=0; j<2; j++) {
            sep_word(next_word, &next_word);
            assert(next_word);
            struct hasht_iter it;
            int rv = hasht_find(&ht, &next_word, &it);
            if (rv != HASHT_OK) {
                assert(rv == HASHT_NOT_FOUND);
                continue;
            }
            assert(it.pair->value);
            free(it.pair->value);
            rv = hasht_remove(&ht, &next_word);
            assert(rv == HASHT_OK);
        }
    }
    printf("filtering time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);

    FILE *fout = fopen(OUTPUT_FNAME, "w");
    assert(fout);
    
    for (int i=0; i<nwords; i++) {
        struct hasht_iter iter;
        const char *word = words[i];
        int rv = hasht_find(&ht, &word, &iter);
        if (rv != HASHT_OK) {
            assert(rv == HASHT_NOT_FOUND);
            continue;
        }
        assert(iter.pair);
        assert(iter.pair->value);
        char *sentence = iter.pair->value;
        fprintf(fout, "%s\n", sentence);
        free(sentence);
    }


    fclose(fout);
    fout = NULL;
    printf("output time: %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
    hasht_deinit(&ht);
}
