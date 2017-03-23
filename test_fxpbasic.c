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

const char usage[] = 
"\nUSAGE: <fxp1> <op> <fxp2>\n\n"
"Both '<fxp1>' and '<fxp2>' should be fixed point decimals, "
"e.g. '1.29', '-7.2', etc. \n"
"A reasonable omitting is acceptable, such like '19', '20.', or '.5'.\n\n"
"The '<op>' in the top usage illustration could be:\n"
"\t'+' to indicate addition,\n"
"\t'-' to indicate subtraction,\n"
"\t'*' to indicate singed multiplication,\n"
"\t'&' to indicate unsigned multiplication,\n"
"\t'/' to indicate signed division,\n"
"\t'\\' to indicate unsigned division.\n"
"\n"
"Note this program is just for testing our shared library, \n"
"and the 'presentation layer' (scanning and printing 32.32 fixed point decimals)"
" is implemented in a way simple but harmful to accuracy.\n"
"So the output results DO NOT represent the precision ability of our library.\n";

static uint64_t (*parse_prefix(char *restrict str))(uint32_t, uint32_t)
{
    if (!strncmp(str, "sin", 3))
        return fxpsin;
    else
        return NULL;
}

static char *scan_sign(char *restrict str, int *restrict is_neg)
{
    while (!isdigit(*str) && *str != '.' && *str)
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
    uint64_t (*me)(uint32_t, uint32_t);

    me = parse_prefix(str);
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
    if (me)
        u.u64 = me(u.u32[0], u.u32[1]);
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

#if 0
uni64_u pi_item(int i)
{
    uni64_u rnt = {.u32 = {0, 8}}, a = {.u32[0] = 0};

    a.u32[1] = 2 * i - 1;
    rnt.u64 = fxpdiv_signed(rnt.u32[0], rnt.u32[1], a.u32[0], a.u32[1]);
    if (!(i & 1))
        rnt.u64 = fxpneg(rnt.u32[0], rnt.u32[1]);
    return rnt;
}
#endif

int main(int argc, char *argv[])
{
    uni64_u u;
    //uni64_u pi = {.u64 = 0}, ite;
    int r;
    char prompt[] = "expression: ";

#if 0
#define CHECKFXP(x) (printf("%#x %#x\n", x.u32[0], x.u32[1]))
    for (int i = 1; ite = pi_item(i), ite.u64 != 0; i++) {
#if 0
        printfxp(stdout, ite);
        putchar('\n');
        CHECKFXP(ite);
#endif
        pi.u64 = fxpadd(pi.u32[0], pi.u32[1], ite.u32[0], ite.u32[1]);
    }
    ite.u32[1] = 0;
    if (pi.u32[0] & 1U)
        ite.u32[0] = 1;
    else
        ite.u32[0] = 0;
    pi.u32[0] >>= 1;
    if (pi.u32[1] & 1U)
        pi.u32[0] += (uint32_t)INT32_MIN;
    pi.u32[1] >>= 1;
    pi.u64 = fxpadd(pi.u32[0], pi.u32[1], ite.u32[0], ite.u32[1]);
    fputs("pi = ", stdout);
    printfxp(stdout, pi);
    putchar('\n');
#endif
    puts(usage);
    while (fputs(prompt, stdout), (r = scanfxp(stdin, &u)) != EOF)
        if (r) 
            fflush(stdin);
    return 0;
}
