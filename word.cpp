//
// Created by guy on 4/7/21.
//


#include "word.h"

using namespace std;


/*
return value: word object (constructor for word object)
parameters: pointer to first byte of the 4 bytes you want made into a word.


description:
  Goes to the unsigned char pointer you pass in, grabs that byte and the next 3 after it and makes
  a word object with the data from the four bytes stored as a 4 element array of byte objects.

*/
word::word(const unsigned char * wordBytes ) {
    for(uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = wordBytes[i];
    }
}


/*
return value: word object (constructor for word object)
parameters: none, default constructor


description:
  Creates a word object with a 4 byte object array with all bytes set to 0;

*/
word::word() {
    for(uint8_t i = 0; i < 4; i++)
    {
        bytes[i] = 0;
    }
}


/*
return value: word object (constructor for word object)
parameters: word object


description:
  Creates a word object that is a copy of the word object passed in to constructor.
  (bytes array will end up with same values for all byte objects)

*/
word::word(const word & rightWord) {
    for(uint8_t i = 0; i < 4; i++)
    {
        *(this->operator[](i)) = rightWord.getByte(i);
    }
}


/*
return value: none (destructor)
parameters: none


description:
  Destroys the word object.

*/
word::~word() {

}




/*
return value: byte object
parameters: index position of byte to get


description:
    returns a copy of byte object at given index position in the word.

    E.G. passing in 0 returns a copy of the byte at position 0 in the word.
*/
byte word::getByte(const uint8_t & i) const {

    if(i <= 3 && i >= 0) {

        return bytes[i];
    } else
    {
        throw OutOfBounds();
    }
}


/*
return value: pointer to byte object
parameters: index position of byte you want a pointer to. (should be in the range of 0-3)


description:
  Returns pointer to a specific byte object in the word based on index position you pass in.
  E.G. wordObj[0] returns a pointer to the first byte object in the wordObj object

*/
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


/*
return value: word object
parameters: word object to perform xor operation with


description:
  performs xor operation on each byte of the 2 words passed in and returns a word object that contains the result.
  E.G. byte 0 in the left word is xor'd with byte 0 in the right word, byte 1 with byte 1, and so on.
*/
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


/*
return value: reference to current word object (allows for chaining)
parameters: word object to perform xor operation with


description:
  performs xor operation on each byte of the 2 words passed in and sets each byte of the word on the left equal to
  the result of the xor operation with the matching byte.

  E.G. byte 0 in the left word is xor'd with byte 0 in the right word set to result, byte 1 with byte 1, and so on.
*/
word & word::operator^=(const word & rightWord) {
    for(uint8_t i = 0; i < 4; i++)
    {
        *(this->operator[](i)) = this->getByte(i) ^ rightWord.getByte(i);
    }

    return *this;
}




/*
return value: reference to current word object (allows for chaining)
parameters: word object to set current word equal to


description:
    sets the current word equal to the word passed in on the right. Returns reference to current word object.
*/
word & word::operator=(const word & rightWord) {

    for(uint8_t i = 0; i < 4; i++)
    {

//        set each matching byte equal to each other
        *(this->operator[](i)) = rightWord.getByte(i);
    }

//    return pointer to current word object
    return *this;
}


/*
return value: none
parameters: none


description:
    all bytes rotate one position to the left.

    E.G. if bytes are currently (0, 1, 2, 3) after bytes will be (1, 2, 3, 0)

*/
void word::leftRotate() {


//    store copy of the value of first byte
    unsigned char firstByte = this->getByte(0).rawData();

//    for each byte except the last
    for(uint8_t i = 0; i < 3; i++)
    {

//        make the value, the value of byte on the right
        *(this->operator[](i)) = this->getByte(i+1);
    }

    *(this->operator[](3)) = firstByte;





}



// same as leftRotate above, but rotates to the right
void word::rightRotate() {
    //    store copy of the value of first byte
    unsigned char lastByte = this->getByte(3).rawData();

//    for each byte except the first
    for(uint8_t i = 3; i > 0; i--)
    {

//        make the value, the value of byte on the left
        *(this->operator[](i)) = this->getByte(i-1);
    }


//    copy what was the last byte onto the first spot
    *(this->operator[](0)) = lastByte;
}




/*
    return value: word object containing all bytes substituted
    parameters:
      unordered_map uint8_t->uint8_t containing the sBox or InvSBox to use for substitution

    description:
      returns a word object that represents what the current word is with all of the bytes
      substituted using the sBox
*/
void word::SubWord(const unordered_map<uint8_t, uint8_t> & sBox) {
//    word retWord;



    for(uint8_t i = 0; i < 4; i++)
    {

//        make each byte of the retWord have a value of the mapped value for the corresponding byte
//        *(retWord[i]) = this->operator[](i)->SubByte(sBox);
        this->operator[](i)->SubByte(sBox);

    }



//    return retWord;
}


ostream  & operator<<(ostream & stream, const word & rightWord)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        stream << rightWord.getByte(i);
    }
    return stream;
}