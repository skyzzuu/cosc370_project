/*
AESDecryptObj.cpp
Joshua Cobb, Luke Engle, Everett Gally
*/

#include "AESDecryptObj.h"

using namespace std;



AESDecryptObj::AESDecryptObj(uint16_t keysize)
{
    keySize = keysize;

//  AES-128
    if(keySize == 128)
    {
        nK = 4;
        nR = 10;

        numRoundConstants = 10;
        roundConstants = new word[numRoundConstants];

//      appendix A of AES spec
        *((roundConstants[0]).operator[](0)) = 1;
        *((roundConstants[1]).operator[](0)) = 2;
        *((roundConstants[2]).operator[](0)) = 4;
        *((roundConstants[3]).operator[](0)) = 8;
        *((roundConstants[4]).operator[](0)) = 16;
        *((roundConstants[5]).operator[](0)) = 32;
        *((roundConstants[6]).operator[](0)) = 64;
        *((roundConstants[7]).operator[](0)) = 128;
        *((roundConstants[8]).operator[](0)) = 27;
        *((roundConstants[9]).operator[](0)) = 54;
    }

//  AES-192
    else if (keySize == 192)
    {
        nK = 6;
        nR = 12;

        numRoundConstants = 8;
        roundConstants = new word[numRoundConstants];

//      appendix A of AES spec
        *((roundConstants[0]).operator[](0)) = 1;
        *((roundConstants[1]).operator[](0)) = 2;
        *((roundConstants[2]).operator[](0)) = 4;
        *((roundConstants[3]).operator[](0)) = 8;
        *((roundConstants[4]).operator[](0)) = 16;
        *((roundConstants[5]).operator[](0)) = 32;
        *((roundConstants[6]).operator[](0)) = 64;
        *((roundConstants[7]).operator[](0)) = 128;
    }

//  AES-256
    else if(keySize == 256)
    {
        nK = 8;
        nR = 14;

        numRoundConstants = 7;
        roundConstants = new word[numRoundConstants];

//      appendix A of AES spec
        *((roundConstants[0]).operator[](0)) = 1;
        *((roundConstants[1]).operator[](0)) = 2;
        *((roundConstants[2]).operator[](0)) = 4;
        *((roundConstants[3]).operator[](0)) = 8;
        *((roundConstants[4]).operator[](0)) = 16;
        *((roundConstants[5]).operator[](0)) = 32;
        *((roundConstants[6]).operator[](0)) = 64;
    }

//  invalid key size
    else
    {
        throw InvalidKeySize(keysize);
    }

//  make key a word array of nK words
    key = new word[nK];

    numWordsInKeySched = nB * (nR + 1);
    keySched = new word[numWordsInKeySched];
}



/*
  Default constructor
  assumes AES-128
 */
AESDecryptObj::AESDecryptObj()  {
    keySize = 128;
    nK = 4;
    nR = 10;

    key = new word[nK];

    numWordsInKeySched = nB * (nR + 1);
    keySched = new word[numWordsInKeySched];
}



AESDecryptObj::~AESDecryptObj() {
    delete [] key;
    delete[] keySched;
    delete[] roundConstants;
}



vector<unsigned char> AESDecryptObj::decrypt(const unsigned char * data, const uint16_t & inputLength, const unsigned char * ciphKey, const unsigned char * ivData, const uint64_t & ivLength)
{
    IV iv(ivData, ivLength);
    return decrypt(data, inputLength, ciphKey, iv);
}



