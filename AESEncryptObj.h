//
// Created by guy on 3/29/21.
//

#ifndef COSC370_PROJECT_AesEncryptObj_H
#define COSC370_PROJECT_AesEncryptObj_H

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <exception>
#include "word.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "IV.h"


/*
 * TODO: add xorBlock function to xor a block with another block (or IV for first block)
 *
 *
 * */



using namespace std;

class AesEncryptObj {
public:
    AesEncryptObj();
    AesEncryptObj(uint16_t);
    ~AesEncryptObj();

    /*
    return value: unsigned char vector containing encrypted bytes
    parameters:
      const unsigned char * containing the bytes to encrypt
      unsigned integer representing how many bytes are in the input
      const unsigned char * containing the encryption key to use
      IV object containing IV to use


    description:
      takes the bytes from the input data given, uses the bytes from the key given, and returns the encrypted version
      of the input data in the form of an unsigned char vector.
      encrypts using the cipher block chaining mode of operation.

 */
    vector<unsigned char> encrypt(const unsigned char * , uint64_t , const unsigned char *, const IV &);


    /*
return value: unsigned char vector containing encrypted bytes
parameters:
  const unsigned char * containing the bytes to encrypt
  unsigned integer representing how many bytes are in the input
  const unsigned char * containing the encryption key to use
  unsigned integer representing how many bytes are in the key
  unsigned char * pointing to the data to be used as the IV
  unsigned integer representing the length of the IV data being passed in


description:
  takes the bytes from the input data given, uses the bytes from the key given, and returns the encrypted version
  of the input data in the form of an unsigned char vector.
  encrypts using the cipher block chaining mode of operation.

*/
    vector<unsigned char> encrypt(const unsigned char * , uint64_t , const unsigned char *, const unsigned char *, uint64_t);




private:


    uint16_t keySize = 0;

//    number of words in the key
    uint8_t nK = 0;

//    number of words in a block, always 4 regardless of key size
    const uint8_t nB = 4;

//    number of rounds, depends on keysize
    uint8_t nR = 0;
    uint8_t numWordsInKeySched = 0;

//    state table
    unsigned char state[4][4] = {0};

//    original input data
    unsigned char * input = nullptr;



    uint8_t numRoundConstants = 0;
    word * roundConstants = nullptr;


    const vector<uint8_t> mixColumnsIrreduce = {8, 4, 3, 1, 0};




