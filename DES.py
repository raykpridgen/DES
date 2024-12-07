from numpy import random as rand
import math

# Doing a DES encryption / decrpytion script
# Plan:
#   1. Take in string, convert it to ASCII 
#   2. Then, Convert ASCII to binary
#       A) Need a list of ASCII then list of binary strings
#       B) 32-126 is the printable character range
#       C) 64 < PCR < 128, so 2^7 = 128 bits long for each character
#   3. Encrypt binary with DES
#   `   A) Maybe have two separate program files each with encrypt and decrypt, and a .txt file in the middle`
#   

# Encrypt fucntion for DES, takes a plaintext string
def encryptDES(plaintext):
    binaryString = "" # String to hold binary value of plaintext
    for char in plaintext: # For each character
        binaryString += format(ord(char), '08b') # Add the binary value of the ASCII code to the string

    parityString = addParityBits(binaryString)

# This function generates a random 48 bit key used to encrypt the message
def generateKey():
    binaryString = "" # Blank string
    i = 0 # Counter variable
    while i < 56: # Do 48 timees, for 48 bits
        randomBit = rand.randint(0, 2) # Pick a random bit, 0 or 1
        binaryString += str(randomBit) # Add random bit to string
        i += 1 # Increment

    return int(binaryString, 2)

# This function takes a string of binary and adds parity bits every 8th bit
def addParityBits(binaryText):
    binaryText = bin(binaryText)[2:]
    print(binaryText)
    iterations = math.ceil(len(binaryText) / 7) # This is the number of times the algorithm needs to run
    parityString = "" # New string to have parity bit added to
    while iterations > 0:
        stringToParity = binaryText[0:7] # Take first 7 bits of the string
        oddCounter = 0 # Counter to check amount of ones
        for char in stringToParity: # For each bit in the string
            if char == "1": # If it is a 1
                oddCounter += 1 # Increment
        if oddCounter % 2 == 0: # If number of 1s is even
            parityString += stringToParity + "1" # Add a 1 as parity bit 
        else: # otherwise
            parityString += stringToParity + "0" # Add a 0 as parity bit
        binaryText = binaryText[7:] # Set the original string to the string minus the first seven bits
        iterations -= 1 # Decrement counter
    print(parityString)
    return int(parityString, 2) # Return parity string

def functionF(rightBits, key):
    
    pass

def sBox(inputBits):
    matrix = [[0b1010, 0b1100, 0b1111, 0b0010, 0b0001, 0b1010, 0b1110, 0b1101, 0b1001, 0b0101, 0b0011, 0b0010, 0b1011, 0b1101, 0b0010, 0b1011],
              [0b0000, 0b0100, 0b1001, 0b1101, 0b0011, 0b1011, 0b1001, 0b1001, 0b1101, 0b0010, 0b1000, 0b1100, 0b1010, 0b1111, 0b0000, 0b0000],
              [0b1101, 0b0010, 0b1010, 0b1111, 0b1101, 0b1011, 0b1010, 0b0101, 0b1001, 0b1010, 0b0010, 0b0001, 0b0001, 0b1010, 0b1101, 0b1111],
              [0b0001, 0b0000, 0b1101, 0b1010, 0b0101, 0b0001, 0b1001, 0b0010, 0b0100, 0b1110, 0b1111, 0b0001, 0b0100, 0b1100, 0b0101, 0b1101]]

    row = inputBits >> 4 # Shift off unneeded bits for the row
    mask = 0b001111 # AND mask for the column
    column = inputBits & mask # Apply mask with AND operator to get the column

    return matrix[row][column]
        
key = generateKey()
addParityBits(key)



#newInput = input("Enter plaintext: ")
#encryptDES(newInput)
#generateKey()

