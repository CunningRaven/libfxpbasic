.include "fxp_macros.s"
.text
.code 16
.align 2

.global fxpneg
.global cfxpneg
.type fxpneg, %function
.type cfxpneg, %function
@ fxpneg: to let a 32.32 fixed-point decimal become its opposite;
@ r0: fraction part, r1: integral part
fxpneg:
cfxpneg:
neg64_macro_thumb r0, r1, r2
bx lr

.global fxpadd
.global cfxpadd
.type fxpadd, %function
.type cfxpadd, %function
@ fxpadd: addiction of two 32.32 fixed-point decimals
@ r0: fractional part of 1st addend, r1: integral part of 1st addend,
@ r2: FP of 2nd, r3: IP of 2nd
fxpadd:
cfxpadd:
cmn r0, r2
adc r1, r1, r3
add r0, r0, r2
bx lr

.global fxpsub
.global cfxpsub
.type fxpsub, %function
.type cfxpsub, %function
@ fxpsub: subtraction of two 32.32 fixed-point decimals
@ r0: fractional part of minuend, r1: integral part of minuend,
@ r2: FP of subtrahend, r3: IP of subtrahend
fxpsub:
cfxpsub:
cmp r0, r2
sbc r1, r1, r3
sub r0, r2
bx lr

.macro fxpmul_unsigned_macro rl, rh, mcndl, mcndh, mtorl, mtorh, trashbin
eor \rh, \rh, \rh
umull \trashbin, \rl, \mcndl, \mtorl
umlal \rl, \rh, \mcndl, \mtorh
umlal \rl, \rh, \mtorl, \mcndh
umlal \rh, \trashbin, \mcndh, \mtorh
.endm

.global fxpmul_unsigned
.global cfxpmul_unsigned
.type fxpmul_unsigned, %function
.type cfxpmul_unsigned, %function
@ fxpmul_unsigned: unsigned multiplication of two 32.32 fixed-point decimals
@ r0: fractional part of multiplicand, r1: integral part of multiplicand,
@ r2: FP of multiplicator, r3: IP of multiplicator
fxpmul_unsigned:
cfxpmul_unsigned:
push {r4 , r5}
fxpmul_unsigned_macro r4, r5, r0, r1, r2, r3, r12
mov r0, r4
mov r1, r5
pop {r4, r5}
bx lr

.macro fxpmul_checksign obj, srcl, srch, buff
asr \srch, \srch, #31
mov \buff, #1
cmn \srch, \buff
adc \buff, \buff, \buff
sub \buff, #2
and \srcl, \srcl, \srch
eor \srcl, \srcl, \srch
add \srcl, \srcl, \buff
add \obj, \obj, \srcl
.endm

.global fxpmul_signed
.global cfxpmul_signed
.type fxpmul_signed, %function
.type cfxpmul_signed, %function
@ fxpmul_signed: signed multiplication of two 32.32 fixed-point decimals
@ r0: fractional part of multiplicand, r1: integral part of multiplicand,
@ r2: FP of multiplicator, r3: IP of multiplicator
fxpmul_signed:
cfxpmul_signed:
push {r4, r5}
fxpmul_unsigned_macro r12, r5, r0, r1, r2, r3, r4
fxpmul_checksign r5, r0, r3, r4
fxpmul_checksign r5, r2, r1, r4
mov r0, r12
mov r1, r5
pop {r4, r5}
bx lr

.code 32
.align 4

.global fxpdiv_signed
.global cfxpdiv_signed
.type fxpdiv_signed, %function
.type cfxpdiv_signed, %function
fxpdiv_signed:
cfxpdiv_signed:
push {r4 - r10, lr}
orrs r4, r2, r3
popeq {r4 - r10, pc}
eors r4, r1, r3
movpl r5, #1
movmi r5, #0
mov r9, r2
mov r10, r3
neg64_macro_cond r9, r10, r5
push {r4}
sbfx r4, r1, #31, #1
mov r5, #64
mov r6, #0
mov r7, #0
#define RGSTR r4
#define CNTRL r5
#define QUOT0 r6
#define QUOT1 r7
#define BUFF0 r8
#define DVSR0 r9
#define DVSR1 r10
fxpdiv_signed_loop:
mov r2, DVSR0
mov r3, DVSR1
lsl1bit64 QUOT0, QUOT1
mov BUFF0, #0
lsl1bit64_ca r0, r1, BUFF0
lsl RGSTR, RGSTR, #1
orr RGSTR, RGSTR, BUFF0
add64_arm r2, r3, r1, RGSTR
orr BUFF0, r2, r3
orr BUFF0, BUFF0, r0
cmp BUFF0, #0
beq fxpdiv_signed_break
teq r3, RGSTR
movpl r1, r2
movpl RGSTR, r3
orrpl QUOT0, QUOT0, #1
sub CNTRL, CNTRL, #1
cmp CNTRL, #0
bne fxpdiv_signed_loop
fxpdiv_signed_out:
mov r0, QUOT0
mov r1, QUOT1
ldr r2, [sp], #4
cmp r2, #0
movpl r2, #0
neg64_macro_cond r0, r1, r2
pop {r4 - r10, pc}
fxpdiv_signed_break:
orr QUOT0, QUOT0, #1
sub CNTRL, CNTRL, #1
lslnbits64 QUOT0, QUOT1, CNTRL
bl fxpdiv_signed_out
#undef RGSTR
#undef CNTRL
#undef QUOT0
#undef QUOT1
#undef BUFF0
#undef DVSR0
#undef DVSR1

.global fxpdiv_unsigned
.global cfxpdiv_unsigned
.type fxpdiv_unsigned, %function
.type cfxpdiv_unsigned, %function
@ fxpdiv_unsigned: division of two 32.32 fixed point decimals
@ r0: fractional part of dividend, r1: integral part of dividend,
@ r2: FP of divisor, r3: IP of divisor
fxpdiv_unsigned:
cfxpdiv_unsigned:
push {r4 - r9, lr}
orrs r4, r2, r3
popeq {r4 - r9, pc}
#define RGSTR r4
#define CNTRL r5
#define PRVW0 r6
#define PRVW1 r7
#define QUOT0 r8
#define QUOT1 r9
mov RGSTR, #0
mov CNTRL, #64
neg64_macro_arm r2, r3
mov QUOT0, #0
mov QUOT1, #0
fxpdiv_unsigned_loop:
mov PRVW1, #0
lsl1bit64_ca r0, r1, PRVW1
lsl RGSTR, RGSTR, #1
orr RGSTR, RGSTR, PRVW1
lsl1bit64 QUOT0, QUOT1
adds PRVW0, r1, r2
adcs PRVW1, RGSTR, r3
movcs r1, PRVW0
movcs RGSTR, PRVW1
orrcs QUOT0, QUOT0, #1
subs CNTRL, CNTRL, #1
bne fxpdiv_unsigned_loop
mov r0, QUOT0
mov r1, QUOT1
pop {r4 - r9, pc}
#undef RGSTR
#undef CNTRL
#undef PRVW0
#undef PRVW1
#undef QUOT0
#undef QUOT1
