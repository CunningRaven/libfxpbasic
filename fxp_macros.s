@.macro neg64_macro m0, m1, insset
@    mvn \m0, \m0
@    mvn \m1, \m1
@.if \insset == 32
@    adds \m0, \m0, #1
@    adc \m1, \m1, #0
@.else
@    mov \insset, #1
@    cmn \m0, \insset
@    adc \m1, \m1, \insset
@    sub \m1, \m1, #1
@    add \m0, \m0, #1
@.endif
@.endm

.macro neg64_macro_thumb reg0, reg1, buff
mvn \reg0, \reg0
mvn \reg1, \reg1
mov \buff, #1
cmn \reg0, \buff
adc \reg1, \reg1, \buff
sub \reg1, \reg1, #1
add \reg0, \reg0, #1
.endm

.macro neg64_macro_arm reg0, reg1
mvn \reg0, \reg0
mvn \reg1, \reg1
adds \reg0, \reg0, #1
adc \reg1, #0
.endm

.macro neg64_macro_cond m0, m1, vr
cmp \vr, #0
mvnne \m0, \m0
mvnne \m1, \m1
addeqs \vr, \vr, #0
addnes \m0, \m0, #1
adc \m1, \m1, #0
.endm

.macro lsl1bit64 m0, m1 @ LSL 1 BIT - 64
@ .code 32
@ .align 4
lsls \m0, \m0, #1
lsl \m1, \m1, #1
adc \m1, #0
.endm

.macro lsl1bit64_ca m0, m1, ca @ LSL 1 BIT - 64 - involving CArry
@ .code 32
@ .align 4
lsls \m0, \m0, #1
add \m0, \m0, \ca
mov \ca, #0
adc \ca, #0
lsls \m1, \m1, #1
add \m1, \m1, \ca
mov \ca, #0
adc \ca, #0
.endm

.macro add64_arm m0, m1, su0, su1
adds \m0, \m0, \su0
adc \m1, \m1, \su1
.endm

.macro add64_thumb m0, m1, n0, n1
cmn m0, n0
adc m1, n1, m1
add m0, m0, n0
.endm

.macro lslnbits64 m0, m1, n @ LSL N-BITS - 64
@ .code 32
@ .align 4
cmp \n, #32
movcs \m1, \m0
movcs \m0, #0
subcs \n, \n, #32
lsl \m1, \m1, \n
rsbcc \n, \n, #32
orrcc \m1, \m1, \m0, lsr \n
rsbcc \n, \n, #32
lslcc \m0, \m0, \n
.endm

