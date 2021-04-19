//
// Created by guy on 4/3/21.
//

#ifndef COSC370_PROJECT_AESDECRYPTOBJ_H
#define COSC370_PROJECT_AESDECRYPTOBJ_H


#include <unordered_map>
#include <cstdint>
#include <vector>
#include <exception>
#include "word.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "IV.h"
#include <string>


using namespace std;


class AESDecryptObj {
public:
    AESDecryptObj();
    ~AESDecryptObj();
    AESDecryptObj(uint16_t);



    /*
return value: unsigned char array containing decrypted bytes
parameters:
unsigned char * containing the bytes to decrypt
unsigned int representing how many bytes are in the input
unsigned char * containing the decryption key to use
unsigned char * pointing to IV to use for decryption
unsigned int containing number of bytes in IV

description:
takes the bytes from the input data given, uses the bytes from the key given, and returns the decrypted version
of the input data.

*/
    vector<unsigned char > decrypt(const unsigned char * , const uint16_t &, const unsigned char *, const IV &);






    /*
return value: unsigned char array containing decrypted bytes
parameters:
  unsigned char * containing the bytes to decrypt
  unsigned int representing how many bytes are in the input
  unsigned char * containing the decryption key to use
  unsigned char * pointing to IV to use for decryption
  unsigned int containing number of bytes in IV

description:
  takes the bytes from the input data given, uses the bytes from the key given, and returns the decrypted version
  of the input data.

*/
    vector<unsigned char > decrypt(const unsigned char * , const uint16_t &, const unsigned char *, const unsigned char *, const uint64_t &);





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


//    how many bytes are in the input
    uint16_t inputLength = 0;


    word * key = nullptr;


    const vector<uint8_t> mixColumnsIrreduce = {8, 4, 3, 1, 0};




