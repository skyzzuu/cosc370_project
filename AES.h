//
// Created by guy on 3/29/21.
//

#ifndef COSC370_PROJECT_AESOBJ_H
#define COSC370_PROJECT_AESOBJ_H

#include <unordered_map>


class AesObj {
    AesObj();
    ~AesObj();

//    state table
    unsigned char state[4][4] = {0};

//    original input data
    unsigned char * input = nullptr;

//    how many bytes are in the input
    int inputLength = 0;


//    128 bit key used to encrypt the data
    unsigned char key[16] = {0};



    const unordered_map<int, int> sBox = {
            {0x00, 0x63}, {0x01, 0x7c},{0x02, 0x77}, {0x03, 0x7b},{0x04, 0xf2}, {0x05, 0x6b},{0x06, 0x6f}, {0x07, 0xc5}, {0x08, 0x30}, {0x09, 0x01},
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },
            { , }, { , }, { , }, { , },  { , }, { , }, { , }, { , }, { , }, { , },



    };

/*
    return value: unsigned char array containing encrypted bytes
    parameters:
      unsigned char * containing the bytes to encrypt
      int representing how many bytes are in the input
      unsigned char * containing the encryption key to use

    description:
      takes the bytes from the input date given, uses the bytes from the key given, and returns the encrypted version
      of the input data.

 */
    unsigned char * encrypt(unsigned char * , int, unsigned char *);

/*

    return value: none
    parameters:
      unsigned char * containing the input
      int representing how many bytes are in the input
      unsigned char [] containing the state table

    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
    void copyInputToState(unsigned char * , int, unsigned  char []);


/*

    return value: none
    parameters:
      unsigned char * containing the input data that needs to be padded
      int representing how many bytes are in the input


    description:
      This function will pad the input using the PKCS# 7
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes is added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block is added with 16 bytes of 16's
      so the decrypting algorithm can determine if padding occurred
*/
    void padInput(unsigned char *, int);






};


#endif //COSC370_PROJECT_AESOBJ_H
