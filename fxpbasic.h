#ifndef FXPBASIC_H
#define FXPBASIC_H

#include <stdint.h>

#ifndef __GNUC__
#define __attribute__(x) 
#endif

uint64_t fxpneg(uint32_t frp, uint32_t inp) __attribute__((__const__));
uint64_t fxpadd(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpsub(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpmul_unsigned(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpmul_signed(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpdiv_unsigned(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpdiv_signed(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2) __attribute__((__const__));
uint64_t fxpsin(uint32_t frp, uint32_t inp) __attribute__((__const__));
uint64_t fxptan(uint32_t frp, uint32_t inp) __attribute__((__const__));
uint64_t fxpexp(uint32_t frp, uint32_t inp) __attribute__((__const__));
uint64_t fxpln(uint32_t frp, uint32_t inp) __attribute__((__const__));

uint64_t cfxpneg(uint64_t u) __attribute__((__const__));
uint64_t cfxpadd(uint64_t a1, uint64_t a2) __attribute__((__const__));
uint64_t cfxpsub(uint64_t minuend, uint64_t subtrahend) __attribute__((__const__));
uint64_t cfxpmul_unsigned(uint64_t multiplicand, uint64_t multiplier) __attribute__((__const__));
uint64_t cfxpmul_signed(uint64_t multiplicand, uint64_t multiplier) __attribute__((__const__));
uint64_t cfxpdiv_unsigned(uint64_t dividend, uint64_t divisor) __attribute__((__const__));
uint64_t cfxpdiv_signed(uint64_t dividend, uint64_t divisor) __attribute__((__const__));
uint64_t cfxpsin(uint64_t u) __attribute__((__const__));
uint64_t cfxptan(uint64_t u) __attribute__((__const__));
uint64_t cfxpexp(uint64_t u) __attribute__((__const__));
uint64_t cfxpln(uint64_t u) __attribute__((__const__));
#endif
