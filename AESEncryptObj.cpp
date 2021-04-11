

#include "AESEncryptObj.h"
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

/*
    Sodium is an encryption library. We use it to populate initialization
    vectors with cryptographically secure random bytes. See function
    generateIV().
*/
//#include <sodium.h>




AesEncryptObj::AesEncryptObj(uint16_t keysize)
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
AesEncryptObj::AesEncryptObj()  {
    keySize = 128;
    nK = 4;
    nR = 10;

    key = new word[nK];


    numWordsInKeySched = nB * (nR + 1);
    keySched = new word[numWordsInKeySched];
}


AesEncryptObj::~AesEncryptObj() {
    delete[] key;
    delete[] keySched;
    delete[] roundConstants;
}



vector<unsigned char> AesEncryptObj::encrypt(const unsigned char * data, uint64_t inputlength, const unsigned char * ciphKey)
{

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

//    pad up to nearest block (or add full block)
//    padInput(inputVector);


    cout << "After Padding: ";
    for(uint64_t i = 0; i < inputVector.size(); i++)
    {
        cout << std::hex << (int) inputVector[i];
    }
    cout << endl << endl;


//    total number of blocks in padded input
    uint16_t numBlocks = inputVector.size() / 16;

//    make a 2d array with the number of rows set to however many blocks there are in the padded input
    unsigned char blockArray[numBlocks][16];

//    take the blocks from the inputVector and copy the blocks into the blockArray
    splitInputIntoBlocks(blockArray, numBlocks, inputVector);

//    inputVector not needed after this point
    inputVector.clear();


    cout << "Round Constants" << endl;
    for(uint8_t i = 0; i < numRoundConstants; i++)
    {
        cout << roundConstants[i] << endl;
    }




    KeyExpansion();


    cout << "Key Schedule words:" << endl;
    for(uint8_t i = 0; i < numWordsInKeySched; i++)
    {
        cout << keySched[i] << endl;
    }
    cout << endl << endl;




    cout << "BLOCKS: " << endl;
    for(uint64_t i = 0; i < numBlocks; i++)
    {
        copyInputToState(blockArray[i], state);


        cout << "BEGINNING STATE" << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(uint8_t column = 0; column < 4; column++ )
            {
                cout << std::hex << (int) state[column][row];
            }
        }
        cout << endl;

        AddRoundKey(0);


        cout << "after add round key: " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(uint8_t column = 0; column < 4; column++ )
            {
                cout << std::hex << (int) state[column][row];
            }
        }
        cout << endl;


        SubBytes();

        cout << "after SubBytes(): " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(uint8_t column = 0; column < 4; column++ )
            {
                cout << std::hex << (int) state[column][row];
            }
        }
        cout << endl;


        ShiftRows();

        cout << "after ShiftRows(): " << endl;
        for(uint8_t row = 0; row < 4; row++)
        {
            for(uint8_t column = 0; column < 4; column++ )
            {
                cout << std::hex << (int) state[column][row];
            }
        }
        cout << endl;








