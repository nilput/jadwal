CFLAGS :=  -Wall -Wextra -Wno-unused-function
all: targets 

targets: bench

#to cause bench_words_O0 to be built for example, add bench_words_O0 to bench: ...
bench: bench_words_O2_NDEBUG bench_sentence_O2_NDEBUG
O0 := -O0 -g3 -fsanitize=address,undefined -DJADWAL_DBG
O2 :=  -O2 -DJADWAL_DBG
O2_NDEBUG := -O2 #no assertions (other than the ones in bench_words.c)

%_O0 : %.c
	$(CC) $(O0) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2 : %.c
	$(CC) $(O2) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2_NDEBUG : %.c
	$(CC) $(O2_NDEBUG) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f bench_words_O0 bench_words_O2 bench_words_O2_NDEBUG bench_sentence_O0 bench_sentence_O2 bench_sentence_O2_NDEBUG