    const unordered_map<uint8_t , uint8_t> InvSBox = {
            {0x00, 0x52}, {0x01, 0x09}, {0x02, 0x6a}, {0x03, 0xd5}, {0x04, 0x30}, {0x05, 0x36}, {0x06, 0xa5}, {0x07, 0x38}, {0x08, 0xbf}, {0x09, 0x40}, {0x0a, 0xa3}, {0x0b, 0x9e}, {0x0c, 0x81}, {0x0d, 0xf3}, {0x0e, 0xd7}, {0x0f, 0xfb},
            {0x10, 0x7c}, {0x11, 0xe3}, {0x12, 0x39}, {0x13, 0x82}, {0x14, 0x9b}, {0x15, 0x2f}, {0x16, 0xff}, {0x17, 0x87}, {0x18, 0x34}, {0x19, 0x8e}, {0x1a, 0x43}, {0x1b, 0x44}, {0x1c, 0xc4}, {0x1d, 0xde}, {0x1e, 0xe9}, {0x1f, 0xcb},
            {0x20, 0x54}, {0x21, 0x7b}, {0x22, 0x94}, {0x23, 0x32}, {0x24, 0xa6}, {0x25, 0xc2}, {0x26, 0x23}, {0x27, 0x3d}, {0x28, 0xee}, {0x29, 0x4c}, {0x2a, 0x95}, {0x2b, 0x0b}, {0x2c, 0x42}, {0x2d, 0xfa}, {0x2e, 0xc3}, {0x2f, 0x4e},
            {0x30, 0x08}, {0x31, 0x2e}, {0x32, 0xa1}, {0x33, 0x66}, {0x34, 0x28}, {0x35, 0xd9}, {0x36, 0x24}, {0x37, 0xb2}, {0x38, 0x76}, {0x39, 0x5b}, {0x3a, 0xa2}, {0x3b, 0x49}, {0x3c, 0x6d}, {0x3d, 0x8b}, {0x3e, 0xd1}, {0x3f, 0x25},
            {0x40, 0x72}, {0x41, 0xf8}, {0x42, 0xf6}, {0x43, 0x64}, {0x44, 0x86}, {0x45, 0x68}, {0x46, 0x98}, {0x47, 0x16}, {0x48, 0xd4}, {0x49, 0xa4}, {0x4a, 0x5c}, {0x4b, 0xcc}, {0x4c, 0x5d}, {0x4d, 0x65}, {0x4e, 0xb6}, {0x4f, 0x92},
            {0x50, 0x6c}, {0x51, 0x70}, {0x52, 0x48}, {0x53, 0x50}, {0x54, 0xfd}, {0x55, 0xed}, {0x56, 0xb9}, {0x57, 0xda}, {0x58, 0x5e}, {0x59, 0x15}, {0x5a, 0x46}, {0x5b, 0x57}, {0x5c, 0xa7}, {0x5d, 0x8d}, {0x5e, 0x9d}, {0x5f, 0x84},
            {0x60, 0x90}, {0x61, 0xd8}, {0x62, 0xab}, {0x63, 0x00}, {0x64, 0x8c}, {0x65, 0xbc}, {0x66, 0xd3}, {0x67, 0x0a}, {0x68, 0xf7}, {0x69, 0xe4}, {0x6a, 0x58}, {0x6b, 0x05}, {0x6c, 0xb8}, {0x6d, 0xb3}, {0x6e, 0x45}, {0x6f, 0x06},
            {0x70, 0xd0}, {0x71, 0x2c}, {0x72, 0x1e}, {0x73, 0x8f}, {0x74, 0xca}, {0x75, 0x3f}, {0x76, 0x0f}, {0x77, 0x02}, {0x78, 0xc1}, {0x79, 0xaf}, {0x7a, 0xbd}, {0x7b, 0x03}, {0x7c, 0x01}, {0x7d, 0x13}, {0x7e, 0x8a}, {0x7f, 0x6b},
            {0x80, 0x3a}, {0x81, 0x91}, {0x82, 0x11}, {0x83, 0x41}, {0x84, 0x4f}, {0x85, 0x67}, {0x86, 0xdc}, {0x87, 0xea}, {0x88, 0x97}, {0x89, 0xf2}, {0x8a, 0xcf}, {0x8b, 0xce}, {0x8c, 0xf0}, {0x8d, 0xb4}, {0x8e, 0xe6}, {0x8f, 0x73},
            {0x90, 0x96}, {0x91, 0xac}, {0x92, 0x74}, {0x93, 0x22}, {0x94, 0xe7}, {0x95, 0xad}, {0x96, 0x35}, {0x97, 0x85}, {0x98, 0xe2}, {0x99, 0xf9}, {0x9a, 0x37}, {0x9b, 0xe8}, {0x9c, 0x1c}, {0x9d, 0x75}, {0x9e, 0xdf}, {0x9f, 0x6e},
            {0xa0, 0x47}, {0xa1, 0xf1}, {0xa2, 0x1a}, {0xa3, 0x71}, {0xa4, 0x1d}, {0xa5, 0x29}, {0xa6, 0xc5}, {0xa7, 0x89}, {0xa8, 0x6f}, {0xa9, 0xb7}, {0xaa, 0x62}, {0xab, 0x0e}, {0xac, 0xaa}, {0xad, 0x18}, {0xae, 0xbe}, {0xaf, 0x1b},
            {0xb0, 0xfc}, {0xb1, 0x56}, {0xb2, 0x3e}, {0xb3, 0x4b}, {0xb4, 0xc6}, {0xb5, 0xd2}, {0xb6, 0x79}, {0xb7, 0x20}, {0xb8, 0x9a}, {0xb9, 0xdb}, {0xba, 0xc0}, {0xbb, 0xfe}, {0xbc, 0x78}, {0xbd, 0xcd}, {0xbe, 0x5a}, {0xbf, 0xf4},
            {0xc0, 0x1f}, {0xc1, 0xdd}, {0xc2, 0xa8}, {0xc3, 0x33}, {0xc4, 0x88}, {0xc5, 0x07}, {0xc6, 0xc7}, {0xc7, 0x31}, {0xc8, 0xb1}, {0xc9, 0x12}, {0xca, 0x10}, {0xcb, 0x59}, {0xcc, 0x27}, {0xcd, 0x80}, {0xce, 0xec}, {0xcf, 0x5f},
            {0xd0, 0x60}, {0xd1, 0x51}, {0xd2, 0x7f}, {0xd3, 0xa9}, {0xd4, 0x19}, {0xd5, 0xb5}, {0xd6, 0x4a}, {0xd7, 0x0d}, {0xd8, 0x2d}, {0xd9, 0xe5}, {0xda, 0x7a}, {0xdb, 0x9f}, {0xdc, 0x93}, {0xdd, 0xc9}, {0xde, 0x9c}, {0xdf, 0xef},
            {0xe0, 0xa0}, {0xe1, 0xe0}, {0xe2, 0x3b}, {0xe3, 0x4d}, {0xe4, 0xae}, {0xe5, 0x2a}, {0xe6, 0xf5}, {0xe7, 0xb0}, {0xe8, 0xc8}, {0xe9, 0xeb}, {0xea, 0xbb}, {0xeb, 0x3c}, {0xec, 0x83}, {0xed, 0x53}, {0xee, 0x99}, {0xef, 0x61},
            {0xf0, 0x17}, {0xf1, 0x2b}, {0xf2, 0x04}, {0xf3, 0x7e}, {0xf4, 0xba}, {0xf5, 0x77}, {0xf6, 0xd6}, {0xf7, 0x26}, {0xf8, 0xe1}, {0xf9, 0x69}, {0xfa, 0x14}, {0xfb, 0x63}, {0xfc, 0x55}, {0xfd, 0x21}, {0xfe, 0x0c}, {0xff, 0x7d}
    };



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


    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
    void copyInputToState(const unsigned char [16]);




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
 *
 *
 *

