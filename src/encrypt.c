#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../inc/encrypt.h"
#include "../inc/utils.h"

typedef unsigned long long ull;

int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

// Define PC-2 table (to produce 48-bit subkeys)
int PC2[48] = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/**
 * @brief
 * Applies the initial permutation to a block of plaintext
 * 
 * @param
 * 64 bit block of plaintext
 * 
 * @return
 * 64 bit block of permuted plaintext
 */
ull initialPermutation(unsigned long long int plaintext)
{
    int plainMatrix[8][8]; // Matrix to hold bits of plaintext
    // This will be used to get each indivudal bit to add it to a matrix
    //Move from left to right, getting least to most significant bit
    unsigned long long int bitGrabber = 0x8000000000000000;
    int row = 4; // Starting row of the algorithm
    int col = 7; // Starting column of the alogithm
    while (1==1) // While true
    {
        // The following carries out the inital permutation matrix of DES. 
        // Starts at the right on the 4th row, then moves up and back down until the column has been filled
        // Then moves right to left
        plainMatrix[row][col] =  ANDToBool(plaintext, bitGrabber); // Place in correct spot, get value of plaintext position
        bitGrabber = bitGrabber >> 1; // Prepare shift for next bit position
        row = row % 4; // Move to top half of matrix as per algorithm
        plainMatrix[row][col] =  ANDToBool(plaintext, bitGrabber); // Place in correct spot, get value of plaintext position
        if (row==3) // Row #3 is the last value to get placed, then move to next column
        {
            if (col == 0) // End the placings if row 3 was placed and column 0 was placed
            {
                break;
            }
            col -= 1; // If not column 0, move left
            row = 4; // Start back with row 4
            continue; // Begin another iteration of while true
        }
        row += 5; // Add 5 mod 4 iterates one each time, except with the jumping back and forth
    }

    unsigned long long int permutedPlaintext;
    // Printing the matrix for testing
    for (int i=0;i<8;i++) // Rows of matrix
    {
        for (int j=0;j<8;j++) // Cols of matrix
        {
            permutedPlaintext = permutedPlaintext << 1;
            permutedPlaintext = permutedPlaintext | plainMatrix[i][j]; 

        }
    }
    
    return permutedPlaintext;
}

/**
 * @brief
 * Expands half of a plaintext block to be sent through the sbox
 * @param
 * Takes a shifted plaintext, where 32 bits are left unshifted
 * @return
 * Returns an expanded block that is intended to be read as 48 bits
 */
ull expander(unsigned long long int halfCipher)
{
    /*
        Inital matrix
        
        01 02 03 04 05 06 07 08
        09 10 11 12 13 14 15 16
        17 18 19 20 21 22 23 24
        25 26 27 28 29 30 31 32
    
        Expanded matrix

        32 01 02 03 04 05 04 05 06 07 08 09
        08 09 10 11 12 13 12 13 14 15 16 17
        16 17 18 19 20 21 20 21 22 23 24 25
        24 25 26 27 28 29 28 29 30 31 32 01 
    */
    
    // Define the starting matrix and the expanded matrix
    int startMatrix[4][8];
    int expandedMatrix[4][12];
    unsigned long long int shifter = 0x0000000080000000; // Shifter to get each bit from the data
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<8; j++)
        {
            int item = ANDToBool(halfCipher, shifter); // AND the bit and the shfiter to get each position's boolean
            startMatrix[i][j] = item; // Append the boolean to the matrix at the specific value
            //printf("%d ", item); print for testing
            shifter = shifter >> 1; // Shift shifter to the right to get next bit in the data
        }
    }

    int counter = 1; // Start at one because [0][0] is covered
    int repeats = 1; // Start at one to account for 32
    expandedMatrix[0][0] = startMatrix[3][7]; // matrix[0][0] is the 32nd bit
    expandedMatrix[3][11] = startMatrix[0][0]; // matrix[3][11] is the first bit
    while (counter < 47) // While the counter is less than number of iterations to run through
    {
        // If the current place is at column 6 or column 0
        if (counter % 12 == 6 || counter % 12 == 0)
        {
            // Repeat the next two values as the were in the two places before
            expandedMatrix[counter / 12][counter % 12] = expandedMatrix[(counter - 2) / 12][(counter-2) % 12];
            expandedMatrix[(counter+1) / 12][(counter + 1) % 12] = expandedMatrix[(counter-1) / 12][(counter-1) % 12];
            // Increment counter and repeats
            counter = counter + 2;
            repeats = repeats + 2;
        }
        else
        {
            // Divide and modulo by 12 to get index for expanded matrix
            // Then subtract repeats, divide and modulo by 8 for original matrix
            expandedMatrix[counter / 12][counter % 12] = startMatrix[(counter - repeats) / 8][(counter - repeats) % 8];
            counter = counter + 1; // Increment coutner
        }
    }

    unsigned long long int returnExpanded = 0;
    for (int i=0; i<48; i++)
    {
        returnExpanded = returnExpanded << 1;
        returnExpanded = expandedMatrix[i/12][i%12] | returnExpanded;
    }
    return returnExpanded;
}

