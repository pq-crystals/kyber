#include <stdint.h>
#include "util.h"

/*************************************************
* Name:        zeroize
*
* Description: Zeroizes a block of memory
*
* Arguments:   - void *data: pointer to a block of memory to
                            be zeroized
*              - size_t size: size of memory ad \ref data
**************************************************/
void zeroize(void *data, size_t size) {
    if (data) {
        volatile uint8_t *d = (volatile uint8_t *)data;
        while (size > 0){
            *d++ = 0;
            --size;
        }
    }
}
