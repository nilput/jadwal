#ifndef UTILH
#define UTILH

#include <unistd.h> 
#include <time.h> 


static unsigned long xorshf96_x = 123456789,
                     xorshf96_y = 362436069,
                     xorshf96_z = 521288629;

static unsigned long xorshf96(void) {          
    //period 2^96-1
    //source: stackoverflow.com/questions/1640258
    unsigned long t;
    xorshf96_x ^= xorshf96_x << 16;
    xorshf96_x ^= xorshf96_x >> 5;
    xorshf96_x ^= xorshf96_x << 1;

    t = xorshf96_x;
    xorshf96_x = xorshf96_y;
    xorshf96_y = xorshf96_z;
    xorshf96_z = t ^ xorshf96_x ^ xorshf96_y;

    return xorshf96_z;
}
//mathematically proven to be cryptographically insecure
static void xorshf96_srand(unsigned long seed) {
    xorshf96_x = seed ^ 0xFEEDBEEF; 
    xorshf96_y = xorshf96_z = 0xBADDBEEF;
    xorshf96_y = xorshf96() ^ seed;
    xorshf96_z = xorshf96() ^ xorshf96_y;
}

struct timer_info{
    struct timespec tstart;
};
static void timer_begin(struct timer_info *timer){
    clock_gettime(CLOCK_MONOTONIC, &timer->tstart);
}
static double timer_dt(struct timer_info *timer){
    struct timespec tend; 
    clock_gettime(CLOCK_MONOTONIC, &tend);
    return ((double)tend.tv_sec          + 1.0e-9 * tend.tv_nsec) - 
           ((double)timer->tstart.tv_sec + 1.0e-9 * timer->tstart.tv_nsec);
}

#endif// UTILH
