AS_PRE = -march=armv7-a

%.s: %.S
	cpp $< -o $@

%.o: %.s
	gcc -c $(AS_PRE) -fPIC $< -o $@

%.debug:
	gcc -std=gnu99 -Wall $^ -o $@ -g

%:
	gcc -std=gnu99 -Wall $^ -o $@

lib%.so:
	gcc -shared -o $@ $^

libfxpbasic.so: fxp.o 

calculator.debug: test_fxpbasic.c fxp_debug.o 

calculator: test_fxpbasic.c libfxpbasic.so
	gcc -std=gnu99 -Wall -L. $^ -lfxpbasic -o $@

fxp.o: fxp.s

fxp_debug.o: fxp.s
	as $(AS_PRE) --gstabs+ $< -o $@

clean:
	-rm *.o
	-rm *.debug
	-rm lib*.so
	-rm fxp.s
	-rm calculator

.PHONY: clean
