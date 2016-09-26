# set phony targets
.PHONY: install manpage pkgconfig clean pristine

install: libperlin.a perlin.h manpage pkgconfig
	mkdir -p /usr/local/lib/perlin/
	cp libperlin.a /usr/local/lib/perlin/
	mkdir -p /usr/local/include/perlin/
	cp perlin.h /usr/local/include/perlin/

stub_from_lib: stub.o
	gcc -g stub.o -o stub_from_lib $(shell pkg-config --libs libperlin) #-L. -lperlin

stub: stub.o perlin.o
	gcc -g stub.o perlin.o -o stub

stub.o: stub.c perlin.h
	gcc -g -c stub.c

perlin.o: perlin.c
	gcc -g -c perlin.c

libperlin.a: perlin.o
	ar -r libperlin.a perlin.o

manpage: perlin.3
	cp perlin.3 /usr/share/man/man3/

pkgconfig: libperlin.pc
	mkdir -p /usr/local/share/pkgconfig/
	cp libperlin.pc /usr/local/share/pkgconfig/

clean:
	rm -f stub stub_from_lib *.o

pristine: clean
	rm -f libperlin.a

