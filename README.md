# libfxpbasic
libfxpbasic: Implement a basic 32.32 fixed point toolkit using assembly on ARM architecture.
It will make such items:
1. libfxpbasic.so: main objective providing basic calculations of 32.32 fixed-point decimals. They're:
  fxpneg: to let a 32.32 fixed-point decimal become its opposite;
  fxpadd: addiction of two;
  fxpsub: subtraction;
  fxpmul_unsigned: multiplication of two unsigned;
  fxpmul_signed: multiplication of two signed;
  fxpdiv_unsigned: division of two unsigned;
  fxpdiv_signed: division of two signed;
2. test_libfxpbasic: a simple calculator to test our shared object.