vector<unsigned char > AESDecryptObj::decrypt(const unsigned char * data, const uint16_t & inputlength, const unsigned char * ciphKey,
                                       const IV & iv) {
    inputLength = inputlength;

//  unsigned char vector containing all the input data
    vector<unsigned char > inputVector;
    inputVector.clear();

//  copy all bytes from original input into vector
    for(uint64_t i = 0; i < inputLength; i++)
    {
        inputVector.push_back(data[i]);
    }

//  read all words from cipher key given into member variable key
    for(uint64_t i = 0; i < nK; i++)
    {
        key[i] = & ciphKey[i * 4];
    }

//  total number of blocks in padded input
    uint16_t numBlocks = inputVector.size() / 16;

//  make a 2d array with the number of rows set to however many blocks there are in the padded input
    unsigned char blockArray[numBlocks][16];

//  take the blocks from the inputVector and copy the blocks into the blockArray
    splitInputIntoBlocks(blockArray, numBlocks, inputVector);

//  inputVector will get the encrypted data added to it later
    inputVector.clear();

    vector<vector<unsigned char>> returnVector(numBlocks);
    KeyExpansion();

//  for each block starting from right
    for(int blockNum = numBlocks - 1; blockNum >= 0; blockNum--)
    {
        copyInputToState(blockArray[blockNum]);
        AddRoundKey(nR);

//      for each round
        for (int roundNum = nR-1; roundNum > 0; roundNum--)
        {

            InvShiftRows();
            InvSubBytes();
            AddRoundKey(roundNum);
            InvMixColumns();

        }

        InvShiftRows();
        InvSubBytes();
        AddRoundKey(0);

//      after all decryption operations, xor with ciphertext block to the left
        xorBlock(blockArray[blockNum-1], blockNum, iv);

        for(uint8_t row = 0; row < 4; row++)
        {
            for( auto & column : state)
            {
//              inputVector.push_back(column[row]);
                returnVector[blockNum].push_back(column[row]);
            }
        }
    }

//  for each block of decrypted data
    for( const vector<unsigned char> & block : returnVector)
    {

//      for each byte in the block
        for( const unsigned char & byteValue : block)
        {
//          add to inputVector
            inputVector.push_back(byteValue);
        }
    }

//  remove padding from last block of decrypted data, (or entire block if the entire last block is just padding)
    removePadding(inputVector);

    return inputVector;
}



void AESDecryptObj::splitInputIntoBlocks(unsigned char blockArray[][16] , uint16_t numBlocks, const vector<unsigned char> & inputVector) {
//  for each block that needs to be created
    for(uint16_t curBlock = 0; curBlock < numBlocks; curBlock++)
    {
//      index position where current block begins
        uint16_t startOfBlock = curBlock * 16;

//      index position where current block ends
        uint16_t endOfBlock = ((curBlock + 1) * 16);

//      current index within the block being added
        uint16_t columnIndex = 0;

//      for each byte in the current block, add the byte to the 2d array
        for(uint16_t curIndex = startOfBlock; curIndex < endOfBlock; curIndex++)
        {
            blockArray[curBlock][columnIndex] = inputVector[curIndex];
            columnIndex++;
        }
    }
}



/*
    return value: none
    parameters:
      const unsigned char array containing the input block to copy onto the state

    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
void AESDecryptObj::copyInputToState(const unsigned char inputBlock[16])
{
//  for each row of the state
    for(uint8_t row = 0; row < 4; row++) {

//      for each column of the state
        for (uint8_t column = 0; column < 4; column++) {

//          formula provided by NIST in AES specification
//          copies byte 0 to top left spot, byte 4 to the right of that, etc
            state[row][column] = inputBlock[row + (4 * column)];
        }
    }
}



/*
    return value: none
    parameters: unsigned char vector containing data that needs to have padding removed

    description:
      This function will remove the padding from the input using the PKCS# 7
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes is added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block exists with 16 bytes of 16's
*/
void AESDecryptObj::removePadding(vector<unsigned char> & input) {

//  the last number of the last block will contain the number of bytes of padding that were added
    const unsigned char lastNum = input.at(input.size()-1);

    const uint32_t startSize = input.size();

    for(uint32_t i = startSize - 1; i > (startSize - 1 - lastNum); i--)
    {

//      if the byte matches what it should be
        if(input.at(i) == lastNum)
        {

//          remove the byte from the end
            input.erase(input.begin() + i);
        }


//      byte does not have value that it should have, throw error
//      all bytes of padding should have the same number
        else
        {
            throw RemovePaddingError();
        }
    }
}



