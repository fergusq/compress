CFLAGS=-Wall

all: compress decompress

compress: src/compress.c src/util.h
	gcc $(CFLAGS) -o $@ $<

decompress: src/decompress.c src/util.h
	gcc $(CFLAGS) -o $@ $<

test: all
	cd test-files; ./test-files.sh xml-test-files; ./test-files.sh random-test-files

.PHONY: all test