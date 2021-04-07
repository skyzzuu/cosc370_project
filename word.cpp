//
// Created by guy on 4/7/21.
//


#include "word.h"

using namespace std;


//pass in 4 element unsigned char array, fills bytes of word with elements from array
word::word(unsigned char wordBytes *) {
    for(uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = wordBytes[i];
    }
}


//default constructor, fill with zeros
word::word() {
    for(uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = 0;
    }
}


unsigned char & word::operator[](uint8_t i) {

//    valid index position in the word
    if(i < 4)
    {

//        return reference to specific byte in the word
        return & bytes[i];
    }

//    invalid index position in word
    else
    {
        throw OutOfBounds(i);
    }
}


word word::operator^(const word & rightWord) {
    word retWord;

//    for each byte in the words
    for(uint8_t i = 0; i < 4; i++)
    {
//        put in the value of the matching words xor'd with each other
        retWord[i] = this->bytes[i] ^ rightWord[i];
    }

    return retWord;
}


word word::rotate() {
    word retWord;

    unsigned char firstByte = this->bytes[0];

    for(uint8_t i = 0; i < 3; i++)
    {
        retWord[i] = this->bytes[i+1];
    }

    retWord[3] = firstByte;

    return retWord;
}