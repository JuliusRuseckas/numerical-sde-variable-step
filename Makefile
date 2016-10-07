CC = gcc

CFLAGS = -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes \
-Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
-Wwrite-strings -fshort-enums -fno-common -O3

TARGET_ARCH = -mtune=native -march=native

LDLIBS = -lgsl -lgslcblas -lfftw3 -lm

objects = utilities.o compute_signal.o spectrum.o

.PHONY: clean all

all: spectrum

spectrum: $(objects)

compute_signal.o spectrum.o: parameters.h

makewisdom: makewisdom.c

wisdom: makewisdom
	./makewisdom

clean:
	$(RM) *.o spectrum makewisdom
