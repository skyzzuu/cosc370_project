/*
AESEncryptObj.cpp
Joshua Cobb, Luke Engle, Everett Gally
*/

#include "AESEncryptObj.h"

using namespace std;



AesEncryptObj::AesEncryptObj(uint16_t keysize)
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

//      ppendix A of AES spec
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
  Assumes AES-128
*/
AesEncryptObj::AesEncryptObj()
{
    keySize = 128;
    nK = 4;
    nR = 10;

    key = new word[nK];

    numWordsInKeySched = nB * (nR + 1);
    keySched = new word[numWordsInKeySched];
}



AesEncryptObj::~AesEncryptObj()
{
    delete[] key;
    delete[] keySched;
    delete[] roundConstants;
}



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
    Takes the bytes from the input data given, uses the bytes from the key given, and returns the encrypted version
    of the input data in the form of an unsigned char vector.
    Encrypts using the cipher block chaining mode of operation.
*/
vector<unsigned char> AesEncryptObj::encrypt(const unsigned char * data, uint64_t inputlength, const unsigned char * ciphKey,
                                             const unsigned char * ivData, uint64_t ivLength)
{

//  make iv object from data and length given
    IV iv(ivData, ivLength);

//  call main encrypt function with previously made iv object and return the result
    return encrypt(data, inputlength, ciphKey, iv);
}



