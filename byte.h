/*
byte.h
Joshua Cobb, Luke Engle, Everett Gally
*/

#ifndef COSC370_PROJECT_BYTE_H
#define COSC370_PROJECT_BYTE_H

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <iostream>

using namespace std;



class byte {

private:

    unsigned char data = 0;



public:

/*
    return value: byte object (constructor for byte object)
    parameters: none

    description:
      creates a byte object with a value of 0

*/
    byte();

/*
    return value: byte object (constructor for byte object)
    parameters: byte object

    description:
      creates a byte object that has the same value as byte object passed in as parameter.

*/
    byte(const byte &);



/*
    return value: byte object (constructor for byte object)
    parameters: unsigned char

    description:
      creates a byte object that has the same value as unsigned char passed in as parameter.
    */
    byte(const uint8_t &);



/*
    return value: none (destructor for byte object)
    parameters: none

    description:
      destroys byte object.
*/
    ~byte();



/*
    return value: byte object reference
    parameters: byte object

    description:
      sets value of current byte object equal to value of byte object on right of =
*/
    byte & operator=(const byte &);



/*
    return value: byte object reference
    parameters: byte object

    description:
      sets value of current byte object equal to value of byte object on right of =
*/
    byte & operator=(const unsigned char &);



/*
    return value: boolean
    parameters: byte object

    description:
      returns true if value of byte object matches value of other byte object
*/
    bool operator==(const byte &);



/*
    return value: byte object
    parameters: byte object

    description:
      performs xor operation on current byte and byte on the right and returns result object
*/
    byte operator^(const byte &);



/*
    return value: none
    parameters: byte object

    description:
      sets value of current byte object equal to xor operation with byte on the right
*/
    void operator^=(const byte &);



/*
    return value: byte object
    parameters: byte object

    description:
        performs xor operation on current byte and byte on the right and returns result object
        E.G. since bytes are finite field's, addition is the same as XOR
*/
    byte operator+(const byte &);



/*
    return value: none
    parameters: byte object

    description:
      sets value of current byte object equal to xor operation with byte on the right of =
*/
    void operator+=(const byte &);



/*
    return value: none
    parameters:
        byte object to perform multiplication with

    description:
        performs multiplication on byte treating them as finite field elements by performing modular reduction
        using irreducible polynomial given and modifies the byte to contain the new result.
*/
    void galoisMultiply(byte);



/*
    return value: none
    parameters:
        byte object

    description:
        performs xtime operation on copy of byte passed in and makes this equal to result.
*/
        void xtime(byte);



/*
    return value: unsigned char
    parameters: none

    description:
      returns the value of the byte as an unsigned char (the raw data contained in the class)
*/
    unsigned char rawData() const;



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
    void SubByte(const unordered_map<uint8_t, uint8_t> &);



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
    void SubByteInPlace(const unordered_map<uint8_t, uint8_t> &);



public:

    friend ostream & operator<<(ostream &, const byte &);

};

#endif //COSC370_PROJECT_BYTE_H
