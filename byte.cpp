/*
byte.cpp
Joshua Cobb, Luke Engle, Everett Gally
*/

#include "byte.h"
#include <unordered_map>
#include <cmath>

using namespace std;



/*
  return value: byte object reference
  parameters: byte object

  description:
    sets value of current byte object equal to value of byte object on right of =
*/
byte& byte::operator=(const byte & rightByte)
{
    data = rightByte.rawData();
    return *this;
}



/*
  return value: byte object reference
  parameters: byte object

  description:
    sets value of current byte object equal to value of byte object on right of =
*/
byte& byte::operator=(const unsigned char & rightChar)
{
    data = rightChar;
    return *this;
}



/*
  return value: byte object (constructor for byte object)
  parameters: none

  description:
    creates a byte object with a value of 0
*/
byte::byte()
{
    data = 0;
}



/*
  return value: byte object (constructor for byte object)
  parameters: unsigned char

  description:
    creates a byte object that has the same value as unsigned char passed in as parameter.
*/
byte::byte(const uint8_t & val)
{
    data = val;
}



/*
  return value: byte object (constructor for byte object)
  parameters: byte object

  description:
    creates a byte object that has the same value as byte object passed in as parameter.
*/
byte::byte(const byte & rightByte)
{
    data = rightByte.data;
}



/*
  return value: none (destructor for byte object)
  parameters: none

  description:
    destroys byte object.
*/
byte::~byte() {
}



/*
  return value: boolean
  parameters: byte object

  description:
    returns true if value of byte object matches value of other byte object
*/
bool byte::operator==(const byte & rightByte)
{
    return data == rightByte.data;
}



/*
  return value: byte object
  parameters: byte object

  description:
    performs xor operation on current byte and byte on the right and returns result object
*/
byte byte::operator^(const byte & rightByte)
{

//  make unsigned char set to result of xor between 2 byte's data
    unsigned char retVal = data ^ rightByte.data;

//  create byte object with unsigned char constructor
    byte temp(retVal);

//  return byte object
    return temp;

}



/*
  return value: none
  parameters: byte object

  description:
    sets value of current byte object equal to xor operation with byte on the right
*/
void byte::operator^=(const byte & rightByte)
{
    data ^= rightByte.data;
}



/*
  return value: byte object
  parameters: byte object

  description:
    performs xor operation on current byte and byte on the right and returns result object
    E.G. since bytes are finite field's, addition is the same as XOR
*/
byte byte::operator+(const byte & rightByte)
{
    return *this ^ rightByte;
}



/*
  return value: none
  parameters: byte object

  description:
    sets value of current byte object equal to xor operation with byte on the right of =
*/
void byte::operator+=(const byte & rightByte)
{
    *this ^= rightByte;
}



/*
  return value: byte object
  parameters:
    byte object to perform multiplication with
    uint8_t vector representing irreducible polynomial to use for multiplication operation

  description:
    performs multiplication on byte treating them as finite field elements by performing modular reduction
    using irreducible polynomial given
*/
void byte::galoisMultiply(byte rightByte)
{

//  start with 0
    byte result = 0;

//  run 8 times, once for each bit in b
    for(uint8_t i = 0; i < 8; i++)
    {

//      if the rightmost bit of b is currently set.
//      shifting to the left 7, and then to the right 7
//      will either leave you with 0 or 1 depending on what the right bit is
        if(((unsigned char)(rightByte.data << 7)) >> 7)
        {
            result.data = result.rawData() ^ this->rawData();
        }

//      xtime operation
        this->xtime(*this);

//      bitwise shift rightbyte 1 to the right
        rightByte.data = rightByte.rawData() >> 1;

    }



//  set this byte equal to the result after multiplication
    this->data = result.rawData();
}



void byte::xtime(byte rightByte) {

//  if msb is set
    if(rightByte.data >= 128)
    {
//      bitwise left shift
        rightByte.data = rightByte.data << 1;

//      xor with 0x1b to reduce
        rightByte.data = rightByte.data ^ 0x1b;
    }

//  msb not set
    else
    {

//      left shift
        rightByte.data  = rightByte.data << 1;
    }

    this->data = rightByte.data;
}



/*
  return value: unsigned char
  parameters: none

  description:
    returns the value of the byte as an unsigned char (the raw data contained in the class)
*/
unsigned char byte::rawData() const
{
    return data;
}



/*
  return value: byte object
  parameters:
    unordered map with uint8_t mapped to uint8_t (should always be the sBox or InvSBox that is passed in)

  description:
    This function takes the byte and will perform the SubBytes transformation, and return the byte.
    This will substitute each of the bytes in the table according to the method that NIST specifies.
    The pre-made sBox or InvSBox is passed in instead of calculating the substitution in the function to
    speed up the encryption and decryption processes.
*/
void byte::SubByte(const unordered_map<uint8_t, uint8_t> & sBox)
{

//  return value that has a key of the value stored in the current byte
//  return sBox.find(this->rawData())->second;

    *(this) = sBox.find(this->rawData())->second;
}



/*
  return value: none
  parameters:
    unordered map with uint8_t mapped to uint8_t (should always be the sBox or InvSBox that is passed in)

  description:
    This function takes the byte and will perform the SubBytes transformation, and modifies the value of the byte object
    it is called on.
    This will substitute each of the bytes in the table according to the method that NIST specifies.
    The pre-made sBox is passed in instead of calculating the substitution in the function to speed up the encryption process.
*/
void byte::SubByteInPlace(const unordered_map<uint8_t, uint8_t> & sBox)
{

//  set byte's value equal to it's mapped value in sBox
    *this = sBox.find(this->rawData())->second;
}



ostream & operator<<(ostream & stream, const byte & rightByte)
{
    stream << std::hex << (int) rightByte.rawData();
    return stream;
}
