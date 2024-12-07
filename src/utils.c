#include "../inc/utils.h"
#include <stdlib.h>
// AND a number with 1 to return an integer 0 or 1
int ANDToBool(unsigned long long int plaintext, unsigned long long int bitCheck)
{
    unsigned long long int newValue = plaintext & bitCheck;
    if (newValue != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


// Generate a new key for the ciphertext to encrypt with (random generation)
unsigned long long int generateKey()
{
    // 64 bit int for the key
    unsigned long long int key = 0x0000000000000000;

    int i = 0; // Counter
    
    int oneCounter = 0; // Counts how many ones are in each span of 7 bits
    while (i < 64) // 64 bits to make
    {   
        
        // For each 8th iteration, add a parity bit
        if (i % 8 == 7)
        {
            if (oneCounter % 2 == 0) // If the one counter has an even number of 1s
            {
                key = (key << 1) | 0x0000000000000001; // Parity is 1 to make total number odd
            }
            else // If the one counter has an odd number of 1s
            {
                key = key << 1; // Parity is 0 to make total number odd
            }
            i++; // Increment counter
            oneCounter = 0; // Reset one counter for next group
            continue; // Restart loop
        }
        //For every other iteration, get a random bit
        int newBit = rand() % 2; // Get a random integer from 0 to 1
        key = key << 1; // Shift one bit up
        if (newBit == 1) // If random bit is 1, add one
        {
            key = key | 0x0000000000000001; // Use AND operation to accomplish
            oneCounter++;
            // Otherwise, shift is the zero we need
        }
        i++; // Increment counter

    }
    return key;

}