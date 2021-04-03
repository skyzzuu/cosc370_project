//
// Created by guy on 3/29/21.
//

#include "AES.h"
#include <cstdint>
#include <cmath>



AesObj::AesObj() {

}


AesObj::~AesObj() {

}



unsigned char * AesObj::encrypt(const unsigned char * data, uint16_t inputLength, const unsigned char key[16])
{

}



/*

    return value: none
    parameters:
      unsigned char * containing the input data that needs to be padded
      uint16_t representing how many bytes are in the input


    description:
      This function will pad the input using the PKCS# 7 and change the inputLength variable to reflect the
      new length after padding.
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes are added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block is added with 16 bytes of 16's
*/
void AesObj::padInput(unsigned char * input, uint16_t & inputLength)
{

//    if the input's length is not a multiple of 128 bits (16 bytes), pad the partial block
    if((inputLength % 16) != 0)
    {
//        how many full blocks there are
        uint16_t fullBlocks = inputLength / 16;

//        represents both how many bytes need to be filled in the partial block and what number will be used to pad
        uint16_t padding = 16 - (inputLength - (fullBlocks * 16));






    }

//    if the input's length is a multiple of 128 bits (16 bytes), add block of just padding
    else
    {

    }
}