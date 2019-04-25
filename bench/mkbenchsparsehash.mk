#CXXFLAGS := -O0 -g3 -Wall -Wextra
CXXFLAGS := -O2 -Wall -Wextra
.PHONY: all clean
all: bench/bench_words_dense_map
clean: 
	rm bench/bench_words_dense_map
