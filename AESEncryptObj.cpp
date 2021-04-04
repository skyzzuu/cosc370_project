

#include "AESEncryptObj.h"
#include <cstdint>
#include <cmath>
#include <algorithm>



AesEncryptObj::AesEncryptObj() {

}


AesEncryptObj::~AesEncryptObj() {

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

unsigned char * AesEncryptObj::generateIV()
{
  return nullptr;
}

vector<unsigned char> AesEncryptObj::encrypt(const unsigned char * data, uint16_t inputLength, const unsigned char key[16])
{

//    unsigned char vector containing all the input data
    vector<unsigned char> inputVector;
    inputVector.clear();

//    copy all bytes from original input into vector
    for(uint16_t i = 0; i < inputLength; i++)
    {
        inputVector.push_back(data[i]);
    }

//    pad up to nearest block (or add full block)
    padInput(inputVector);

//    total number of blocks in padded input
    uint16_t numBlocks = inputVector.size() / 16;

//    make a 2d array with the number of rows set to however many blocks there are in the padded input
    unsigned char blockArray[numBlocks][16];

//    take the blocks from the inputVector and copy the blocks into the blockArray
    splitInputIntoBlocks(blockArray, numBlocks, inputVector);

//    inputVector not needed after this point
    inputVector.clear();









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
