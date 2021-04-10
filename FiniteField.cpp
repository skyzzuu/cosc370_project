//
// Created by guy on 4/8/21.
//

#include "FiniteField.h"
#include <algorithm>
#include <vector>


FiniteField::FiniteField()
{
//    start off with empty vector
    elements.clear();
}


FiniteField::FiniteField(const byte & rightByte)
{
    elements.clear();

    //    powers of 2
    const uint8_t powsOfTwo[8] = {1, 2, 4, 8 ,16 , 32, 64, 128};


    //    make a copy of the original byte
    unsigned char temp = rightByte.rawData();



    //    will contain the exponents of the bits that are set in the byte
    vector<uint8_t> returnVector;
    returnVector.clear();


    //    for each power of 2 starting from right
    for(uint8_t i = 7; i >= 0; i++)
    {
//        value of the current power
        uint8_t currentPower = powsOfTwo[i];

//        if the current power fits into the remaining value of the byte
        if (currentPower <= temp)
        {
//            add the index position to the vector
//            represents which bits are set
            returnVector.push_back(i);

//            subtract the value from the byte
            temp -= currentPower;
        }
    }

    elements = returnVector;
}


void FiniteField::operator=(const vector<uint8_t> & rightVect)
{
    elements.clear();
    elements = rightVect;
}


void FiniteField::operator=(const FiniteField & rightField)
{
    elements.clear();
    elements = rightField.elements;

}


void FiniteField::operator=(const byte & rightByte)
{
    elements.clear();

    //    powers of 2
    const uint8_t powsOfTwo[8] = {1, 2, 4, 8 ,16 , 32, 64, 128};


    //    make a copy of the original byte
    unsigned char temp = rightByte.rawData();



    //    for each power of 2 starting from right
    for(uint8_t i = 7; i >= 0; i++)
    {
//        value of the current power
        uint8_t currentPower = powsOfTwo[i];

//        if the current power fits into the remaining value of the byte
        if (currentPower <= temp)
        {
//            add the index position to the vector
//            represents which bits are set
            elements.push_back(i);

//            subtract the value from the byte
            temp -= currentPower;
        }
    }

}

FiniteField FiniteField::operator+(const FiniteField & rightField) const
{
    byte leftByte;
    leftByte = *this;

    byte rightByte;
    rightByte = rightField;

    byte retByte = leftByte + rightByte;

    FiniteField retField = retByte;

    return retField;
}



void FiniteField::operator+=(const FiniteField & rightField)
{
    *this = *this + rightField;
}




FiniteField FiniteField::galoisMultiply(const FiniteField & rightField, const vector <uint8_t> & irreduce) {

//    copy elements from this finite field into return field
    FiniteField retField = *this;

//    copy elements from rightField into return field
    for(uint8_t i = 0; i < rightField.size(); i++)
    {
        retField.addElement(rightField.getElement(i));
    }

//    remove elements that appear an even number of times, and remove duplicates of elements
//    that appear an odd number of times
    retField.xorSelf();

//    sort the vector
    sort(retField.elements.begin(), retField.elements.end());

//    modular reduction
    retField.mod_reduce(irreduce);



    return retField;
}



void FiniteField::explode(const vector <uint8_t> & irreduce) {
    // how many polynomial elements were exploded in current loop
    uint8_t numExploded = 0;



    do
    {
        numExploded = 0;

        // will store the numbers from irreducible polynomial + what needs to be added to them
        vector<uint8_t> numsToAdd;
        numsToAdd.clear();

        uint8_t i = 0;

        // iterate through each number
        while(i < size())
        {

            // if larger than allowed
            if(this->getElement(i) >= irreduce[0])
            {
                // how many numbers were exploded in current loop
                numExploded++;

                // store temporary value
                uint8_t temp = this->getElement(i);

                // remove the element that is too large
                removeElement(i);


                for(uint8_t j = 1; j < irreduce.size(); j++)
                {
                    // push back numbers from irreducible polynomial plus the difference
                    numsToAdd.push_back(irreduce[j] + temp - irreduce[0]);
                }

                // if an item was removed, decrement index
                i--;


            }

            // increment index to move to next element
            i++;
        }

//        add in all of the numbers after explosion
        for(const uint8_t & x : numsToAdd)
        {
            addElement(x);
        }

//    end only when an entire loop is done without any elements exploded
    } while (numExploded > 0);
}




void FiniteField::mod_reduce(const vector <uint8_t> & irreduce) {
    explode(irreduce);
    xorSelf();
}



void FiniteField::xorSelf() {

    // positions of numbers where there is an even quantity of the number
    vector<uint8_t> even_removes;

    // positions of numbers where there is an odd quantity of the number
    vector<uint8_t> odd_removes;


    for(uint8_t i = 0; i < this->size(); i++)
    {
        uint8_t x = this->getElement(i);

        // how many times the number is in the polynomial
        uint8_t count = 0;

        // positions of the number that match what is being search for
        vector<uint8_t> positions;
        positions.clear();


        for(uint8_t inner = 0; inner < this->size(); inner++)
        {

            uint8_t y = this->getElement(inner);

            // if there is a match
            if(x == y)
            {



                // add the position to the list of positions
                positions.push_back(i);
                count++;


            }
        }


        // if there is an even number of the element
        if((count % 2) == 0)
        {
            sort(positions.begin(), positions.end());

            // for each position
            for(uint8_t pos : positions)
            {
                bool already_in = false;

                for(uint8_t pos2 : even_removes)
                {
                    if(pos == pos2)
                    {
                        already_in = true;
                        break;
                    }
                }

                if(!already_in)
                {
                    // add positions of elements that need to be removed
                    even_removes.push_back(pos);
                }


            }
        }

//        odd number of the element
        else if(count > 1)
        {
            sort(positions.begin(), positions.end());

            // counter to keep track of how many positions have been added
            uint8_t removed = 0;

            // for each position
            for(uint8_t pos : positions)
            {
                bool already_in = false;

                for(uint8_t pos2 : odd_removes)
                {
                    if(pos == pos2)
                    {
                        already_in = true;
                        break;
                    }
                }

                if(!already_in)
                {
                    // if not on the first position, add to vector
                    if(removed > 0)
                    {
                        odd_removes.push_back(pos);
                    }
                }


                removed++;
            }
        }
    }


//    sort the positions of the elements that need to be removed
    sort(even_removes.begin(), even_removes.end());
    sort(odd_removes.begin(), odd_removes.end());

//    count of how many elements have been removed
    uint8_t removed = 0;

    // for each of the numbers where there is an even count
    for(uint8_t pos : even_removes)
    {
//        remove the element
        this->removeElement((pos - removed));


        removed++;
    }


//    remove the duplicate values for elements that appear an odd number of times
    removed = 0;
    for(uint8_t pos : odd_removes)
    {
//        remove the element
        this->removeElement((pos - removed));


        removed++;
    }


}



uint8_t & FiniteField::operator[](const uint8_t & i)  {

    if(i >= 0 && i < size())
    {
        return elements[i];
    } else
    {
        throw OutOfBounds();
    }

}


uint8_t FiniteField::getElement(const uint8_t & i) const {

    if(i >= 0 && i < size())
    {
        return elements[i];
    } else
    {
        throw OutOfBounds();
    }
}


uint8_t FiniteField::size() const {
    return elements.size();
}






void FiniteField::addElement(const uint8_t & element) {
    elements.push_back(element);
}


void FiniteField::removeElement(const uint8_t & position) {
    elements.erase(elements.begin() + position);
}