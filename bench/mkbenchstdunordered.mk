#CXXFLAGS := -O0 -g3 -Wall -Wextra
CXXFLAGS := -O2 -Wall -Wextra
.PHONY: all clean
all: bench/bench_words_std_unordered_map bench/bench_sentence_std_unordered_map
clean: 
	rm bench/bench_words_std_unordered_map bench/bench_sentence_std_unordered_map
