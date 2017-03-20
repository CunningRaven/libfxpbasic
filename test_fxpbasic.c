#include "fxpbasic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

typedef union {
    int64_t i64;
    int32_t i32[2];
} uni64_s;
typedef union {
    uint64_t u64;
    uint32_t u32[2];
} uni64_u;
#define FP_PRINTLEN 9
#define FP_MAX 999999999U
#define FP_MIN 100000000U
#define FP_INI 500000000U
#define FXPBUFSIZ 127
#define MACROQUOT(m) QUOTATE(m)
#define QUOTATE(m) #m
uint64_t (*operp[])(uint32_t, uint32_t, uint32_t, uint32_t) = {
    fxpadd, fxpsub, fxpmul_signed, fxpmul_unsigned, fxpdiv_signed,
    fxpdiv_unsigned
};
void printfxp(FILE *fp, uni64_u u);

static char *scan_sign(char *restrict str, int *restrict is_neg)
{
    while (!isdigit(*str) && *str)
        if (*str++ == '-')
            *is_neg = !*is_neg;
    return str;
}

static char *find_dot(char *str)
{
    while (isdigit(*str))
        str++;
    if (*str == '.') {
        *str = '\0';
        return str + 1;
    } else
        return NULL;
}

uint32_t convfpofxp(uint32_t fp)
{
    uint32_t subtrahend = FP_INI, rnt = 0;
    int i;

    if (fp == 0)
        return 0;
    while (fp < FP_MIN)
        fp *= 10;
    while (fp > FP_MAX)
        fp /= 10;
    for (i = 32; i > 0 && subtrahend > 0 && fp > 0;
            i--) {
        rnt <<= 1;
        if (fp > subtrahend) {
            fp -= subtrahend;
            rnt |= 1;
        }
        subtrahend >>= 1;
    }
    rnt <<= i;
    return rnt;
}

static uint64_t str2fxp(char *str)
{
    int is_neg = 0;
    char *fp, *ip;
    uni64_u u;

    ip = scan_sign(str, &is_neg);
    u.u32[1] = strtoul(ip, NULL, 10);
    if ((fp = find_dot(ip)) == NULL)
        u.u32[0] = 0;
    else {
        u.u32[0] = strtoul(fp, NULL, 10);
        u.u32[0] = convfpofxp(u.u32[0]);
    }
    if (is_neg)
        u.u64 = fxpneg(u.u32[0], u.u32[1]);
    return u.u64;
}

int scanfxp(FILE *restrict fp, uni64_u *restrict rsltp)
{
    int r;
    char buff1[FXPBUFSIZ+1], buff2[FXPBUFSIZ+1];
    char operator;
    uni64_u operand1, operand2;

    r = fscanf(fp, "%"MACROQUOT(FXPBUFSIZ)"s %c %"MACROQUOT(FXPBUFSIZ)"s", 
            buff1, &operator, buff2);
    if (r == EOF)
        return r;
    else if (r != 3) 
        return 1;
    else {
        int i = 0;

        operand1.u64 = str2fxp(buff1);
        operand2.u64 = str2fxp(buff2);
        switch (operator) {
            case '\\':
                i++;
            case '/':
                i++;
            case '&':
                i++;
            case '*':
                i++;
            case '-':
                i++;
            case '+':
                break;
            default:
                return 1;
        }
        rsltp->u64 = operp[i](operand1.u32[0], operand1.u32[1],
                operand2.u32[0], operand2.u32[1]);
        printfxp(stderr, operand1);
        fprintf(stderr, " %c ", operator);
        printfxp(stderr, operand2);
        fputs(" = ", stderr);
        printfxp(stderr, *rsltp);
        fputc('\n', stderr);
        return 0;
    }
}

uint32_t rconvfpofxp(uint32_t fp)
{
    uint32_t addend = FP_INI, rnt = 0,
             mask = (uint32_t)INT32_MIN;

    while (fp && addend && rnt + addend < FP_MAX) {
        if (fp & mask)
            rnt += addend;
        fp <<= 1;
        addend >>= 1;
    }
    return rnt;
}

void printfxp(FILE *fp, uni64_u u)
{
    if ((int32_t)u.u32[1] < 0) {
        putc((int)(unsigned)'-', fp);
        u.u64 = fxpneg(u.u32[0], u.u32[1]);
    }
    fprintf(fp, "%u", u.u32[1]);
    if (u.u32[0] != 0) 
        fprintf(fp, ".%0"MACROQUOT(FP_PRINTLEN)"u", rconvfpofxp(u.u32[0]));
}

int main(void)
{
    uni64_u u;
    int r;
    char prompt[] = "expression: ";

    while (fputs(prompt, stdout), (r = scanfxp(stdin, &u)) != EOF)
        if (r) 
            fflush(stdin);
    return 0;
}
