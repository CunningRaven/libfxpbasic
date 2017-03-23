CFLAGS = -std=gnu99 -Wall
CPPFLAGS = -DLINUX -D_POSIX_C_SOURCE=200809L
AS_PRE = -march=armv7-a

%.s: %.S
	cpp $< -o $@

%.o: %.s
	$(CC) -c $(AS_PRE) -fPIC $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -fPIC $^ -o $@

%.debug:
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ -g

%:
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

lib%.so:
	$(CC) -shared -o $@ $^

libfxpbasic.so: fxp.o fxpmath.o

calculator.debug: test_fxpbasic.c fxp_debug.o fxpmath_debug.o

calculator: test_fxpbasic.c libfxpbasic.so
	$(CC) $(CFLAGS) $(CPPFLAGS) -L. $< -lfxpbasic -o $@

fxp.o: fxp.s

fxp_debug.o: fxp.s
	as $(AS_PRE) --gstabs+ $< -o $@

fxpmath_debug.o: fxpmath.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $^ -o $@ -g

clean:
	-rm *.o
	-rm *.debug
	-rm lib*.so
	-rm fxp.s
	-rm calculator

.PHONY: clean
