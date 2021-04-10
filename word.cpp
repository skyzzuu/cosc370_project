//
// Created by guy on 4/7/21.
//


#include "word.h"

using namespace std;


//pass in 4 element unsigned char array, fills bytes of word with elements from array
word::word(const unsigned char * wordBytes ) {
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


//constructor passing in another word obj as param
word::word(const word & rightWord) {
    for(uint8_t i = 0; i < 4; i++)
    {
        *(this->operator[](i)) = rightWord.getByte(i);
    }
}



word::~word() {

}





byte word::getByte(const uint8_t & i) const {

    if(i <= 3 && i >= 0) {

        return bytes[i];
    } else
    {
        throw OutOfBounds();
    }
}


//overload [] operator returning reference to specific byte in bytes array with bounds checking
byte * word::operator[](uint8_t i) {

//    valid index position in the word
    if(i <= 3 && i >= 0)
    {

//        return reference to specific byte in the word
        return & bytes[i];
    }

//    invalid index position in word
    else
    {
        throw OutOfBounds();
    }
}


//overload bitwise xor operator
word word::operator^(const word & rightWord) {
    word retWord;

//    for each byte in the words
    for(uint8_t i = 0; i < 4; i++)
    {
//        put in the value of the matching words xor'd with each other
        *(retWord[i]) = this->getByte(i) ^ rightWord.getByte(i);
    }

    return retWord;
}


//overload bitwise xor assignment operator
word & word::operator^=(const word & rightWord) {
    for(uint8_t i = 0; i < 4; i++)
    {
        *(this->operator[](i)) = this->getByte(i) ^ rightWord.getByte(i);
    }

    return *this;
}




//overload assignment operator
word & word::operator=(const word & rightWord) {

    for(uint8_t i = 0; i < 4; i++)
    {

//        set each matching byte equal to each other
        *(this->operator[](i)) = rightWord.getByte(i);
    }

//    return pointer to current word object
    return *this;
}



word word::leftRotate() {

//    word that will be returned
    word retWord;

//    store copy of the value of first byte
    unsigned char firstByte = this->getByte(0).rawData();

//    for each byte except the last
    for(uint8_t i = 0; i < 3; i++)
    {

//        make the value, the value of byte on the right
        *(retWord[i]) = this->getByte(i+1);
    }

    *(retWord[3]) = firstByte;

    return retWord;




}

/*
    return value: word object containing all bytes substituted
    parameters:
      unordered_map uint8_t->uint8_t containing the sBox to use for substitution

    description:
      returns a word object that represents what the current word is with all of the bytes
      substituted using the sBox
*/
word word::SubWord(const unordered_map<uint8_t, uint8_t> & sBox) {
    word retWord;

//    for(uint8_t i = 0; i < 4; i++)
//    {
//        *(retWord[i]) = sBox.find(this->getByte(i).rawData())->second;
//    }

    for(uint8_t i = 0; i < 4; i++)
    {

//        make each byte of the retWord have a value of the mapped value for the corresponding byte
        *(retWord[i]) = this->operator[](i)->SubByte(sBox);
    }



    return retWord;
}