    return value: none
    parameters: unsigned char vector containing data that needs to have padding removed


    description:
      This function will remove the padding from the input using the PKCS# 7
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes is added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block exists with 16 bytes of 16's
*/

    void removePadding(vector<unsigned char > &);



/*

    return value: none
    parameters: none


    description:
      This function takes the state that you pass in and will perform the InvSubBytes transformation on the table.
      This will substitute each of the bytes in the table according to the method that NIST specifies.
      The pre-made InvSBox is passed in instead of calculating the substitution in the function to speed up the decryption process.
*/
    void InvSubBytes();



/*
    return value: none
    parameters: none


    description:
      This function takes the state and shifts the rows within it according to the schema identified by NIST.
      The top row will not be shifted.
      The second row will be shifted to the right 1 space.
      The third row will be shifted to the right 2 spaces.
      The fourth row will be shifted to the right 3 spaces.

*/
    void InvShiftRows();



/*
    return value: none
    parameters: none


    description:
      This function operates on each column treating each column in the state as a 4-term polynomial over GF(2^8).
      The columns are multiplied modulo (x^4) + 1 with a fixed polynomial defined by NIST.
*/
    void InvMixColumns();




/*
    return value: none
    parameters:
        unsigned integer containing the current round number


    description:
      This function will take the round key passed in and perform a bitwise XOR on the state, essentially performing
      an addition within a Galois Field.
*/
    void AddRoundKey(const uint8_t &);



//  key schedule containing 44 4-byte words that will be generated in the KeyExpansion function
//  each row represents a 4-byte word
    word * keySched = nullptr;

/*
    return value: none
    parameters: none

    description:
      This function takes the original cipher key passed in for decryption and the empty key schedule array and
      performs the KeyExpansion operation to generate the round keys that will be needed for the AddRoundKey transformation
      and puts them into the keySched array.
*/
    void KeyExpansion();





    /*
    return value: none
    parameters:
        2d unsigned char array containing all the ciphertext blocks (should be blockArray passed in)
        unsigned 8-bit integer containing the current round number

    description:
      This function will xor the state with the IV object passed in, in the first round,
      or will find the previous ciphertext block and xor the state with it for every other round.

      will xor state with ciphertext block before it after decrypting to restore to original plaintext.

      This implements the cipher block chaining mode of operation.
*/
    void xorBlock(const unsigned char[16] , const uint8_t & , const IV & );





    /* EXCEPTION CLASSES */

    class InvalidKeySize
    {
    public:
        explicit InvalidKeySize(uint16_t size){}
    };



    class RemovePaddingError
    {
    public:
        explicit RemovePaddingError(){}
    };



};


#endif //COSC370_PROJECT_AESDECRYPTOBJ_H
