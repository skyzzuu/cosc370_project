//
// Created by guy on 4/17/21.
//

#include "IV.h"





/*
    return value: none (constructor)
    parameters:
        unsigned char array containing the data for the IV you want to use
        unsigned 64-bit integer containing the length of the IV in bytes

    description:
      This function will create an IV object from the given data and length.
*/
IV::IV(const unsigned char * rawData, uint64_t Length) {
    length = Length;


//    reserve memory needed for data
    data = new unsigned char[length];

//    copy data passed in into data array
    for(uint64_t i = 0; i < length; i++)
    {
        data[i] = rawData[i];
    }
}



/*
    return value: none (constructor)
    parameters:
        unsigned 64-bit integer containing the length of the IV you want to generate in bytes

    description:
      This function will generate an IV object with the given length in bytes.
*/
IV::IV(uint64_t Length) {
    length = Length;



//    reserve memory needed for data
    data  = new unsigned char[length];


//        in each loop get a new byte from /dev/urandom and store it in array
    for(uint64_t i = 0; i < length; i++)
    {
        data[i] = getRandU8();
    }


}




/*
return value: none (destructor)
parameters: none

description:
  This function will destroy the IV object and free the memory it is using.
*/
IV::~IV() {
    delete data;
}





/*
return value:  pointer to the IV data (const so the data cannot be modified)
parameters: none

description:
This function will return a pointer to the data contained within the IV. It is const so that the user cannot accidentally
 modify the data. Since the user passes in the length of the IV, they are responsible for making sure they do not read
 beyond the end of the data. They should know how many bytes are in the IV.
*/
const unsigned char * IV::getData() const {
    return data;
}





/*
return value:  unsigned 64-bit integer representing the length of the data in the IV
parameters: none

description:
This function will return an integer representing how long the data in the IV is.
*/
uint64_t IV::getLength() const {


//    return length of IV  in bytes
    return length;
}



/*
    return value: unsigned char
    parameters: none
    description: this function returns one randomly generated unsigned char
      using /dev/urandom. The returned value is used in generateIV().
*/
TYPE IV::getRandU8()
{

//    start byte off as 0
    TYPE rnum = 0;

//    open /dev/urandom for reading
    int fd = open(DEVURANDOM, O_RDONLY);

//    if the file was opened correctly
    if (fd != -1)
    {

//        read one byte and store in rnum
        (void) read(fd, (void *)&rnum, sizeof(TYPE));

//        close /dev/urandom
        (void) close(fd);
    }


//    return the byte
    return rnum;
}