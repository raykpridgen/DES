#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inc/encrypt.h"
#include "inc/decrypt.h"
#include "inc/utils.h"

typedef unsigned long long ull;
int decryptDES()
{
    // Decrpytion is the reverse process of encryption, in reverse order of the keys
    return 0;
} 

// The inputs need to be the 64 bit ciphertext and the 64b key
int encryptDES(unsigned long long int plaintext, unsigned long long int key)
{
    printf("Plaintext: %llx\n", plaintext);
    // Take initial data, run it through initial permutation to prepare for shifting
    plaintext = initialPermutation(plaintext);
    
    printf("Initial Permutation: %llx\n", plaintext);

    // Process two halves of the data 16 times. 
    // Split permutation into left and right, 32b each
    unsigned long long int rightBits = (plaintext << 32) >> 32;

    printf("Right half: %llx\n", rightBits);

    // Expand the right half 32 -> 48
    
    // Iterate through 16 rounds of encryption
    for (int k=0; k < 1; k++)
    {
        // Figure out how to implement this key pattern
        unsigned long long int subKey = shuffleKey(key, k);

        rightBits = expander(rightBits);
        printf("Expanded right half: %llx\n", rightBits);    

        // XOR with the subkey given this round
        //rightBits = rightBits ^ subKey;

        // Use another, separate permutation function to scramble bits again
        rightBits = permutationAlpha(rightBits);

        // XOR with the left side
        rightBits = rightBits ^ (plaintext >> 32);

        // Repeat with the previous right side as the left side, and the new right side as the right
        plaintext = (plaintext << 32) | rightBits;
    }
    
    /*
    Big thing to figure out is how to do the key shifting for each round

    key will be:
    00000000000000000000000000000000000000000000000000000000
    11111111111111111111111111111111111111111111111111111111

    need to get 16 of these: 
    000000000000000000000000000000000000000000000000
    
    */
    return 0;
}


int main() {
    srand(time(NULL)); // Seed using clock
    unsigned long long int testVal = 0b0000000000000000111111111111111111000000011111111111111111111111;
    //                                 ________________--____--____--____--____--____--____--____--____
    //                                                 000000000000000000000000000000000000000000000000
    unsigned long long int sBoxOutput = sBoxes(testVal);

    printf("SBOX\nInput:  %llx\nOutput: %llx", testVal, sBoxOutput);
    
    
}