//        for(uint8_t round = 0; round < nR; round++)
//        {
//
//        }


    }
    cout << endl;






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
void AesEncryptObj::splitInputIntoBlocks(unsigned char blockArray[][16], uint16_t numBlocks, const vector<unsigned char> & inputVector)
{
//    for each block that needs to be created
    for(uint16_t curBlock = 0; curBlock < numBlocks; curBlock++)
    {
//        index position where current block begins
        uint16_t startOfBlock = curBlock * 16;

//        index position where current block ends
        uint16_t endOfBlock = ((curBlock + 1) * 16);

//        current index within the block being added
        uint16_t columnIndex = 0;

//        for each byte in the current block, add the byte to the 2d array
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

//    for each row of the state
    for(uint8_t row = 0; row < 4; row++)
    {

//        for each column of the state
        for(uint8_t column = 0; column < 4; column++)
        {

//            formula provided by NIST in AES specification
//            copies byte 0 to top left spot, byte 4 to the right of that, etc
            stateTable[row][column] = inputBlock[row + (4 * column)];
        }
    }
}



/*
    return value: none
    parameters:
        vector that contains the finite field representation of a byte
        vector that contains the finite field representation of the irreducible polynomial to use

    description:
        This function takes the finite field byte and the finite field irreducible polynomial
        and expands any terms that need to be expanded according to irreducible polynomial given
        (E.G. polynomials with an exponent greater than 7 need to be exploded)
*/
void AesEncryptObj::explode(vector<uint8_t> & finiteField, const vector<uint8_t> & irreducePoly)
{
    // how many polynomial elements were exploded in current loop
    int numExploded = 0;

    do
    {
        numExploded = 0;

        // will store the numbers from irreducible polynomial + what needs to be added to them
        vector<int> numsToAdd;
        numsToAdd.clear();

        int i = 0;

        // iterate through each number
        while(i < finiteField.size())
        {
            // if larger than allowed
            if(finiteField[i] >= irreducePoly[0])
            {
                // how many numbers were exploded in current loop
                numExploded++;

                // store temporary value
                int temp = finiteField[i];

                // remove the element that is too large
                finiteField.erase(finiteField.begin() + i);


                for(int j = 1; j < irreducePoly.size(); j++)
                {
                    // push back numbers from irreducible polynomial plus the difference
                    numsToAdd.push_back(irreducePoly[j] + temp - irreducePoly[0]);
                }

                // if an item was removed, decrement index
                i--;


            }

            // increment index to move to next element
            i++;
        }

//        add in all of the numbers after explosion
        for(int x : numsToAdd)
        {
            finiteField.push_back(x);
        }

//    end only when an entire loop is done without any elements exploded
    } while (numExploded > 0);

}




/*
    return value: none
    parameters:
        uint8_t vectors containing the finite fields you want to add

    description:
        This function takes 2 finite fields and adds them (E.G. xor) and then returns a finite field with the result
*/
void AesEncryptObj::galoisAdd( vector<uint8_t> & vect)
{



    // positions of numbers where there is an even quantity of the number
    vector<int> even_removes;

    // positions of numbers where there is an odd quantity of the number
    vector<int> odd_removes;


    for(int x : vect)
    {
        // how many times the number is in the polynomial
        int count = 0;

        // positions of the number that match what is being search for
        vector<int> positions;
        positions.clear();


        for(int i = 0; i < vect.size(); i++)
        {

            int y = vect[i];

            // if there is a match
            if(x == y)
            {



                // add the position to the list of positions
                positions.push_back(i);
                count++;


            }
        }


        // if there is an even number of the element
        if((count % 2) == 0)
        {
            sort(positions.begin(), positions.end());

            // for each position
            for(int pos : positions)
            {
                bool already_in = false;

                for(int pos2 : even_removes)
                {
                    if(pos == pos2)
                    {
                        already_in = true;
                        break;
                    }
                }

                if(!already_in)
                {
                    // add positions of elements that need to be removed
                    even_removes.push_back(pos);
                }


            }
        }

//        odd number of the element
        else if(count > 1)
        {
            sort(positions.begin(), positions.end());

            // counter to keep track of how many positions have been added
            int removed = 0;

            // for each position
            for(int pos : positions)
            {
                bool already_in = false;

                for(int pos2 : odd_removes)
                {
                    if(pos == pos2)
                    {
                        already_in = true;
                        break;
                    }
                }

                if(!already_in)
                {
                    // if not on the first position, add to vector
                    if(removed > 0)
                    {
                        odd_removes.push_back(pos);
                    }
                }


                removed++;
            }
        }
    }


//    sort the positions of the elements that need to be removed
    sort(even_removes.begin(), even_removes.end());
    sort(odd_removes.begin(), odd_removes.end());

//    count of how many elements have been removed
    int removed = 0;

    // for each of the numbers where there is an even count
    for(int pos : even_removes)
    {
//        remove the element
        vect.erase(vect.begin() + (pos - removed));


        removed++;
    }


//    remove the duplicate values for elements that appear an odd number of times
    removed = 0;
    for(int pos : odd_removes)
    {
//        remove the element
        vect.erase(vect.begin() + (pos - removed));


        removed++;
    }
}



/*
    return value: none
    parameters:
        uint8_t vector containing the finite field to apply modular reduction to
        uint8_t vector containing the irreducible polynomial to use for modular reduction

    description:
        This function takes the finite field and the irreducible polynomial given and applies modular reduction to
        the finite field using the irreducible polynomial
*/
void AesEncryptObj::mod_reduce(vector<uint8_t> & vals, const vector<uint8_t> & irreduce)
{

    explode(vals, irreduce);
    galoisAdd(vals);

}



/*
    return value: uint8_t vector containing the result of the finite field multiplication
    parameters:
        2 uint8_t vectors containing the finite fields you want to multiply
        uint8_t vector containing the irreducible polynomial to use as last parameter

    description:
        This function takes 2 finite fields and multiplies them, applying modular reduction with irreducible
        polynomial given
*/
vector<uint8_t> AesEncryptObj::galoisMultiply(const vector<uint8_t> & a, const vector<uint8_t> & b, const vector<uint8_t> & irreduce)
{
    vector<uint8_t> vect;

    for(unsigned char x : a)
    {
        for(unsigned char j : b)
        {
            vect.push_back(x + j);

        }
    }

//    galois addition on the vector
    galoisAdd(vect);



    sort(vect.rbegin(), vect.rend());

    // modular reduction
    mod_reduce(vect, irreduce);



    return vect;

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
    for(uint8_t row = 0; row < 4; row++)
    {
        for(uint8_t column = 0; column < 4; column++)
        {
//            set value of byte to the mapped value in the sBox
//            passes in byte as key to find function, then fetches the value from the iterator using "second"
            state[row][column] = sBox.find(state[row][column])->second;
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
            stateWord = stateWord.leftRotate();
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

            cout << "byte from state: " << (int)  state[row][column] << endl;
            cout << "round key word: " << (keySched[(round * nB) + column]) << endl;
            cout << "byte from round key word: " << ((keySched[(round * nB) + column])[row])->rawData() << endl;

//            make each byte in state the result of xor with byte from round key
            state[row][column] = state[row][column] ^ ((keySched[(round * nB) + column])[row])->rawData();


            cout << "result from addroundkey: " << std::hex << (int) state[row][column] << endl << endl;
        }
    }

}








/*
    return value: unsigned char array of size 12 bytes
    parameters: none
    description: this function generates an initialization vector (IV). The
      IV is a cryptographically securely randomized array of unsigned
      characters of size 12 bytes. The IV is later used as an input to the
      encryption algorithm to ensure semantic security (plaintext cannot be
      derived from a message's cyphertext).
*/
//unsigned char * AesEncryptObj::generateIV()
//{
//    unsigned char initVector[12] = {0};
//
////    initialize Sodium and populate IV. randombytes_buf() is a Sodium library function
//    if(sodium_init() == -1)
//        return nullptr;
//    randombytes_buf(initVector, 12);
//
//    unsigned char *ptr = &initVector[0];
//    return ptr;
//}







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

    cout << endl << "IN KEY EXPANSION" << endl << endl;
//    holds a temporary 4-byte word in a later loop, will hold the value of the previous word
    word temp;

//    index position
    uint8_t i = 0;


//    for each word of the cipher key
    while (i < nK)
    {
//        copy the current word of the cipher key into the beginning of the key schedule
        keySched[i] = key[i];
        cout << keySched[i] << endl;

        i++;
    }
    cout << endl;

    i = nK;


//    while i less than (4 * ( number of rounds + 1 ) )
    while (i < (nB * (nR + 1)))
    {
//        copy the previous word into temp
        temp = keySched[i-1];
        cout << "temp: " << temp << endl;


//        happens every nK words of the key
        if((i % nK) == 0)
        {
//            rotate temp
            temp = temp.leftRotate();
            cout << "After RotWord(): " << temp << endl;


//            then sub each of the bytes according to sBox
            temp = temp.SubWord(sBox);
            cout << "After SubWord(): " << temp << endl;


//            then xor each byte with word from round constants
            temp = temp ^ roundConstants[(i / nK) - 1];
            cout << "i: " << (int) i << endl;
            cout << "Nk: " << (int) nK << endl;
            cout << (i / nK) - 1 << endl;
            cout << "Rcon[i/Nk]: " << roundConstants[(i / nK) - 1] << endl;
            cout << "After XOR: " << temp << endl;

        }

//        AES-256 and (i-4) is a multiple of nK
        else if (nK == 8 && (i-4) % nK == 0)
        {
            temp = temp.SubWord(sBox);
        }

        cout << "w[i-Nk]: " << keySched[i - nK] << endl;
//        set current word of key schedule equal to XOR with word nK positions before and temp word
        keySched[i] = keySched[i - nK] ^ temp;

        cout << "temp XOR w[i-nK]: " << keySched[i] << endl << endl << endl;


//        move one word (4 bytes) to the right
        i++;
    }
    cout << endl;


}




