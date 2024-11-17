CC=gcc

UNAME := $(shell uname)

DEBUG ?= 0

INCLUDES = -I ../src/cache/  -I ../src/bmp/ -I ../src/gfx/ -I ../src/wav/  -I ../src/rand/ -I ../src/rand/

ifeq ($(DEBUG), 1)
	CFLAGS=-O0 $(INCLUDES) -Wall -g -DDEBUG $(INCLUDES)
	LDFLAGS= -lc -lm -ldl -lpthread -lasan
else
	CFLAGS=-O3 $(INCLUDES) -Wall $(INCLUDES)  #-DSAMPLERATE_5MSPS
	LDFLAGS= -lc -lm -ldl -s
endif

ifeq ($(UNAME), Darwin)
CFLAGS += -arch x86_64 -mmacosx-version-min=10.9
LDFLAGS += -arch x86_64 -mmacosx-version-min=10.9
endif

ifeq ($(UNAME), Linux)
LDFLAGS += -Wl,-rpath=.
endif

ifeq ($(findstring CYGWIN,$(shell uname)),CYGWIN)
endif

EXEC=test

all: $(EXEC)

test:  test.o wav_file.o rand_gen.o bmp_file.o cache.o gfx.o font8x8.o font4x6.o
	$(CC) -o $@    $^ $(LDFLAGS)

test.o: ../src/test_prg/test.c
	$(CC) -o $@ -c $< $(CFLAGS)

wav_file.o: ../src/wav/wav_file.c
	$(CC) -o $@ -c $< $(CFLAGS)

gfx.o: ../src/gfx/gfx.c
	$(CC) -o $@ -c $< $(CFLAGS)

font8x8.o: ../src/gfx/font8x8.c
	$(CC) -o $@ -c $< $(CFLAGS)

font4x6.o: ../src/gfx/font4x6.c
	$(CC) -o $@ -c $< $(CFLAGS)

cache.o: ../src/cache/cache.c
	$(CC) -o $@ -c $< $(CFLAGS)

bmp_file.o: ../src/bmp/bmp_file.c
	$(CC) -o $@ -c $< $(CFLAGS)

rand_gen.o: ../src/rand/rand_gen.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf *.so

mrproper: clean
	rm -rf $(EXEC)

.PHONY: clean mrproper
