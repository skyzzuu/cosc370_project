//
// Created by guy on 4/8/21.
//

#ifndef COSC370_PROJECT_FINITEFIELD_H
#define COSC370_PROJECT_FINITEFIELD_H


#include <vector>
#include <cstdint>
#include "byte.h"

using namespace std;


class FiniteField {
private:

//    contains the elements of the finite field
    vector<uint8_t> elements;


public:

    /*
    return value: FiniteField object (constructor for FiniteField object)
    parameters: none


    description:
      creates a FiniteField with no elements

    */
    FiniteField();

    /*
    return value: FiniteField object (constructor for FiniteField object)
    parameters: byte object


    description:
      creates a FiniteField equivalent of the byte object passed in

    */
    FiniteField(const byte &);


    /*
    return value: none
    parameters: FiniteField object


    description:
      sets elements of this FiniteField exactly the same as FiniteField on the right

    */
    void operator=(const FiniteField &);



    /*
    return value: none
    parameters: byte object


    description:
      sets elements of this FiniteField equivalent to the byte object passed in

    */
    void operator=(const byte &);

    /*
    return value: none
    parameters: vector<uint8_t>


    description:
      sets elements of this FiniteField exactly to vector passed in

    */
    void operator=(const vector<uint8_t> &);

    FiniteField operator+(const FiniteField &) const;
    void operator+=(const FiniteField &);

    FiniteField galoisMultiply(const FiniteField &, const vector<uint8_t> &);
    void xorSelf();

    void explode(const vector<uint8_t> & );
    void mod_reduce(const vector<uint8_t> & );

    uint8_t & operator[](const uint8_t &);
    uint8_t getElement(const uint8_t &) const;

    uint8_t size() const;


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


    /* EXCEPTION CLASS */


    class OutOfBounds {
    public:
        OutOfBounds() {}
    };

};


#endif //COSC370_PROJECT_FINITEFIELD_H
