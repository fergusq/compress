#include <stdio.h>
#include "util.h"

#define WINDOW_SIZE 2000

typedef struct prefix {
	// Relatiivinen sijainti eli etäisyys aiempaan kohtaan
	u_int16_t location;

	// Etuliitteen pituus
	u_int16_t length;
} prefix;

// Etsii yhteitä etuliitettä merkkijonon kodista i ja j (i > j).
// Palauttaa prefix-olion, jossa on merkitty etuliitteen relatiivinen sijainti ja koko
prefix find_prefix_at(long i, long j, data file) {
	prefix p = { location: i-j, length: 0 };
	while (i < file.length && file.buffer[i++] == file.buffer[j++]) p.length++;
	return p;
}

// Etsii aiempaa kohtaa tiedostosta, jolla on sama etuliite kuin kohdalla i
// Käy läpi enintään WINDOW_SIZE-muuttujassa määritellyn määrän kohtia
prefix find_prefix(long i, data file) {
	prefix g = { location: 0, length: 0 };
	for (int j = i <= WINDOW_SIZE ? 0 : i-WINDOW_SIZE; j < i; j++) {
		prefix p = find_prefix_at(i, j, file);
		if (p.length > g.length) g = p;
	}
	return g;
}

// Tulostaa koodinvaihtomerkin 128, viittauksen relatiivisen sijainnin ja viittauksen pituuden (little endian -formaatissa)
void print_reference(short location, short length) {
	putchar(128);
	putchar(location & 0xFF);
	putchar(location >> 8);
	putchar(length & 0xFF);
	putchar(length >> 8);
}

// Pakkaa tiedoston ja tulostaa pakatun datan
void compress(data file) {
	fprintf(stderr, "Uncompressed size: %ld\n", file.length);
	unsigned long compressed_length = 4;

	// Pakatun datan alussa on tiedoston alkuperäinen koko (little endian -formaatissa)
	putchar(file.length & 0xFF);
	putchar((file.length >> 8) & 0xFF);
	putchar((file.length >> 16) & 0xFF);
	putchar((file.length >> 24) & 0xFF);

	for (long i = 0; i < file.length; i++) {
		prefix p = find_prefix(i, file);

		// Jos löytyi etuliite, joka on pidempi kuin etuliiteviittauksen koko (5), tee viittaus
		if (p.length > 5) {
			print_reference(p.location, p.length);
			i += p.length - 1;
			compressed_length += 5;
		} else if (file.buffer[i] == 128) {
			// Koska 128 on koodinvaihtomerkki, se pitää koodata pakattuun dataan erikoisella tavalla, viittauksena, jonka pituus on 0
			print_reference(0, 0);
			compressed_length += 5;
		} else {
			// Tulostetaan tavallinen merkki
			putchar(file.buffer[i]);
			compressed_length++;
		}
	}
	fprintf(stderr, "Compressed size: %ld (%ld%%)\n", compressed_length, 100*compressed_length/file.length);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(1);
	}
	
	data f = read_file(argv[1]);
	compress(f);
	free(f.buffer);
}