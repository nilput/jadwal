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

#define OUTPUT_FNAME "bench_sentence_std_unordered_map.txt"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../third_party/strhash/superfasthash.h"
#include "../third_party/data/words.h"
#include "util.h" //fast rand, timer

#include <cassert>
#include <functional>
#include <unordered_map>

static bool streqcmp(const char *key_1, const char *key_2) {
    if (key_1 == key_2)
        return true;
    return strcmp(key_1, key_2) == 0;
}

static uint32_t hashwrap(const char *s) {
    return SuperFastHash(s, strlen(s));
}

void *xmalloc(size_t sz) {
    void *m = malloc(sz);
    assert(!!m);
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
    typedef std::unordered_map<const char *,
                        char *,
                        std::function<uint32_t(const char *)>,
                        std::function<bool (const char *, const char *)> 
                        > maptype;
    maptype hashtable(0, hashwrap, streqcmp);

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
        char *sentence = (char *)xmalloc(sentence_len);
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
        std::pair<maptype::iterator, bool> it = hashtable.insert(std::make_pair(word, sentence));
        assert(it.second);
    }
    printf("insertion time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);

    for (int i=0; i<nwords; i++) {

        const char *word = words[i];
        maptype::iterator it = hashtable.find(word);
        if (it == hashtable.end())
            continue;

        assert(it->second);
        const char *sentence = it->second;
        const char *next_word = sentence;
        for (int j=0; j<2; j++) {
            sep_word(next_word, &next_word);
            assert(next_word);
            maptype::iterator iter = hashtable.find(next_word);
            if (iter == hashtable.end())
                continue;
            assert(iter->second);
            free((void *)iter->second);
            hashtable.erase(iter);
        }
    }
    printf("filtering time: %f\n", timer_dt(&tm_tmp));
    timer_begin(&tm_tmp);

    FILE *fout = fopen(OUTPUT_FNAME, "w");
    assert(fout);

    for (int i=0; i<nwords; i++) {
        const char *word = words[i];
        maptype::iterator it = hashtable.find(word);
        if (it == hashtable.end())
            continue;

        assert(it->second);
        char *sentence = it->second;
        fprintf(fout, "%s\n", sentence);
        free(sentence);
    }

    fclose(fout);
    fout = NULL;
    printf("output time: %f\n", timer_dt(&tm_tmp));
    printf("total time:     %f\n", timer_dt(&tm_init));
    printf("success\n");
}

