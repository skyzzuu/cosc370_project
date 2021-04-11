//
// Created by guy on 4/7/21.
//

#ifndef COSC370_PROJECT_WORD_H
#define COSC370_PROJECT_WORD_H



#include <cstdint>
#include <unordered_map>
#include "byte.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace std;


class word
{
public:


    /*
    return value: word object (constructor for word object)
    parameters: pointer to first byte of the 4 bytes you want made into a word.


    description:
      Goes to the unsigned char pointer you pass in, grabs that byte and the next 3 after it and makes
      a word object with the data from the four bytes stored as a 4 element array of byte objects.

    */
    word(const unsigned char *);



    /*
    return value: word object (constructor for word object)
    parameters: none, default constructor


    description:
      Creates a word object with a 4 byte object array with all bytes set to 0;

    */
    word();


    /*
    return value: word object (constructor for word object)
    parameters: word object


    description:
      Creates a word object that is a copy of the word object passed in to constructor.
      (bytes array will end up with same values for all byte objects)

    */
    word(const word &);


    /*
    return value: none (destructor)
    parameters: none


    description:
      Destroys the word object.

    */
    ~word();

    /*
    return value: pointer to byte object
    parameters: index position of byte you want a pointer to. (should be in the range of 0-3)


    description:
      Returns pointer to a specific byte object in the word based on index position you pass in.
      E.G. wordObj[0] returns a pointer to the first byte object in the wordObj object

    */
    byte * operator[](uint8_t i);


    /*
    return value: word object
    parameters: word object to perform xor operation with


    description:
      performs xor operation on each byte of the 2 words passed in and returns a word object that contains the result.
      E.G. byte 0 in the left word is xor'd with byte 0 in the right word, byte 1 with byte 1, and so on.
    */
    word operator^(const word &);


    /*
    return value: reference to current word object (allows for chaining)
    parameters: word object to perform xor operation with


    description:
      performs xor operation on each byte of the 2 words passed in and sets each byte of the word on the left equal to
      the result of the xor operation with the matching byte.

      E.G. byte 0 in the left word is xor'd with byte 0 in the right word set to result, byte 1 with byte 1, and so on.
    */
    word & operator^=(const word &);


    /*
    return value: reference to current word object (allows for chaining)
    parameters: word object to set current word equal to


    description:
        sets the current word equal to the word passed in on the right. Returns reference to current word object.
    */
    word & operator=(const word &);


    /*
    return value: byte object
    parameters: index position of byte to get


    description:
        returns a copy of byte object at given index position in the word.

        E.G. passing in 0 returns a copy of the byte at position 0 in the word.
    */
    byte getByte(const uint8_t &) const;


    /*
    return value: word object
    parameters: none


    description:
        returns a copy of word object with all bytes rotated one position to the left.

        E.G. if bytes are currently (0, 1, 2, 3) after this function a word object with bytes (1, 2, 3, 0)
        will be returned.
    */
    word leftRotate();


    /*
    return value: word object
    parameters: none


    description:
        returns a copy of word object with all bytes rotated one position to the right.

        E.G. if bytes are currently (0, 1, 2, 3) after this function a word object with bytes (3, 0, 1, 2)
        will be returned.
    */
    word rightRotate();


    /*
    return value: word object
    parameters: unordered_map uint8_t -> uint8_t
                (should contain either sBox or InvSBox depending on encryption vs decryption)


    description:
        returns a copy of word object with all bytes substituted according to map passed in.

        E.G. should be used for SubBytes and SubWord transformations.
    */
    word SubWord(const unordered_map<uint8_t, uint8_t> & );


    /*
    return value: ostream reference object
    parameters:
        ostream reference
        word object reference


    description:
        outputs all bytes in the word in their hex form

        E.G. if word is 0xff, 0xaa, 0xbb, 0xcc, will output: ffaabbcc in ostream

    */
    friend ostream & operator<<(ostream & , const word & );



private:

//    contains the 4 byte objects that make up the word
    byte bytes[4] = {0};



    /* EXCEPTION CLASS */

    //    used when index position that isn't in the range 0-3 is given
    class OutOfBounds {
    public:
        OutOfBounds() {}
    };
};


#endif //COSC370_PROJECT_WORD_H
