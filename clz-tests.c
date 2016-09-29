#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define CLOCK_ID CLOCK_MONOTONIC_RAW
#define ONE_SEC 1000000000.0
#define FILE_PATH "time.csv"

uint32_t count = 16;
uint32_t mask  = 0xFFFFFFFF;

double get_avgofCI(double* datas ,double loop_size)
{
    double ci_max = 0.0;
    double ci_min = 0.0;
    double sd = 0.0;
    double avg = 0.0;
    double output = 0.0;
    for(int i=0; i < loop_size ; i++) {
        avg += datas[i];
    }
    avg = avg / loop_size;

    for(int i=0; i<loop_size ; i++) {
        const double tmp = datas[i] - avg;
        sd += (tmp * tmp);
    }
    sd = sqrt(sd / loop_size);

    ci_max = avg + (1.96 * sd );
    ci_min = avg - (1.96 * sd );

    int valid=0;
    for(int i = 0 ; i < loop_size ; i++) {
        if(datas[i] >= ci_min && datas[i] <= ci_max) {
            valid++;
            output += datas[i];
        }
    }

    output = output / (double)valid;

    return output;
}

int clz_iter(uint32_t x)
{
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) {
            n -= c;
            x = y;
        }
        c >>= 1;
    } while (c);
    return (n - x);
}

int clz_binary_search(uint32_t x)
{
    if (x == 0) return 32;
    int n = 0;
    if (x <= 0x0000FFFF) {
        n += 16;
        x <<= 16;
    }
    if (x <= 0x00FFFFFF) {
        n += 8;
        x <<= 8;
    }
    if (x <= 0x0FFFFFFF) {
        n += 4;
        x <<= 4;
    }
    if (x <= 0x3FFFFFFF) {
        n += 2;
        x <<= 2;
    }
    if (x <= 0x7FFFFFFF) {
        n += 1;
        x <<= 1;
    }
    return n;
}

int clz_byte_shift(uint32_t x)
{
    if (x == 0) return 32;
    int n = 1;
    if ((x >> 16) == 0) {
        n += 16;
        x <<= 16;
    }
    if ((x >> 24) == 0) {
        n += 8;
        x <<= 8;
    }
    if ((x >> 28) == 0) {
        n += 4;
        x <<= 4;
    }
    if ((x >> 30) == 0) {
        n += 2;
        x <<= 2;
    }
    n = n - (x >> 31);
    return n;
}

int clz_recursive(uint32_t x)
{
    int result;
    // shift upper half down, rest is filled up with 0s
    uint16_t upper = (x >> count);
    // mask upper half away
    mask >>= count;
    uint16_t lower = (x & mask);
    // stopping condition
    if(count == 1) {
        return !(x >> 1);
    }
    // shifting count and go into recursive
    count >>= 1;
    result = upper ? clz_recursive(upper) : (count << 1) + clz_recursive(lower);
    count <<= 1;
    return result;
}

int clz_tail_recursive(uint32_t x , int final_result)
{
    if(!x)
        return final_result;
    return clz_tail_recursive( (x >> 1) , final_result-1 );
}

//main tests all functions
int main(int argc , char *argv[])
{
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    assert(argc >= 2 && "Not enough arguments");

    uint32_t input = (uint32_t)atoi(argv[1]);

    FILE* fout = fopen(FILE_PATH ,"a+");
    assert(fout && "FILE OPEN FAIL");

    fprintf(fout , "%u " , input);

    const int loops = 10;

    double datas[10];
    //iteration
    for(int i =0; i<loops ; i++) {
        clock_gettime(CLOCK_ID , &start);
        clz_iter(input);
        clock_gettime(CLOCK_ID , &end);
        datas[i] = (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fout , "%.12lf ", get_avgofCI(datas , loops));

    //binary search
    for(int i =0; i<loops ; i++) {
        clock_gettime(CLOCK_ID , &start);
        clz_binary_search(input);
        clock_gettime(CLOCK_ID , &end);
        datas[i] = (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fout , "%.12lf ", get_avgofCI(datas , loops));

    //byte-shift
    for(int i =0; i<loops ; i++) {
        clock_gettime(CLOCK_ID , &start);
        clz_byte_shift(input);
        clock_gettime(CLOCK_ID , &end);
        datas[i] = (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fout , "%.12lf ", get_avgofCI(datas , loops));

    //recursive
    for(int i =0; i<loops ; i++) {
        clock_gettime(CLOCK_ID , &start);
        clz_recursive(input);
        clock_gettime(CLOCK_ID , &end);
        datas[i] = (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fout , "%.12lf ", get_avgofCI(datas , loops));

    //tail recursion
    for(int i =0; i<loops ; i++) {
        clock_gettime(CLOCK_ID , &start);
        clz_tail_recursive(input , 32);
        clock_gettime(CLOCK_ID , &end);
        datas[i] = (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ONE_SEC;
    }
    fprintf(fout , "%.12lf\n", get_avgofCI(datas , loops));


    fclose(fout);

    return 0;
}
