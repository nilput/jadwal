CFLAGS :=  -Wall -Wextra -Wno-unused-function
all: targets 

targets: run_tests

TESTS :=  hasht_test_O0 hasht_test_O2 hasht_test_O3 hasht_test_O2_NDEBUG
run_tests: $(TESTS)
	for prg in $^; do \
		./"$$prg" || exit 1; \
	done

hasht_test_O0: CFLAGS += -O0 -g3 -fsanitize=address,undefined -DHASHT_DBG
hasht_test_O2: CFLAGS += -O2 -DHASHT_DBG
hasht_test_O2_NDEBUG: CFLAGS += -O2 #no assertions
hasht_test_O3: CFLAGS += -O3 -DHASHT_DBG 

%_O0 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2_NDEBUG : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%_O3 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f hasht_test_O0 hasht_test_O2 hasht_test_O3 hasht_test_O2_NDEBUG
