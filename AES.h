//
// Created by guy on 3/29/21.
//

#ifndef COSC370_PROJECT_AESOBJ_H
#define COSC370_PROJECT_AESOBJ_H

#include <unordered_map>


class AesObj {
    AesObj();
    ~AesObj();

//    state table
    unsigned char state[4][4] = {0};

//    original input data
    unsigned char * input = nullptr;

//    how many bytes are in the input
    int inputLength = 0;


//    128 bit key used to encrypt the data
    unsigned char key[16] = {0};



    const unordered_map<int, int>   = {
            {0, 0}, {1, 1},{2, 141}, {3, 246},{4, 203}, {5, 82},{6, 123}, {7, 209}, {8, 232}, {9, 79},
            {10, 41}, {11, 192},{12, 176}, {13, 225},{14, 229}, {15, 199},{16, }, {17, }, {18, }, {19, },
            {20, }, {21, },{22, }, {23, },{24, }, {25, },{26, }, {27, }, {28, }, {29, },
            {30, }, {31, },{32, }, {33, },{34, }, {35, },{36, }, {37, }, {38, }, {39, },
            {40, }, {41, },{42, }, {43, },{44, }, {45, },{46, }, {47, }, {48, }, {49, },
            {50, }, {51, },{52, }, {53, },{54, }, {55, },{56, }, {57, }, {58, }, {59, },
            {60, }, {61, },{62, }, {63, },{64, }, {65, },{66, }, {67, }, {68, }, {69, },
            {70, }, {71, },{72, }, {73, },{74, }, {75, },{76, }, {77, }, {78, }, {79, },
            {80, }, {81, },{82, }, {83, },{84, }, {85, },{86, }, {87, }, {88, }, {89, },
            {90, }, {91, },{92, }, {93, },{94, }, {95, },{96, }, {97, }, {98, }, {99, },
            {100, }, {101, },{102, }, {103, },{104, }, {105, },{106, }, {107, }, {108, }, {109, },
            {110, }, {111, },{112, }, {113, },{114, }, {115, },{116, }, {117, }, {118, }, {119, },
            {120, }, {121, },{122, }, {123, },{124, }, {125, },{126, }, {127, }, {128, }, {129, },
            {130, }, {131, },{132, }, {133, },{134, }, {135, },{136, }, {137, }, {138, }, {139, },
            {140, }, {141, },{142, }, {143, },{144, }, {145, },{146, }, {147, }, {148, }, {149, },
            {150, }, {151, },{152, }, {153, },{154, }, {155, },{156, }, {157, }, {158, }, {159, },
            {160, }, {161, },{162, }, {163, },{164, }, {165, },{166, }, {167, }, {168, }, {169, },
            {170, }, {171, },{172, }, {173, },{174, }, {175, },{176, }, {177, }, {178, }, {179, },
            {180, }, {181, },{182, }, {183, },{184, }, {185, },{186, }, {187, }, {188, }, {189, },
            {190, }, {191, },{192, }, {193, },{194, }, {195, },{196, }, {197, }, {198, }, {199, },
            {200, }, {201, },{202, }, {203, },{204, }, {205, },{206, }, {207, }, {208, }, {209, },
            {210, }, {211, },{212, }, {213, },{214, }, {215, },{216, }, {217, }, {218, }, {219, },
            {220, }, {221, },{222, }, {223, },{224, }, {225, },{226, }, {227, }, {228, }, {229, },
            {230, }, {231, },{232, }, {233, },{234, }, {235, },{236, }, {237, }, {238, }, {239, },
            {240, }, {241, },{242, }, {243, },{244, }, {245, },{246, }, {247, }, {248, }, {249, },
            {250, }, {251, },{252, }, {253, },{254, }



    };

/*
    return value: unsigned char array containing encrypted bytes
    parameters:
      unsigned char * containing the bytes to encrypt
      int representing how many bytes are in the input
      unsigned char * containing the encryption key to use

    description:
      takes the bytes from the input date given, uses the bytes from the key given, and returns the encrypted version
      of the input data.

 */
    unsigned char * encrypt(unsigned char * , int, unsigned char *);

/*

    return value: none
    parameters:
      unsigned char * containing the input
      int representing how many bytes are in the input
      unsigned char [] containing the state table

    description:
      copies the bytes from the input data given and copies the data onto the state table
*/
    void copyInputToState(unsigned char * , int, unsigned  char []);


/*

    return value: none
    parameters:
      unsigned char * containing the input data that needs to be padded
      int representing how many bytes are in the input


    description:
      This function will pad the input using the PKCS# 7
      In this standard, you just pad bytes that contain the value of how many bytes were added
      E.G. If 6 bytes is added to the last block to make it get to 128 bits, you add the bytes
      06 06 06 06 06 06
      If the data given is a multiple of the block size, an extra block is added with 16 bytes of 16's
      so the decrypting algorithm can determine if padding occurred
*/
    void padInput(unsigned char *, int);






};


#endif //COSC370_PROJECT_AESOBJ_H