/**
 * @brief
 * Second permutation to be applied after the sBox has altered bits
 * @param
 * 
 */
ull permutationAlpha(unsigned long long int cipherChunk)

{
    return cipherChunk;
}

/**
 * @brief
 * Standard DES sBox, has a 4x12 matrix to output confusion bits
 * @param
 * Takes a 64 bit variable holding a shifted block of 6 bits
 * @return
 * Returns a 64 bit variable holding a shifted block of 4 bits
 */
ull sBoxes(unsigned long long int input)
{
    // Return this at the end
    unsigned long long int output = 0;
    // 3 Dimension matrix for the sboxes
    int sBoxes[8][4][16] = 
    {
        // sBox One
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        // sBox Two
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        // sBox Three
        {
            {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
            {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
            {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
            {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        // sBox Four
        {
            {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
            {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
            {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
            {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        // sBox Five
        {
            {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
            {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
            {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
            {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        // sBox Six
        {
            {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
            {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
            {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
            {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        // sBox Seven
        {
            {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
            {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
            {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
            {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        // sBox Eight
        {
            {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
            {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
            {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
            {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };
    
    // For 8 iterations, 8 sboxes
    for (int i=0; i<8; i++)
    {
        int boxVal = sBoxes[i][(input << (16 + 6 * i)) >> 62][(input << (18 + 6 * i)) >> 60];
        //printf("Sbox row: %d\nSbox col: %d\nSbox output: %d\n", (input << (16 + 6 * i)) >> 62, (input << (18 + 6 * i)) >> 60, boxVal);
        if(i==7)
        {
            output = output | boxVal;
        }
        else
        {
            output = (output | boxVal) << 6;
        }
        //printf("Output: %llx\n", output);
    }
    return output;
}

ull shuffleKey(unsigned long long int key, int iteration)
{

}

ull permute(ull key, int table[], int tableSize, int outputSize) {
    ull result = 0;
    for (int i = 0; i < outputSize; i++) {
        result <<= 1;
        result |= (key >> (64 - table[i])) & 1; // Map the bits according to the table
    }
    return result;
}

// Function to perform a left circular shift on 28 bits
ull leftShift(ull half, int shifts, int size) {
    ull mask = (1ULL << size) - 1;  // Mask for size bits
    half = (half << shifts) | (half >> (size - shifts)); // Shift and wrap
    return half & mask; // Ensure it stays within 28 bits
}

// ShuffleKey function
ull shuffleKey(ull key, int iteration) {
    // Step 1: Apply PC-1 to reduce to 56 bits
    ull permutedKey = permute(key, PC1, 56, 56);

    // Step 2: Split into two halves
    ull C = (permutedKey >> 28) & 0xFFFFFFF; // Left 28 bits
    ull D = permutedKey & 0xFFFFFFF;        // Right 28 bits

    // Step 3: Perform left shifts according to iteration
    C = leftShift(C, iteration, 28);
    D = leftShift(D, iteration, 28);

    // Step 4: Combine halves back into 56 bits
    ull combined = (C << 28) | D;

    // Step 5: Apply PC-2 to reduce to 48 bits
    ull subkey = permute(combined, PC2, 56, 48);

    return subkey; // Return the 48-bit subkey
}
