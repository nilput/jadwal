#CXXFLAGS := -O0 -g3 -Wall -Wextra
CXXFLAGS := -O2 -Wall -Wextra -Isparsehash/src
.PHONY: all clean
all: bench_words_dense_map bench_sentence_dense_map
clean: 
	rm bench_words_dense_map bench_sentence_dense_map
