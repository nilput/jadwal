CFLAGS :=  -Wall -Wextra -Wno-unused-function
all: targets 

targets: bench

bench: bench_words_O2_NDEBUG
bench_words_O0:  CFLAGS += -O0 -g3 -fsanitize=address,undefined -DHASHT_DBG
bench_words_O2:  CFLAGS += -O2 -DHASHT_DBG
bench_words_O2_NDEBUG: CFLAGS += -O2 #no assertions (other than the ones in bench_words.c)


%_O0 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2_NDEBUG : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%_O3 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f bench_words_O0 bench_words_O2 bench_words_O2_NDEBUG
