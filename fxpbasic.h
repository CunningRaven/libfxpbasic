#ifndef FXPBASIC_H
#define FXPBASIC_H

#include <stdint.h>

uint64_t fxpneg(uint32_t frp, uint32_t inp);
uint64_t fxpadd(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);
uint64_t fxpsub(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);
uint64_t fxpmul_unsigned(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);
uint64_t fxpmul_signed(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);
uint64_t fxpdiv_unsigned(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);
uint64_t fxpdiv_signed(uint32_t frp1, uint32_t inp1, uint32_t frp2, uint32_t inp2);

#endif
