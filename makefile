all: stub stub_from_lib libperlin.a

stub_from_lib: stub.o
	gcc -g stub.o -o stub_from_lib `pkg-config --libs libperlin` #-L. -lperlin

stub: stub.o perlin.o
	gcc -g stub.o perlin.o -o stub

stub.o: stub.c perlin.h
	gcc -g -c stub.c

perlin.o: perlin.c
	gcc -g -c perlin.c

libperlin.a: perlin.o
	ar -r libperlin.a perlin.o

manpage: libperlin.3
	sudo cp libperlin.3 /usr/local/share/man/man3
