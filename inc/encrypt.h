#ifndef ENCRYPT_H
#define ENCRYPT_H
// Initial permutation of the plaintext per DES
unsigned long long int initialPermutation(unsigned long long int plaintext);

// Expand a 32 bit number to 48 bits per DES
unsigned long long int expander(unsigned long long int halfCipher);

// Takes 6 bits and returns 4 bits
unsigned long long int sBoxes(unsigned long long int input);

// Another permutation that apparently happens
unsigned long long int permutationAlpha(unsigned long long int cipherChunk);

#endif