/*
    return value: none
    parameters: none

    description:
      This function takes the state that you pass in and will perform the InvSubBytes transformation on the table.
      This will substitute each of the bytes in the table according to the method that NIST specifies.
      The pre-made InvSBox is passed in instead of calculating the substitution in the function to speed up the decryption process.
*/
void AESDecryptObj::InvSubBytes() {
    for(auto & row : state)
    {
        for(unsigned char & column : row)
        {
//          set value of byte to the mapped value in the InvSBox
//          passes in byte as key to find function, then fetches the value from the iterator using "second"
            column = InvSBox.find(column)->second;
        }
    }
}



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
void AESDecryptObj::InvShiftRows() {
    for(uint8_t wordCounter = 0; wordCounter < 4; wordCounter++)
    {
        word stateWord;
        stateWord = state[wordCounter];

        for(uint8_t rotateCount = 0; rotateCount < wordCounter; rotateCount++)
        {
            stateWord.rightRotate();
        }

        for(uint8_t byteIndex = 0; byteIndex < 4; byteIndex++)
        {
            state[wordCounter][byteIndex] = (stateWord[byteIndex])->rawData();
        }
    }
}



/*
    return value: none
    parameters: none

    description:
      This function operates on each column treating each column in the state as a 4-term polynomial over GF(2^8).
      The columns are multiplied modulo (x^4) + 1 with a fixed polynomial defined by NIST.
*/
void AESDecryptObj::InvMixColumns() {
//  constants of a^-1(x) defined in AES spec 5.1.3
    const byte firstConst = 0x0b;
    const byte secondConst = 0x0d;
    const byte thirdConst = 0x09;
    const byte fourthConst = 0x0e;

    byte firstParen;
    byte secondParen;
    byte thirdParen;
    byte fourthParen;

    for(uint8_t column = 0; column < 4; column++)
    {

//      will be copied over into the column
        byte curColumn[4];

        for(uint8_t row = 0; row < 4; row++)
        {

//          will contain the final value for the byte, starts off as 0
            byte retByte;

//          set each byte object to corresponding byte in the state
            firstParen = state[0][column];
            secondParen = state[1][column];
            thirdParen = state[2][column];
            fourthParen = state[3][column];

//          all of the following multiplications are defined in AES spec 5.1.3
            if(row == 0)
            {

                firstParen.galoisMultiply(fourthConst);
                secondParen.galoisMultiply(firstConst);
                thirdParen.galoisMultiply(secondConst);
                fourthParen.galoisMultiply(thirdConst);

            }

            else if(row == 1)
            {

                firstParen.galoisMultiply(thirdConst);
                secondParen.galoisMultiply(fourthConst);
                thirdParen.galoisMultiply(firstConst);
                fourthParen.galoisMultiply(secondConst);

            }

            else if(row == 2)
            {


                firstParen.galoisMultiply(secondConst);
                secondParen.galoisMultiply(thirdConst);
                thirdParen.galoisMultiply(fourthConst);
                fourthParen.galoisMultiply(firstConst);

            }

            else
            {

                firstParen.galoisMultiply(firstConst);
                secondParen.galoisMultiply(secondConst);
                thirdParen.galoisMultiply(thirdConst);
                fourthParen.galoisMultiply(fourthConst);

            }

//          xor of all the bytes
            retByte = firstParen + secondParen + thirdParen + fourthParen;

//          set the matching element of the column to the final vaule
            curColumn[row] = retByte;

        }

//      for each row of the current column being worked on
        for(uint8_t row = 0; row < 4; row++)
        {

//          copy elements from the column into the column in the state
            state[row][column] = curColumn[row].rawData();
        }
    }
}