/*
  return value: unsigned char vector containing encrypted bytes
  parameters:
    const unsigned char * containing the bytes to encrypt
    unsigned integer representing how many bytes are in the input
    const unsigned char * containing the encryption key to use
    IV object containing IV to use

  description:
    Takes the bytes from the input data given, uses the bytes from the key given, and returns the encrypted version
    of the input data in the form of an unsigned char vector.
    Encrypts using the cipher block chaining mode of operation.

*/
vector<unsigned char> AesEncryptObj::encrypt(const unsigned char * data, uint64_t inputlength, const unsigned char * ciphKey, const IV & iv)
{
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

//  pad up to nearest block (or add full block)
    padInput(inputVector);

//  total number of blocks in padded input
    uint16_t numBlocks = inputVector.size() / 16;

//  make a 2d array with the number of rows set to however many blocks there are in the padded input
    unsigned char blockArray[numBlocks][16];

//  take the blocks from the inputVector and copy the blocks into the blockArray
    splitInputIntoBlocks(blockArray, numBlocks, inputVector);

//  inputVector will get the encrypted data added to it later
    inputVector.clear();

//  generate round keys from cipher key
    KeyExpansion();

    for(uint64_t i = 0; i < numBlocks; i++)
    {
        copyInputToState(blockArray[i], state);

//      xor state with the iv, or previous ciphertext block if not the first round
        xorBlock(inputVector, i, iv);

        AddRoundKey(0);

        for (uint8_t roundNum = 1; roundNum < nR; roundNum++)
        {
            SubBytes();
            ShiftRows();
            MixColumns();
            AddRoundKey(roundNum);
        }

        SubBytes();
        ShiftRows();
        AddRoundKey(nR);

        for(uint8_t row = 0; row < 4; row++)
        {
            for( auto & column : state)
            {
                inputVector.push_back(column[row]);
            }
        }
    }

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

//  if the input's length is not a multiple of 128 bits (16 bytes), pad the partial block
    if((inputVector.size() % 16) != 0)
    {

//      how many full blocks there are
        uint16_t fullBlocks = inputVector.size() / 16;

//      represents both how many bytes need to be filled in the partial block and what number will be used to pad
        uint8_t padding = 16 - (inputVector.size() - (fullBlocks * 16));

//      for each byte that needs to be padded, add the padding byte to the inputVector
        for(uint8_t i = 0; i < padding; i++)
        {
            inputVector.push_back(padding);
        }
    }

//  if the input's length is a multiple of 128 bits (16 bytes), add block of just padding (16 bytes containing 16)
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
     2d array that will contain the individual blocks in a separated format
     unsigned integer representing how many blocks are in the input data
     unsigned char vector that contains the input data

   description:
     takes the data from the input vector and splits it into blocks of 16 bytes each as a 16 element vector within the 2d vector
     each 16 element vector represents a block of 128 bits
*/
void AesEncryptObj::splitInputIntoBlocks(unsigned char blockArray[][16], uint16_t numBlocks, const vector<unsigned char> & inputVector)
{
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
      2d unsigned char array containing the state table

    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
void AesEncryptObj::copyInputToState(const unsigned char inputBlock[16] , unsigned char stateTable[4][4] )
{

//  for each row of the state
    for(uint8_t row = 0; row < 4; row++)
    {

//      for each column of the state
        for(uint8_t column = 0; column < 4; column++)
        {
//          formula provided by NIST in AES specification
//          copies byte 0 to top left spot, byte 4 to the right of that, etc
            stateTable[row][column] = inputBlock[row + (4 * column)];
        }
    }
}



/*
    return value: none
    parameters: none

    description:
      This function takes the state and will perform the SubBytes transformation using the sBox .
      This will substitute each of the bytes in the table according to the method that NIST specifies.
      The pre-made sBox is used instead of calculating the substitution in the function to speed up the encryption process.
*/
void AesEncryptObj::SubBytes()
{
    for(auto & row : state)
    {
        for(unsigned char & column : row)
        {
//          set value of byte to the mapped value in the sBox
//          passes in byte as key to find function, then fetches the value from the iterator using "second"
            column = sBox.find(column)->second;
        }

    }
}



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
void AesEncryptObj::ShiftRows()
{
    for(uint8_t wordCounter = 0; wordCounter < 4; wordCounter++)
    {
        word stateWord;
        stateWord = state[wordCounter];

        for(uint8_t rotateCount = 0; rotateCount < wordCounter; rotateCount++)
        {
            stateWord.leftRotate();
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
void AesEncryptObj::MixColumns()
{

//  constants of a(x) defined in AES spec 5.1.3
//  none of the 0x01's are used since multiplying by 0x01 gets the same result as doing nothing
    const byte firstConst = 0x03;
    const byte fourthConst = 0x02;

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

//              finite field multiply with 0x02
                firstParen.galoisMultiply(fourthConst);

//              finite field multiply with 0x03
                secondParen.galoisMultiply(firstConst);
            }

            else if(row == 1)
            {

//              finite field multiply with 0x02
                secondParen.galoisMultiply(fourthConst);

//              finite field multiply with 0x03
                thirdParen.galoisMultiply(firstConst);
            }

            else if(row == 2)
            {

//              finite field multiply with 0x02
                thirdParen.galoisMultiply(fourthConst);

//              finite field multiply with 0x03
                fourthParen.galoisMultiply(firstConst);
            }

            else
            {

//              finite field multiply with 0x03
                firstParen.galoisMultiply(firstConst);

//              finite field multiply with 0x02
                fourthParen.galoisMultiply(fourthConst);
            }

//          xor of all the bytes
            retByte = firstParen + secondParen + thirdParen + fourthParen;

//          set the matching element of the column to the final value
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
      This function will get the round key and perform a bitwise XOR on the state, essentially performing
      an addition within a finite Field.
*/
void AesEncryptObj::AddRoundKey( const uint8_t & round )
{
    for(uint8_t column = 0; column < 4; column++)
    {
        for(uint8_t row = 0; row < 4; row++)
        {

//          make each byte in state the result of xor with byte from round key
            state[row][column] = state[row][column] ^ ((keySched[(round * nB) + column])[row])->rawData();
        }
    }
}



/*
    return value: none
    parameters: none

    description:
      This function takes the original cipher key passed in for encryption and the empty key schedule array and
      performs the KeyExpansion operation to generate the round keys that will be needed for the AddRoundKey transformation
      and puts them into the keySched array.
*/
void AesEncryptObj::KeyExpansion()
{

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

//          then sub each of the bytes according to sBox
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



/*
    return value: none
    parameters:
        unsigned char vector containing the previously encrypted data (inputVector from encrypt function should be only parameter passed in)
        unsigned 8-bit integer containing the current round number

    description:
      This function will xor the state with the IV object passed in, in the first round,
      or will find the previous ciphertext block and xor the state with it for every other round.
      This implements the cipher block chaining mode of operation.
*/
void AesEncryptObj::xorBlock(const vector<unsigned char> & inputVector, const uint8_t & blockNumber, const IV & iv)
{

//  for first block, state is xor'd with the IV
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
        uint64_t curByte = (blockNumber - 1) * 16;

//      for each column of the state
        for(uint8_t column = 0; column < 4; column++)
        {

//          for each row of the current column
            for(uint8_t row = 0; row < 4; row++)
            {

//              xor each byte of the state with corresponding byte in the last ciphertext block
                state[row][column] = state[row][column] ^ inputVector.at(curByte);
                curByte++;
            }
        }
    }
}
