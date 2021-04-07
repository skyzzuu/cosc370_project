//
// Created by guy on 4/7/21.
//

#ifndef COSC370_PROJECT_WORD_H
#define COSC370_PROJECT_WORD_H



#include <cstdint>

using namespace std;


class word
{
public:
    unsigned char bytes[4] = {0};

    word(unsigned char *);
    word();

    unsigned char & operator[](uint8_t i);

    word operator^(const word &);

    word rotate();



    /* EXCEPTION CLASS */

    class OutOfBounds
    {
    public:
        OutOfBounds(int index){}

    };
};


#endif //COSC370_PROJECT_WORD_H
