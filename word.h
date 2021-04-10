//
// Created by guy on 4/7/21.
//

#ifndef COSC370_PROJECT_WORD_H
#define COSC370_PROJECT_WORD_H



#include <cstdint>
#include <unordered_map>
#include "byte.h"

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


    word operator^(const word &);
    void operator^=(const word &);

    void operator=(const word &);

    byte getByte(const uint8_t &) const;


    word leftRotate();
    word rightRotate();



    word SubWord(const unordered_map<uint8_t, uint8_t> & );


private:
    byte bytes[4] = {0};



    /* EXCEPTION CLASS */

    class OutOfBounds
    {
    public:
        OutOfBounds(int index){}

    };
};


#endif //COSC370_PROJECT_WORD_H
