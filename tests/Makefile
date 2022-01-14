CFLAGS :=  -Wall -Wextra -Wno-unused-function
all: targets 

targets: run_tests

TESTS :=  jadwal_test_O0 jadwal_test_O2 jadwal_test_O3 jadwal_test_O2_NDEBUG jadwal_test_udata_O0
run_tests: $(TESTS)
	for prg in $^; do \
		./"$$prg" || exit 1; \
	done

jadwal_test_O0: CFLAGS += -O0 -g3 -fsanitize=address,undefined -DJADWAL_DBG
jadwal_test_O2: CFLAGS += -O2 -DJADWAL_DBG
jadwal_test_O2_NDEBUG: CFLAGS += -O2 #no assertions
jadwal_test_O3: CFLAGS += -O3 -DJADWAL_DBG 
jadwal_test_udata_O0: CFLAGS += -O0 -g3 -fsanitize=address,undefined -DJADWAL_DBG -DJADWAL_DATA_ARG

%_O0 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O2_NDEBUG : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
%_O3: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%_udata_O0 : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TESTS)
