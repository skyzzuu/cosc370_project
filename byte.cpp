//
// Created by guy on 4/8/21.
//




#include "byte.h"
#include "FiniteField.h"
#include <unordered_map>
#include <cmath>

using namespace std;


// TODO: fix assignment operator overloading
// TODO: should return byte &


byte& byte::operator=(const byte & rightByte)
{
    data = rightByte.rawData();
    return *this;
}





byte& byte::operator=(const unsigned char & rightChar)
{
    data = rightChar;
    return *this;

}


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


byte::byte()
{
    data = 0;
}




byte::byte(const uint8_t & val)
{
    data = val;
}


byte::byte(const byte & rightByte)
{
    data = rightByte.data;
}


byte::~byte() {

}




bool byte::operator==(const byte & rightByte)
{
    return data == rightByte.data;
}


byte byte::operator^(const byte & rightByte)
{

//    make unsigned char set to result of xor between 2 byte's data
    unsigned char retVal = data ^ rightByte.data;

//    create byte object with unsigned char constructor
    byte temp(retVal);

//    return byte object
    return temp;

}




void byte::operator^=(const byte & rightByte)
{
    data ^= rightByte.data;
}




byte byte::operator+(const byte & rightByte)
{
    return *this ^ rightByte;
}




void byte::operator+=(const byte & rightByte)
{
    *this ^= rightByte;
}



byte byte::galoisMultiply(const byte & rightByte, const vector<uint8_t> & irreduce)
{

//    make version of this as finite field
    FiniteField leftField = *this;

//    make version of right byte as finite field
    FiniteField rightField = rightByte;

//    get result of finite field multiplication with given irreducible polynomial
    FiniteField retField = leftField.galoisMultiply(rightField, irreduce);

//    convert back to byte
    byte retByte;
    retByte = retField;

//    return resulting byte object
    return retByte;
}








unsigned char byte::rawData() const
{
    return data;
}



byte byte::SubByte(const unordered_map<uint8_t, uint8_t> & sBox) const
{

//    return value that has a key of the value stored in the current byte
    return sBox.find(this->rawData())->second;
}


void byte::SubByteInPlace(const unordered_map<uint8_t, uint8_t> & sBox)
{

//    set byte's value equal to it's mapped value in sBox
    *this = sBox.find(this->rawData())->second;
}