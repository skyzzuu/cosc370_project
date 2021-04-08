//
// Created by guy on 4/7/21.
//

#ifndef COSC370_PROJECT_WORD_H
#define COSC370_PROJECT_WORD_H



#include <cstdint>
#include <unordered_map>

using namespace std;


class word
{
public:


    word(unsigned char *);
    word();
    word(const word &);

    unsigned char & operator[](uint8_t i);

    word operator^(const word &);
    void operator^=(const word &);

    void operator=(const word &);



    word leftRotate();
    word rightRotate();



    word SubWord(const unordered_map<uint8_t, uint8_t> & );


private:
    byte bytes[4] = {0};



    /* EXCEPTION CLASS */

    class OutOfBounds
    {
    public:
        OutOfBounds(int index){}

    };
};


#endif //COSC370_PROJECT_WORD_H
