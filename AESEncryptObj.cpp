

#include "AESEncryptObj.h"
#include <cstdint>
#include <cmath>



AesEncryptObj::AesEncryptObj() {

}


AesEncryptObj::~AesEncryptObj() {

}



vector<unsigned char> AesEncryptObj::encrypt(const unsigned char * data, uint16_t inputLength, const unsigned char key[16])
{
//    unsigned char vector containing all the input data
    vector<unsigned char > inputVector;
    inputVector.clear();

//    copy all bytes from original input into vector
    for(uint16_t i = 0; i < inputLength; i++)
    {
        inputVector.push_back(data[i]);
    }

    padInput(inputVector);



    unsigned char blockVector[16][inputVector.size() / 16];


    ~inputVector();


    return inputVector;
}



/*

    return value: none
    parameters:
      unsigned char vector containing the input data that needs to be padded
      uint16_t representing how many bytes are in the input


    description:
      This function will pad the input using the PKCS# 7 and change the inLen variable to reflect the
      new length after padding.
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes are added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block is added with 16 bytes of 16's
*/
void AesEncryptObj::padInput(vector<unsigned char> & inputVector)
{

//    if the input's length is not a multiple of 128 bits (16 bytes), pad the partial block
    if((inputVector.size() % 16) != 0)
    {

//        how many full blocks there are
        uint16_t fullBlocks = inputVector.size() / 16;

//        represents both how many bytes need to be filled in the partial block and what number will be used to pad
        uint8_t padding = 16 - (inputVector.size() - (fullBlocks * 16));

//        for each byte that needs to be padded, add the padding byte to the inputVector
        for(uint8_t i = 0; i < padding; i++)
        {
            inputVector.push_back(padding);
        }

    }

//    if the input's length is a multiple of 128 bits (16 bytes), add block of just padding (16 bytes containing 16)
    else
    {
        for(uint8_t i = 0; i < 16; i++)
        {
            inputVector.push_back(16);
        }
    }
}



/*

   return value: none
   parameters:
     2d unsigned char vector that will hold the blocks of input data
     unsigned char vector that contains the input data

   description:
     takes the data from the input vector and splits it into blocks of 16 bytes each as a 16 element vector within the 2d vector
     each 16 element vector represents a block of 128 bits
*/
void AesEncryptObj::splitStateIntoBlocks(vector <vector<unsigned char>> & blockVector, const vector<unsigned char> & inputVector)
{
    blockVector.clear();


}