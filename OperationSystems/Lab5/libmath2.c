#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

int dec2bin(long num){
    int bin = 0, k = 1;
    while (num != 0)
    {
        bin += (num % 2) * k;
        k *= 10;
        num /= 2;
    }
    return bin;
}

int dec2third(long num) {
    int third = 0, k = 1;
    while (num)
    {
        third += (num % 3) * k;
        k *= 10;
        num /= 3;
    }
    return third;
}
