//
// Created by guy on 3/29/21.
//

#include "AES.h"


AesObj::AesObj() {

}


AesObj::~AesObj() {

}



unsigned char * AesObj::encrypt(unsigned char * data, unsigned char key[]) {

}


void AesObj::padInput(unsigned char * input) {

    //    number of bytes in input is not a multiple of 16 (128 bits)
    if((sizeof(input) % 16) != 0)
    {

    }

}