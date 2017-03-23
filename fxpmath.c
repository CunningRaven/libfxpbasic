#include "fxpbasic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t sincal(uint64_t a) __attribute__((__const__));

static uint64_t sincal(uint64_t a)
{
    return cfxpsub(cfxpmul_signed((uint64_t)3 << 32, a),
            cfxpmul_signed((uint64_t)4 << 32, 
                cfxpmul_signed(a, 
                    cfxpmul_signed(a, a))));
}

uint64_t fxpsin(uint32_t frp, uint32_t inp)
{
    int times = 0;
    uint32_t u[2] = {frp, inp};

    while ((u[1] != 0 || u[0] >= 0x8000U) &&
            (u[1] != ~0 || u[0] < 0xffff7fffU)) {
        *(uint64_t *)u = fxpdiv_signed(u[0], u[1], 0, 3);
        times++;
    }
    for (; times > 0; times--)
        *(uint64_t *)u = sincal(*(uint64_t *)u);
    return *(uint64_t *)u;
}

uint64_t cfxpsin(uint64_t u)
{
    uint32_t a[2];

    *(uint64_t *)a = u;
    return fxpsin(a[0], a[1]);
}
