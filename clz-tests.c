#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW

uint8_t clz_iter(uint32_t x)
{
	int clz(uint32_t x) {
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) { n -= c; x = y; }
        c >>= 1;
    } while (c);
    return (n - x);
}

int clz_binary_search(uint32_t x) {
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) { n += 16; x <<= 16; }
    if (x <= 0x00FFFFFF) { n += 8; x <<= 8; }
    if (x <= 0x0FFFFFFF) { n += 4; x <<= 4; }
    if (x <= 0x3FFFFFFF) { n += 2; x <<= 2; }
    if (x <= 0x7FFFFFFF) { n += 1; x <<= 1; }
    return n;
}

int clz_byte_shift(uint32_t x) {
    if (x == 0) return 32;
    int n = 1;
    if ((x >> 16) == 0) { n += 16; x <<= 16; }
    if ((x >> 24) == 0) { n += 8; x <<= 8; }
    if ((x >> 28) == 0) { n += 4; x <<= 4; }
    if ((x >> 30) == 0) { n += 2; x <<= 2; }
    n = n - (x >> 31);
    return n;
}

//main tests all functions
int main(int argc , char *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    if(argc < 2 ) return -1;

    clock_gettime(CLOCK_ID , &start);
    /*
        put clz function here
    */
    clcok_gettime(CLOCK_ID , &end);

}
