CFLAGS=-Wall

all: compress decompress

compress: src/compress.c src/util.h
	gcc $(CFLAGS) -o $@ $<

decompress: src/decompress.c src/util.h
	gcc $(CFLAGS) -o $@ $<

test: all
	mkdir -p test/all-sizes-xml-test-files
	cd test; ./test-files.sh xml-test-files; ./test-files.sh random-test-files; ./test-files.sh all-sizes-xml-test-files;

.PHONY: all test