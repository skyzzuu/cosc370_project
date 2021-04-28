/*
FiniteField.h
Joshua Cobb, Luke Engle, Everett Gally
*/

#ifndef COSC370_PROJECT_FINITEFIELD_H
#define COSC370_PROJECT_FINITEFIELD_H

#include <vector>
#include <cstdint>
#include "byte.h"
#include <algorithm>
#include <set>

using namespace std;



class FiniteField {
private:

//  contains the elements of the finite field
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



/*
    return value: FiniteField
    parameters: FiniteField object

    description:
      returns a FiniteField containing the result of finite field addition with FiniteField passed in
*/
    FiniteField operator+(const FiniteField &) const;



/*
    return value: none
    parameters: FiniteField

    description:
      sets elements of this FiniteField equal to finite field addition with FiniteField passed in
*/
    void operator+=(const FiniteField &);



/*
    return value: FiniteField
    parameters:
        FiniteField
        uint8_t vector containing irreducible polynomial

    description:
      performs finite field multiplication on this FiniteField and the FiniteField passed in
      and performs modular reduction using irreducible polynomial given
*/
    FiniteField galoisMultiply(const FiniteField &, const vector<uint8_t> &);



/*
    return value: none
    parameters: none

    description:
      will remove all duplicate elements that appear an evn number of times
      and remove duplicate elements for elements that appear an odd number of times
*/
    void xorSelf();



/*
    return value: none
    parameters: uint8_t vector containing irreducible polynomial to use

    description:
      explode elements that are too large to be within the FiniteField after multiplication.
      (essentially performs the modular reduction)

      explodes according to irreducible polynomial given

      E.G. if the irreducible polynomial defined in AES specification section 4.2 {8, 4, 3, 1, 0}
      is passed in, anything larger than or equal to 8 will be exploded
*/
    void explode(const vector<uint8_t> & );



/*
    return value: none
    parameters: uint8_t vector containing irreducible polynomial to use

    description:
      calls the explode function to perform modular reduction, then calls the xorSelf function to
      remove duplicates and remove elements appearing an even number of times
*/
    void mod_reduce(const vector<uint8_t> & );



/*
    return value: uint8_t
    parameters: uint8_t containing index position

    description:
      returns reference to element at given index position in the field
*/
    uint8_t & operator[](const uint8_t &);



/*
    return value: uint8_t
    parameters: uint8_t

    description:
      returns copy of element at given index position
*/
    uint8_t getElement(const uint8_t &) const;



/*
    return value: uint8_t
    parameters: none

    description:
      returns the number of elements in the FiniteField
*/
    uint8_t size() const;



/*
    return value: none
    parameters:
        uint8_t containing element to add

    description:
      adds uint8_t passed in to the FiniteField
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
