

#include "AESDecryptObj.h"

using namespace std;



AESDecryptObj::AESDecryptObj(uint16_t keysize)
{
    keySize = keysize;






//    AES-128
    if(keySize == 128)
    {
        cout << "AES-128" << endl;
        nK = 4;
        nR = 10;

        numRoundConstants = 10;
        roundConstants = new word[numRoundConstants];

//        appendix A of AES spec
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

//    AES-192
    else if (keySize == 192)
    {
        nK = 6;
        nR = 12;


        numRoundConstants = 8;
        roundConstants = new word[numRoundConstants];

        //        appendix A of AES spec
        *((roundConstants[0]).operator[](0)) = 1;
        *((roundConstants[1]).operator[](0)) = 2;
        *((roundConstants[2]).operator[](0)) = 4;
        *((roundConstants[3]).operator[](0)) = 8;
        *((roundConstants[4]).operator[](0)) = 16;
        *((roundConstants[5]).operator[](0)) = 32;
        *((roundConstants[6]).operator[](0)) = 64;
        *((roundConstants[7]).operator[](0)) = 128;

    }

//    AES-256
    else if(keySize == 256)
    {
        nK = 8;
        nR = 14;


        numRoundConstants = 7;
        roundConstants = new word[numRoundConstants];

        //        appendix A of AES spec
        *((roundConstants[0]).operator[](0)) = 1;
        *((roundConstants[1]).operator[](0)) = 2;
        *((roundConstants[2]).operator[](0)) = 4;
        *((roundConstants[3]).operator[](0)) = 8;
        *((roundConstants[4]).operator[](0)) = 16;
        *((roundConstants[5]).operator[](0)) = 32;
        *((roundConstants[6]).operator[](0)) = 64;

    }

//    invalid key size
    else
    {
        throw InvalidKeySize(keysize);
    }


//    make key a word array of nK words
    key = new word[nK];

    numWordsInKeySched = nB * (nR + 1);
    keySched = new word[numWordsInKeySched];

}




/*
 * Default constructor
 * assumes AES-128
 *
 * */
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



vector<unsigned char > AESDecryptObj::decrypt(const unsigned char * data, const uint16_t & inputlength, const unsigned char * ciphKey,
                                       const unsigned char * iv, const uint64_t & ivLength) {

    inputLength = inputlength;

//    unsigned char vector containing all the input data
    vector<unsigned char > inputVector;
    inputVector.clear();

//    copy all bytes from original input into vector
    for(uint64_t i = 0; i < inputLength; i++)
    {
        inputVector.push_back(data[i]);
    }


//    read all words from cipher key given into member variable key
    for(uint64_t i = 0; i < nK; i++)
    {
        key[i] = & ciphKey[i * 4];
    }



    cout << "cipher key: ";
    for(uint8_t i = 0; i < nK; i++)
    {
        cout << key[i];
    }
    cout << endl << endl;






    cout << "Beginning plaintext: ";
    for(uint64_t i = 0; i < inputlength; i++)
    {
        cout << std::hex << (int) data[i];
    }
    cout << endl << endl;


    //    total number of blocks in padded input
    uint16_t numBlocks = inputVector.size() / 16;

//    make a 2d array with the number of rows set to however many blocks there are in the padded input
    unsigned char blockArray[numBlocks][16];

//    take the blocks from the inputVector and copy the blocks into the blockArray
    splitInputIntoBlocks(blockArray, numBlocks, inputVector);

//    inputVector will get the encrypted data added to it later
    inputVector.clear();




    KeyExpansion();



    cout << "BLOCKS: " << endl;
    for(uint64_t i = 0; i < numBlocks; i++)
    {
        copyInputToState(blockArray[i]);



        cout << "BEGINNING STATE" << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(const auto & column : state)
            {
                cout << std::hex << (int) column[row];
            }
        }
        cout << endl;

        AddRoundKey(0);


        for (uint8_t roundNum = 1; roundNum < nR; roundNum++)
        {

            cout << "\n\n***\nround " << (int) (roundNum) << "\n***\n\n";

            cout << (int) (roundNum) << " start: " << endl;
            for(uint8_t row = 0; row < 4; row++)
            {
                for(const auto & column : state)
                {
                    cout << std::hex << (int) column[row];
                }
            }
            cout << endl << endl;


            InvSubBytes();

            cout << (int) (roundNum) << " SubBytes(): " << endl;
            for(uint8_t row = 0; row < 4; row++)
            {
                for(const auto & column : state)
                {
                    cout << std::hex << (int) column[row];
                }
            }
            cout << endl << endl;


            InvShiftRows();

            cout << (int) (roundNum) << " ShiftRows(): " << endl;
            for(uint8_t row = 0; row < 4; row++)
            {
                for(const auto & column : state)
                {
                    cout << std::hex << (int) column[row];
                }
            }
            cout << endl << endl;


            InvMixColumns();

            cout << (int) (roundNum) << " MixColumns(): " << endl;
            for(uint8_t row = 0; row < 4; row++)
            {
                for(const auto & column : state)
                {
                    cout << std::hex << (int) column[row];
                }
            }
            cout << endl << endl;



            AddRoundKey(roundNum);


        }



        InvSubBytes();


        cout <<  "final SubBytes(): " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(const auto & column : state)
            {
                cout << std::hex << (int) column[row];
            }
        }
        cout << endl << endl;


        InvShiftRows();

        cout << "final ShiftRows(): " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(const auto & column : state)
            {
                cout << std::hex << (int) column[row];
            }
        }
        cout << endl << endl;


        AddRoundKey(nR);


        cout << "FINAL: " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(const auto & column : state)
            {
                cout << std::hex << (int) column[row];
            }
        }
        cout << endl << endl;








    }
    cout << endl;


    for(uint8_t row = 0; row < 4; row++)
    {
        for( auto & column : state)
        {
            inputVector.push_back(column[row]);
        }
    }







    return inputVector;


}



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
void AESDecryptObj::removePadding(vector<unsigned char> & input) {

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
//            set value of byte to the mapped value in the sBox
//            passes in byte as key to find function, then fetches the value from the iterator using "second"
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
            stateWord = stateWord.rightRotate();
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
    for(uint8_t column = 0; column < 4; column++)
    {
        for(uint8_t row = 0; row < 4; row++)
        {

//            make each byte in state the result of xor with byte from round key
            state[row][column] = state[row][column] ^ ((keySched[(round * nB) + column])[row])->rawData();

        }
    }
}