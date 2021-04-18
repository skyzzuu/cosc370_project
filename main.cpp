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


    IV iv(2);



//    AesEncryptObj aes128KeyExpand(128);
//
//    aes128KeyExpand.encrypt(plaintext, 16, aes128keyExpansionCipherKey);
//
//
//    AesEncryptObj aes192KeyExpand(192);
//
//    aes192KeyExpand.encrypt(plaintext, 16, aes192keyExpansionCipherKey);
//
//
//    AesEncryptObj aes256KeyExpand(256);
//    aes256KeyExpand.encrypt(plaintext, 16, aes256keyExpansionCipherKey);






    vector<unsigned char> aes128EncryptedData;

    AesEncryptObj aes128(128);
    aes128EncryptedData = aes128.encrypt(plaintext, 16, aes128key, iv);



    vector<unsigned char> aes192EncryptedData;

    AesEncryptObj aes192(192);
    aes192EncryptedData = aes192.encrypt(plaintext, 16, aes192key, iv);



    vector<unsigned char> aes256EncryptedData;

    AesEncryptObj aes256(256);
    aes256EncryptedData = aes256.encrypt(plaintext, 16, aes256key, iv);






    unsigned char aes128EncrData[aes128EncryptedData.size()];

    for(int i = 0; i < aes128EncryptedData.size(); i++)
    {
        aes128EncrData[i] = aes128EncryptedData[i];
    }



    unsigned char aes192EncrData[aes192EncryptedData.size()];

    for(int i = 0; i < aes192EncryptedData.size(); i++)
    {
        aes192EncrData[i] = aes192EncryptedData[i];
    }



    unsigned char aes256EncrData[aes256EncryptedData.size()];

    for(int i = 0; i < aes256EncryptedData.size(); i++)
    {
        aes256EncrData[i] = aes256EncryptedData[i];
    }





    vector<unsigned char> aes128DecryptedData;

    AESDecryptObj aesDecr128(128);
    aes128DecryptedData = aesDecr128.decrypt(aes128EncrData, aes128EncryptedData.size(), aes128key, iv);



    vector<unsigned char> aes192DecryptedData;

    AESDecryptObj aesDecr192(192);
    aes192DecryptedData = aesDecr192.decrypt(aes192EncrData, aes192EncryptedData.size(), aes192key, iv);


    vector<unsigned char> aes256DecryptedData;

    AESDecryptObj aesDecr256(256);
    aes256DecryptedData = aesDecr256.decrypt(aes256EncrData, aes256EncryptedData.size(), aes256key, iv);



    cout << "AES-128 encrypted data" << endl;
    for(const unsigned char & temp : aes128EncryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl;



    cout << "AES-128 decrypted data" << endl;
    for(const unsigned char & temp : aes128DecryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl << endl;





    cout << "AES-192 encrypted data" << endl;
    for(const unsigned char & temp : aes192EncryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl;


    cout << "AES-192 decrypted data" << endl;
    for(const unsigned char & temp : aes192DecryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl << endl;





    cout << "AES-256 encrypted data" << endl;
    for(const unsigned char & temp : aes256EncryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl;


    cout << "AES-256 decrypted data" << endl;
    for(const unsigned char & temp : aes256DecryptedData)
    {
        cout << std::hex << (int) temp;
    }
    cout << endl << endl << endl;





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
