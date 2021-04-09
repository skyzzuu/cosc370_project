//
// Created by guy on 4/8/21.
//

#ifndef COSC370_PROJECT_BYTE_H
#define COSC370_PROJECT_BYTE_H


#include <unordered_map>
#include <cstdint>
#include <vector>

using namespace std;

class FiniteField;

class byte {
private:
    unsigned char data = 0;


public:

    byte();
    byte(const byte &);
    byte(const unsigned char &);
    ~byte();

    byte & operator=(const byte &);
    byte & operator=(const unsigned char &);
    bool operator==(const byte &);
    byte & operator=(const FiniteField & );

    byte operator^(const byte &);
    void operator^=(const byte &);

    byte operator+(const byte &);
    void operator+=(const byte &);

    byte galoisMultiply(const byte &, const vector<uint8_t> &);


    unsigned char rawData() const;


    /*

    return value: byte object
    parameters:
      unordered map with uint8_t mapped to uint8_t (should always be the sBox or InvSBox that is passed in)


    description:
      This function takes the byte and will perform the SubBytes transformation, and return the byte.
      This will substitute each of the bytes in the table according to the method that NIST specifies.
      The pre-made sBox is passed in instead of calculating the substitution in the function to speed up the encryption process.
*/
    byte SubByte(const unordered_map<uint8_t, uint8_t> &);



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

};


#endif //COSC370_PROJECT_BYTE_H
