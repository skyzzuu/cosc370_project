//
// Created by guy on 4/8/21.
//




#include "byte.h"
#include "FiniteField.h"
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
return value: byte object reference
parameters: FiniteField object


description:
  sets value of byte equal to equivalent byte value of FiniteField passed in.

  E.G. FiniteField with elements {5, 0} is 00100001 in binary or 33 in decimal, the value of the byte will
  be 33.

*/
byte& byte::operator=(const FiniteField & rightField) {
    unsigned char retVal = 0;


    //    for each value in the field
    for(uint8_t i = 0; i < rightField.size(); i++)
    {
        //        add the value of 2 to that power
        retVal += pow(2, rightField.getElement(i));
    }

    data = retVal;


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

//    make unsigned char set to result of xor between 2 byte's data
    unsigned char retVal = data ^ rightByte.data;

//    create byte object with unsigned char constructor
    byte temp(retVal);

//    return byte object
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
byte byte::galoisMultiply(const byte & rightByte, const vector<uint8_t> & irreduce)
{

//    make version of this as finite field
    FiniteField leftField = *this;

//    make version of right byte as finite field
    FiniteField rightField = rightByte;

//    get result of finite field multiplication with given irreducible polynomial
//    FiniteField object does heavy lifting for multiplication
    FiniteField retField = leftField.galoisMultiply(rightField, irreduce);

//    convert back to byte
    byte retByte;
    retByte = retField;

//    return resulting byte object
    return retByte;
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
byte byte::SubByte(const unordered_map<uint8_t, uint8_t> & sBox) const
{

//    return value that has a key of the value stored in the current byte
    return sBox.find(this->rawData())->second;
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

//    set byte's value equal to it's mapped value in sBox
    *this = sBox.find(this->rawData())->second;
}




ostream & operator<<(ostream & stream, const byte & rightByte)
{
    stream << std::hex << (int) rightByte.rawData();
    return stream;
}