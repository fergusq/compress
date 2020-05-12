#include <stdio.h>
#include "util.h"

void decompress(data file) {
	// Pakatun datan alussa on pakkaamaton koko
	unsigned long uncompressed_length =
		file.buffer[0] |
		(file.buffer[1] << 8) |
		(file.buffer[2] << 16) |
		(file.buffer[3] << 24);
	fprintf(stderr, "Uncompressed size: %ld\n", uncompressed_length);

	// Varataan tilapäinen puskuri pakkaamattomalle datalle
	unsigned char *buffer = malloc(uncompressed_length);

	// pointer-muuttujassa on puskurin koko eli seuraava tyhjä paikka
	long pointer = 0;
	for (long i = 4; i < file.length; i++) {
		// Jos törmättiin koodinvaihtomerkkiin, luetaan relatiivinen sijainti ja pituus
		if (file.buffer[i] == 128) {
			short location = file.buffer[i+1] | (file.buffer[i+2] << 8);
			short length = file.buffer[i+3] | (file.buffer[i+4] << 8);
			// Jos pituus on nolla, viittaus esittääkin literaalia koodinvaihtomerkkiä
			if (length == 0) {
				buffer[pointer++] = 128;
			} else {
				// Muulloin lisätään puskuriin etuliite aiempaa puskurista
				long p = pointer;
				for (long j = 0; j < length; j++) {
					buffer[pointer++] = buffer[p - location + j];
				}
			}
			i += 4;
		} else {
			// Muulloin lisätään puskuriin literaali pakatusta datasta
			buffer[pointer++] = file.buffer[i];
		}
	}

	// Tulostetaan pakkaamaton data
	for (long i = 0; i < pointer; i++) {
		putchar(buffer[i]);
	}

	// Vapautetaan tilapäinen puskuri
	free(buffer);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(1);
	}
	
	data f = read_file(argv[1]);
	decompress(f);
	free(f.buffer);
}