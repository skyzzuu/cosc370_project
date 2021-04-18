//
// Authors: Everett Gally,
//

#include <iostream>
#include "AESEncryptObj.h"
#include "AESDecryptObj.h"
#include "word.h"
#include "byte.h"
#include "FiniteField.h"
#include "IV.h"


using namespace std;


int main()
{
    unsigned char plaintext[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    unsigned char aes128key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char aes192key[24]=  {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    unsigned char aes256key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    unsigned char aes128keyExpansionCipherKey[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    unsigned char aes192keyExpansionCipherKey[24] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
    unsigned char aes256keyExpansionCipherKey[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};


//    AesEncryptObj aes128KeyExpand(128);
//
//    aes128KeyExpand.encrypt(plaintext, 16, aes128keyExpansionCipherKey);


//    AesEncryptObj aes192KeyExpand(192);
//
//    aes192KeyExpand.encrypt(plaintext, 16, aes192keyExpansionCipherKey);


//    AesEncryptObj aes256KeyExpand(256);
//    aes256KeyExpand.encrypt(plaintext, 16, aes256keyExpansionCipherKey);


    vector<unsigned char> encryptedData;

    IV iv(2);





    AesEncryptObj aes128(128);
    encryptedData = aes128.encrypt(plaintext, 16, aes128key, iv);







//    AesEncryptObj aes192(192);
//    encryptedData = aes192.encrypt(plaintext, 16, aes192key, iv);


//    AesEncryptObj aes256(256);
//    encryptedData = aes256.encrypt(plaintext, 16, aes256key, iv);



    vector<unsigned char> decryptedData;
    unsigned char encrData[encryptedData.size()];

    for(int i = 0; i < encryptedData.size(); i++)
    {
        encrData[i] = encryptedData[i];
    }







    for(const unsigned char & temp : encryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl;



//
//
//
//    unsigned char iv[2] = {0x045, 0x25};

    AESDecryptObj aesDecr128(128);
    decryptedData = aesDecr128.decrypt(encrData, encryptedData.size(), aes128key, iv);



//    AESDecryptObj aesDecr192(192);
//    decryptedData = aesDecr192.decrypt(encrData, encryptedData.size(), aes192key, iv);


//    AESDecryptObj aesDecr256(256);
//    decryptedData = aesDecr256.decrypt(encrData, encryptedData.size(), aes256key, iv);


    cout << "DING DING" << endl;
    for(const unsigned char & temp : decryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl;








//    AesEncryptObj aes256(256);
//    vector<unsigned char> encryptedData = aes256.encrypt(plaintext, 16, aes256key);
//
//    cout << endl << endl;
//    for(const unsigned char & byte : encryptedData)
//    {
//        cout << std::hex << (int) byte;
//    }



//    byte left = 0x53;
//    byte right = 0x04;
//
//
//    left.galoisMultiply(right);
//
//
//    cout << std::hex << (int) left.rawData() << endl;








    return 0;
}


//
//unsigned char * generateIV()
//{
//    unsigned char initVector[12] = {0};
//
////    initialize Sodium and populate IV. randombytes_buf() is a Sodium library function
//    if(sodium_init() == -1)
//        return nullptr;
//    randombytes_buf(initVector, 12);
//
//    unsigned char *ptr = &initVector[0];
//    return ptr;
//}
//