    //    how many bytes are in the input
    uint64_t inputLength = 0;


//     key used to encrypt the data, length depends on which key size is being used (4 * nK)
    word * key = nullptr;


//    sBox, can pass in the number you have, and it will give you the number that it should be substituted with in the SubBytes section
    const unordered_map<uint8_t , uint8_t> sBox = {
            {0x00,0x63}, {0x01,0x7c}, {0x02,0x77}, {0x03,0x7b}, {0x04,0xf2}, {0x05,0x6b}, {0x06,0x6f}, {0x07,0xc5}, {0x08,0x30}, {0x09,0x01}, {0x0a,0x67}, {0x0b,0x2b}, {0x0c,0xfe}, {0x0d,0xd7}, {0x0e,0xab}, {0x0f,0x76},
            {0x10,0xca}, {0x11,0x82}, {0x12,0xc9}, {0x13,0x7d}, {0x14,0xfa}, {0x15,0x59}, {0x16,0x47}, {0x17,0xf0}, {0x18,0xad}, {0x19,0xd4}, {0x1a,0xa2}, {0x1b,0xaf}, {0x1c,0x9c}, {0x1d,0xa4}, {0x1e,0x72}, {0x1f,0xc0},
            {0x20,0xb7}, {0x21,0xfd}, {0x22,0x93}, {0x23,0x26}, {0x24,0x36}, {0x25,0x3f}, {0x26,0xf7}, {0x27,0xcc}, {0x28,0x34}, {0x29,0xa5}, {0x2a,0xe5}, {0x2b,0xf1}, {0x2c,0x71}, {0x2d,0xd8}, {0x2e,0x31}, {0x2f,0x15},
            {0x30,0x04}, {0x31,0xc7}, {0x32,0x23}, {0x33,0xc3}, {0x34,0x18}, {0x35,0x96}, {0x36,0x05}, {0x37,0x9a}, {0x38,0x07}, {0x39,0x12}, {0x3a,0x80}, {0x3b,0xe2}, {0x3c,0xeb}, {0x3d,0x27}, {0x3e,0xb2}, {0x3f,0x75},
            {0x40,0x09}, {0x41,0x83}, {0x42,0x2c}, {0x43,0x1a}, {0x44,0x1b}, {0x45,0x6e}, {0x46,0x5a}, {0x47,0xa0}, {0x48,0x52}, {0x49,0x3b}, {0x4a,0xd6}, {0x4b,0xb3}, {0x4c,0x29}, {0x4d,0xe3}, {0x4e,0x2f}, {0x4f,0x84},
            {0x50,0x53}, {0x51,0xd1}, {0x52,0x00}, {0x53,0xed}, {0x54,0x20}, {0x55,0xfc}, {0x56,0xb1}, {0x57,0x5b}, {0x58,0x6a}, {0x59,0xcb}, {0x5a,0xbe}, {0x5b,0x39}, {0x5c,0x4a}, {0x5d,0x4c}, {0x5e,0x58}, {0x5f,0xcf},
            {0x60,0xd0}, {0x61,0xef}, {0x62,0xaa}, {0x63,0xfb}, {0x64,0x43}, {0x65,0x4d}, {0x66,0x33}, {0x67,0x85}, {0x68,0x45}, {0x69,0xf9}, {0x6a,0x02}, {0x6b,0x7f}, {0x6c,0x50}, {0x6d,0x3c}, {0x6e,0x9f}, {0x6f,0xa8},
            {0x70,0x51}, {0x71,0xa3}, {0x72,0x40}, {0x73,0x8f}, {0x74,0x92}, {0x75,0x9d}, {0x76,0x38}, {0x77,0xf5}, {0x78,0xbc}, {0x79,0xb6}, {0x7a,0xda}, {0x7b,0x21}, {0x7c,0x10}, {0x7d,0xff}, {0x7e,0xf3}, {0x7f,0xd2},
            {0x80,0xcd}, {0x81,0x0c}, {0x82,0x13}, {0x83,0xec}, {0x84,0x5f}, {0x85,0x97}, {0x86,0x44}, {0x87,0x17}, {0x88,0xc4}, {0x89,0xa7}, {0x8a,0x7e}, {0x8b,0x3d}, {0x8c,0x64}, {0x8d,0x5d}, {0x8e,0x19}, {0x8f,0x73},
            {0x90,0x60}, {0x91,0x81}, {0x92,0x4f}, {0x93,0xdc}, {0x94,0x22}, {0x95,0x2a}, {0x96,0x90}, {0x97,0x88}, {0x98,0x46}, {0x99,0xee}, {0x9a,0xb8}, {0x9b,0x14}, {0x9c,0xde}, {0x9d,0x5e}, {0x9e,0x0b}, {0x9f,0xdb},
            {0xa0,0xe0}, {0xa1,0x32}, {0xa2,0x3a}, {0xa3,0x0a}, {0xa4,0x49}, {0xa5,0x06}, {0xa6,0x24}, {0xa7,0x5c}, {0xa8,0xc2}, {0xa9,0xd3}, {0xaa,0xac}, {0xab,0x62}, {0xac,0x91}, {0xad,0x95}, {0xae,0xe4}, {0xaf,0x79},
            {0xb0,0xe7}, {0xb1,0xc8}, {0xb2,0x37}, {0xb3,0x6d}, {0xb4,0x8d}, {0xb5,0xd5}, {0xb6,0x4e}, {0xb7,0xa9}, {0xb8,0x6c}, {0xb9,0x56}, {0xba,0xf4}, {0xbb,0xea}, {0xbc,0x65}, {0xbd,0x7a}, {0xbe,0xae}, {0xbf,0x08},
            {0xc0,0xba}, {0xc1,0x78}, {0xc2,0x25}, {0xc3,0x2e}, {0xc4,0x1c}, {0xc5,0xa6}, {0xc6,0xb4}, {0xc7,0xc6}, {0xc8,0xe8}, {0xc9,0xdd}, {0xca,0x74}, {0xcb,0x1f}, {0xcc,0x4b}, {0xcd,0xbd}, {0xce,0x8b}, {0xcf,0x8a},
            {0xd0,0x70}, {0xd1,0x3e}, {0xd2,0xb5}, {0xd3,0x66}, {0xd4,0x48}, {0xd5,0x03}, {0xd6,0xf6}, {0xd7,0x0e}, {0xd8,0x61}, {0xd9,0x35}, {0xda,0x57}, {0xdb,0xb9}, {0xdc,0x86}, {0xdd,0xc1}, {0xde,0x1d}, {0xdf,0x9e},
            {0xe0,0xe1}, {0xe1,0xf8}, {0xe2,0x98}, {0xe3,0x11}, {0xe4,0x69}, {0xe5,0xd9}, {0xe6,0x8e}, {0xe7,0x94}, {0xe8,0x9b}, {0xe9,0x1e}, {0xea,0x87}, {0xeb,0xe9}, {0xec,0xce}, {0xed,0x55}, {0xee,0x28}, {0xef,0xdf},
            {0xf0,0x8c}, {0xf1,0xa1}, {0xf2,0x89}, {0xf3,0x0d}, {0xf4,0xbf}, {0xf5,0xe6}, {0xf6,0x42}, {0xf7,0x68}, {0xf8,0x41}, {0xf9,0x99}, {0xfa,0x2d}, {0xfb,0x0f}, {0xfc,0xb0}, {0xfd,0x54}, {0xfe,0xbb}, {0xff,0x16}
    };





/*

    return value: none
    parameters:
      const unsigned char array containing the input block to copy onto the state
      2d unsigned char array containing the state table

    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
    static void copyInputToState(const unsigned char [16], unsigned char [4][4]);



/*

   return value: none
   parameters:
     2d unsigned char vector that will hold the blocks of input data
     unsigned char vector that contains the input data

   description:
     takes the data from the input vector and splits it into blocks of 16 bytes each as a 16 element vector within the 2d vector
     each 16 element vector represents a block of 128 bits
*/
    static void splitInputIntoBlocks(unsigned char [][16], uint16_t , const vector<unsigned char> &);




/*

    return value: none
    parameters:
      unsigned char vector containing the input data that needs to be padded


    description:
      This function will pad the input using the PKCS# 7 and change the inputLength variable to reflect the
      new length after padding.
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes are added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block is added with 16 bytes of 16's.

*/
    static void padInput(vector<unsigned char > &);

/*

    return value: none
    parameters: none


    description:
      This function takes the state that you pass in and will perform the SubBytes transformation on the table.
      This will substitute each of the bytes in the table according to the method that NIST specifies.
      The pre-made sBox is passed in instead of calculating the substitution in the function to speed up the encryption process.
*/
    void SubBytes();


/*
    return value: none
    parameters: none


    description:
      This function takes the state and shifts the rows within it according to the schema identified by NIST.
      The top row will not be shifted.
      The second row will be shifted to the left 1 space.
      The third row will be shifted to the left 2 spaces.
      The fourth row will be shifted to the left 3 spaces.

*/
    void ShiftRows();




/*
    return value: none
    parameters: none


    description:
      This function operates on each column treating each column in the state as a 4-term polynomial over GF(2^8).
      The columns are multiplied modulo (x^4) + 1 with a fixed polynomial defined by NIST.
*/
    void MixColumns();




/*
    return value: none
    parameters:
        unsigned integer containing the current round number

    description:
      This function will get the round key and perform a bitwise XOR on the state, essentially performing
      an addition within a finite Field.
*/
    void AddRoundKey(const uint8_t &);



//  key schedule containing 4-byte words that will be generated in the KeyExpansion function
//  each row represents a 4-byte word
    word * keySched = nullptr;



/*
    return value: none
    parameters: none

    description:
      This function takes the original cipher key passed in for encryption and the empty key schedule array and
      performs the KeyExpansion operation to generate the round keys that will be needed for the AddRoundKey transformation
      and puts them into the keySched array.
*/
    void KeyExpansion();





/*
    return value: none
    parameters:
        unsigned char vector containing the previously encrypted data (inputVector from encrypt function should be only parameter passed in)
        unsigned 8-bit integer containing the current round number
        IV object containing IV to xor with in first round, will not be used after first round.

    description:
      This function will xor the state with the IV object in the first round,
      or will find the previous ciphertext block and xor the state with it for every other round.

      This implements the cipher block chaining mode of operation.
*/
    void xorBlock(const vector<unsigned char> &, const uint8_t &, const IV &);












    /* EXCEPTION CLASSES */

    class InvalidKeySize
    {
        public:
            explicit InvalidKeySize(uint16_t size){}
    };



};


#endif //COSC370_PROJECT_AesEncryptObj_H
