AS_PRE = -march=armv7-a

%.s: %.S
	cpp $< -o $@

%.o: %.s
	as $(AS_PRE) $< -o $@

%.debug:
	gcc -std=gnu99 -Wall $^ -o $@ -g

%:
	gcc -std=gnu99 -Wall $^ -o $@

lib%.so:
	gcc -shared -o $@ $^

libfxpbasic.so: fxp.o 

calculator.debug: test_fxpbasic.c fxp_debug.o 

fxp.o: fxp.s

fxp_debug.o: fxp.s
	as $(AS_PRE) --gstabs+ $< -o $@

clean:
	-rm *.o
	-rm *.debug
	-rm lib*.so
	-rm fxp.s

.PHONY: clean
