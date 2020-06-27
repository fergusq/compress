#include <stdio.h>
#include "util.h"

// Jäsentää little endian -muodossa koodatun 16-bittisen luvun
unsigned short parse_short(unsigned char *buffer) {
	return buffer[0] | (buffer[1] << 8);
}

// Jäsentää little endian -muodossa koodatun 32-bittisen luvun
unsigned long parse_long(unsigned char *buffer) {
	return buffer[0] |
		(buffer[1] << 8) |
		(buffer[2] << 16) |
		(buffer[3] << 24);
}

// Purkaa parametrina annetun datan ja tulostaa sen
void decompress(data compressed_data) {
	// Pakatun datan alussa on pakkaamaton koko
	unsigned long uncompressed_length = parse_long(compressed_data.buffer);
	fprintf(stderr, "Uncompressed size: %ld\n", uncompressed_length);

	// Varataan tilapäinen puskuri pakkaamattomalle datalle
	unsigned char *buffer = malloc(uncompressed_length);

	// pointer-muuttujassa on puskurin koko eli seuraava tyhjä paikka
	unsigned long pointer = 0;
	for (long i = 4; i < compressed_data.length; i++) {
		// Jos törmättiin koodinvaihtomerkkiin, luetaan relatiivinen sijainti ja pituus
		if (compressed_data.buffer[i] == 128) {
			unsigned short location = parse_short(&compressed_data.buffer[i+1]);
			unsigned short length = parse_short(&compressed_data.buffer[i+3]);
			// Jos pituus on nolla, viittaus esittääkin literaalia koodinvaihtomerkkiä
			if (length == 0) {
				buffer[pointer++] = 128;
			} else {
				// Muulloin lisätään puskuriin etuliite aiempaa puskurista
				unsigned long p = pointer;
				for (long j = 0; j < length; j++) {
					buffer[pointer++] = buffer[p - location + j];
				}
			}
			i += 4;
		} else {
			// Muulloin lisätään puskuriin literaali pakatusta datasta
			buffer[pointer++] = compressed_data.buffer[i];
		}
	}

	// Tulostetaan pakkaamaton data
	for (unsigned long i = 0; i < pointer; i++) {
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