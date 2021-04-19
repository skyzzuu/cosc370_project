//
// Created by guy on 4/17/21.
//

#ifndef COSC370_PROJECT_IV_H
#define COSC370_PROJECT_IV_H

#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;


#define DEVURANDOM "/dev/urandom"
typedef unsigned char TYPE;


class IV {
public:


/*
    return value: none (constructor)
    parameters:
        unsigned char array containing the IV you want to use
        unsigned 64-bit integer containing the length of the IV in bytes

    description:
      This function will create an IV object from the given data and length.
*/
    IV(const unsigned char *, uint64_t);



/*
    return value: none (constructor)
    parameters:
        unsigned 64-bit integer containing the length of the IV you want to generate in bytes

    description:
      This function will generate an IV object with the given length in bytes.
*/
    IV(uint64_t);






    /*
    return value: none (destructor)
    parameters: none

    description:
      This function will destroy the IV object and free the memory it is using.
*/
    ~IV();


    /*
return value:  pointer to the IV data (const so the data cannot be modified)
parameters: none

description:
  This function will return a pointer to the data contained within the IV. It is const so that the user cannot accidentally
     modify the data. Since the user passes in the length of the IV, they are responsible for making sure they do not read
     beyond the end of the data, however the iv objects do allow seeing how many bytes are in the IV with getlength().

*/
    const unsigned char * getData() const;



    /*
return value:  unsigned 64-bit integer
parameters: none

description:
This function will return an integer representing how long the data in the IV is in bytes.
*/
    uint64_t getLength() const;


private:

//    actual data within the IV
    unsigned char * data = nullptr;

//    length of the data in the iv in bytes
    uint64_t length;


    /*
return value:  unsigned char
parameters: none

description:
This function will return a cryptographically secure random byte by reading from the urandom device.
     static because it does not change based on which object it is called on.
*/
    static TYPE getRandU8();


};


#endif //COSC370_PROJECT_IV_H
