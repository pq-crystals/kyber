#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "randombytes.h"
#include "util.h"

#define SIZE_BUFF 1024

static int test_zeroize() {
    uint8_t buff[SIZE_BUFF];
    uint8_t ref_buff[SIZE_BUFF] = {0};
    randombytes(buff, SIZE_BUFF);

    zeroize(buff, SIZE_BUFF);
    if (memcmp(buff, ref_buff, SIZE_BUFF) != 0) {
        printf("memory block is NOT completely zeroized\n");
        return 1;
    } else {
        printf("memory block is completely zeroized\n");
    }
    return 0;
}

int main() {
    return test_zeroize();
}