/*
    return value: none
    parameters:
        unsigned integer containing the current round number

    description:
      This function will take the round key passed in and perform a bitwise XOR on the state, essentially performing
      an addition within a Galois Field.
*/
void AESDecryptObj::AddRoundKey(const uint8_t & roundNum) {

//    if(roundNum == 0)
//    {
//        for(uint8_t column = 0; column < 4; column++)
//        {
//            for(uint8_t row = 0; row < 4; row++)
//            {
//
//                cout << std::dec << (int) numWordsInKeySched << " words in key schedule" << endl;
//
////            make each byte in state the result of xor with byte from round key
//                cout << "getting word at index position: " << std::dec << (int) (((nR + 1) * nB)-4+column) << endl;
//                cout << "getting byte " << std::dec << (int) row << " from that word" << endl << endl;
//                state[row][column] = state[row][column] ^ ((keySched[((nR + 1) * nB)-4+column])[row])->rawData();
//
//
//                cout << endl;
//
//            }
//        }
//    }

//    else
//    {
        for(uint8_t column = 0; column < 4; column++)
        {
            for(uint8_t row = 0; row < 4; row++)
            {

//                cout << "getting word at index position: " << std::dec << (int) ((roundNum * nB) + column) << endl;
//                cout << "getting byte " << std::dec << (int) row << " from that word" << endl << endl;


//            make each byte in state the result of xor with byte from round key
                state[row][column] = state[row][column] ^ ((keySched[(roundNum * nB) + column])[row])->rawData();



//                word temp = ((keySched[(roundNum * nB) + column]));

//                cout << temp << endl;

            }
        }
//    }

}



void AESDecryptObj::KeyExpansion() {

//  holds a temporary 4-byte word in a later loop, will hold the value of the previous word
    word temp;

//  index position
    uint8_t i = 0;

//  for each word of the cipher key
    while (i < nK)
    {
//      copy the current word of the cipher key into the beginning of the key schedule
        keySched[i] = key[i];

        i++;
    }

    i = nK;

//  while i less than (4 * ( number of rounds + 1 ) )
    while (i < (nB * (nR + 1)))
    {

//      copy the previous word into temp
        temp = keySched[i-1];

//      happens every nK words of the key
        if((i % nK) == 0)
        {

//          rotate temp
            temp.leftRotate();

//          then sub each of the bytes according to InvSBox
            temp.SubWord(sBox);

//          then xor each byte with word from round constants
            temp = temp ^ roundConstants[(i / nK) - 1];

        }

//      AES-256 and (i-4) is a multiple of nK
        else if (nK == 8 && (i-4) % nK == 0)
        {
            temp.SubWord(sBox);
        }

//      set current word of key schedule equal to XOR with word nK positions before and temp word
        keySched[i] = keySched[i - nK] ^ temp;

//      move one word (4 bytes) to the right
        i++;
    }
}



void AESDecryptObj::xorBlock(const unsigned char blockArray[16]  , const uint8_t & blockNumber, const IV & iv)
{
//  first block (last one to be decrypted) is xor'd with the iv
    if(blockNumber == 0)
    {

//      if iv is a full block (128 bits)
        if(iv.getLength() == 16)
        {

//          stores index position of current byte of the iv being used for xor
            uint64_t curByte = 0;

            for(uint8_t column = 0; column < 4; column++)
            {
                for(uint8_t row = 0; row < 4; row++)
                {

//                xor each byte of the state with corresponding byte of the iv
                  state[row][column] ^= iv.getData()[curByte];
                  curByte++;

                }
            }
        }

//      iv has incorrect length
        else
        {

            string errMsg = "IV has incorrect length, should be 16 bytes, actual length is " + to_string((long) iv.getLength());
            throw runtime_error(errMsg);

        }
    }


//  in every round except the first, the state is xor'd with the previous ciphertext block
    else
    {

//      index position of first byte of the last ciphertext block
        uint64_t curByte = 0;


//      for each column of the state
        for(uint8_t column = 0; column < 4; column++)
        {

//          for each row of the current column
            for(uint8_t row = 0; row < 4; row++)
            {

//              xor each byte of the state with corresponding byte in the last ciphertext block
//              state[row][column] = state[row][column] ^ inputVector.at(curByte);

                state[row][column] ^= blockArray[curByte];
                curByte++;
                
            }
        }
    }
}
