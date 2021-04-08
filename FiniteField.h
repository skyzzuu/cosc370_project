//
// Created by guy on 4/8/21.
//

#ifndef COSC370_PROJECT_FINITEFIELD_H
#define COSC370_PROJECT_FINITEFIELD_H


#include "byte.h"
#include <vector>
#include <cstdint>


class FiniteField {
private:
    vector<uint8_t> elements;


public:
    FiniteField();
    FiniteField(const byte &);

    void operator=(const FiniteField &);
    void operator=(const byte &);
    void operator=(const vector<uint8_t> &);

    FiniteField operator+(const FiniteField &);
    void operator+=(const FiniteField &);

    FiniteField operator*(const FiniteField &, const vector<uint8_t> &);
    void operator*=(const FiniteField &, const vector<uint8_t> &);

    void explode(const vector<uint8_t> & );
    void mod_reduce(const vector<uint8_t> & );

    uint8_t & operator[](const uint8_t &);

    uint8_t size();


    /*

    return value: none
    parameters:
        uint8_t containing element to add to FiniteField

    description:
      This function takes the uint8_t element you pass in and adds it into the finite field.
*/
    void addElement(const uint8_t &);


/*

    return value: none
    parameters:
        uint8_t containing index position of element to remove

    description:
      This function takes the index position given and removes the element at that position
      from the finite field.
*/
    void removeElement(const uint8_t &);

};


#endif //COSC370_PROJECT_FINITEFIELD_H
