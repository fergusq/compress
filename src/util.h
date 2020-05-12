#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct data {
	unsigned char *buffer;
	unsigned long length;
} data;

// https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
data read_file(char *filename) {
	unsigned char *buffer = 0;
	unsigned long length;
	FILE *f = fopen(filename, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}

	if (buffer) {
		data ans = { buffer: buffer, length: length };
		return ans;
	}
	else {
		fprintf(stderr, "Failed to read file %s", filename);
		exit(1);
	}
}