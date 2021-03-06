/*
main.cpp
Joshua Cobb, Luke Engle, Everett Gally
*/

#include <iostream>
#include "AESEncryptObj.h"
#include "AESDecryptObj.h"
#include "IV.h"
#include <iomanip>
#include <string.h>

using namespace std;



int main()
{


//    NIST SP 800-38A F.2 example
    unsigned char ivBytes[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    IV iv(ivBytes, 16);

    unsigned char cbcaes128Key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    unsigned char cbcaes128Plaintext[64]  = {
            0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
            0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
            0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
            0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };

    unsigned char cbcaes192Key[24] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};

    unsigned char cbcaes192Plaintext[64] = {
            0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
            0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
            0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
            0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };

    unsigned char cbcaes256Key[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};

    unsigned char cbcaes256Plaintext[64] = {
            0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
            0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
            0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
            0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };



    vector<unsigned char> aes128EncryptedData;

    AesEncryptObj aes128(128);
    aes128EncryptedData = aes128.encrypt(cbcaes128Plaintext, 64, cbcaes128Key, iv);

    vector<unsigned char> aes192EncryptedData;

    AesEncryptObj aes192(192);
    aes192EncryptedData = aes192.encrypt(cbcaes192Plaintext, 64, cbcaes192Key, iv);

    vector<unsigned char> aes256EncryptedData;

    AesEncryptObj aes256(256);
    aes256EncryptedData = aes256.encrypt(cbcaes256Plaintext, 64, cbcaes256Key, iv);



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
    aes128DecryptedData = aesDecr128.decrypt(aes128EncrData, aes128EncryptedData.size(), cbcaes128Key, iv);

    vector<unsigned char> aes192DecryptedData;

    AESDecryptObj aesDecr192(192);
    aes192DecryptedData = aesDecr192.decrypt(aes192EncrData, aes192EncryptedData.size(), cbcaes192Key, iv);

    vector<unsigned char> aes256DecryptedData;

    AESDecryptObj aesDecr256(256);
    aes256DecryptedData = aesDecr256.decrypt(aes256EncrData, aes256EncryptedData.size(), cbcaes256Key, iv);



    cout << "AES-128 encrypted data" << endl;
    for(uint16_t i = 0; i < aes128EncryptedData.size(); i++)
    {

//      cout << std::hex << (int) temp;

        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')   << std::hex << (int) aes128EncryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0') << std::hex << (int) aes128EncryptedData.at(i);

        }
    }

    cout << endl << endl;



    cout << "AES-128 decrypted data" << endl;
    for(uint16_t i = 0; i < aes128DecryptedData.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes128DecryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes128DecryptedData.at(i);

        }
    }

    cout << endl << endl << endl;



    cout << "AES-192 encrypted data" << endl;
    for(uint16_t i = 0; i < aes192EncryptedData.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes192EncryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes192EncryptedData.at(i);

        }
    }

    cout << endl << endl << endl;



    cout << "AES-192 decrypted data" << endl;
    for(uint16_t i = 0; i < aes192DecryptedData.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes192DecryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes192DecryptedData.at(i);

        }
    }

    cout << endl << endl << endl;



    cout << "AES-256 encrypted data" << endl;
    for(uint16_t i = 0; i < aes256EncryptedData.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes256EncryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes256EncryptedData.at(i);

        }
    }

    cout << endl << endl << endl;



    cout << "AES-256 decrypted data" << endl;
    for(uint16_t i = 0; i < aes256DecryptedData.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes256DecryptedData.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes256DecryptedData.at(i);

        }
    }

    cout << endl << endl << endl;











/* Enter up to 64 bytes of text to be encoded and decoded with AES-128 */

/*
    vector<unsigned char> aes128EncryptedDataInput;
    unsigned char cbcaes128PlaintextInput[64] = {0};
    char input[64];

//  get input text to be encoded and decoded
    cout << endl << endl;
    cout << "Enter text to be encrypted:" << endl;
    cin.getline(input, sizeof(input));
    int inputSize = strlen(input);
    memcpy(cbcaes128PlaintextInput, input, inputSize);



//  encrypt and display user input
    aes128EncryptedDataInput = aes128.encrypt(cbcaes128PlaintextInput, 64, cbcaes128Key, iv);

    unsigned char aes128EncrDataInput[aes128EncryptedDataInput.size()];

    for(int i = 0; i < aes128EncryptedDataInput.size(); i++)
    {
        aes128EncrDataInput[i] = aes128EncryptedDataInput[i];
    }

    cout << endl << endl;
    cout << "AES-128 input encrypted data";
    for(uint16_t i = 0; i < aes128EncryptedDataInput.size(); i++)
    {

//      cout << std::hex << (int) temp;

        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')   << std::hex << (int) aes128EncryptedDataInput.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0') << std::hex << (int) aes128EncryptedDataInput.at(i);

        }
    }

    cout << endl << endl;



//  decrypt and display user input
    vector<unsigned char> aes128DecryptedDataInput;
    AESDecryptObj aesDecr128Input(128);
    aes128DecryptedDataInput = aesDecr128Input.decrypt(aes128EncrDataInput, aes128EncryptedDataInput.size(), cbcaes128Key, iv);
    cout << endl << endl << endl;

    cout << "AES-128 input decrypted data (hex)";
    for(uint16_t i = 0; i < aes128DecryptedDataInput.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl << endl;
            cout << setw(2) << setfill('0')  << std::hex << (int) aes128DecryptedDataInput.at(i);
        }

        else
        {
            cout << setw(2) << setfill('0')  << std::hex << (int) aes128DecryptedDataInput.at(i);
        }
    }

    cout << endl << endl << endl;

    cout << "AES-128 input decrypted data (characters)" << endl;
    for(uint16_t i = 0; i < aes128DecryptedDataInput.size(); i++)
    {
        if(i % 16 == 0)
        {
            cout << endl;
            cout << aes128DecryptedDataInput.at(i);
        }

        else
        {
            cout << aes128DecryptedDataInput.at(i);
        }
    }
*/

    cout << endl << endl << endl;

    return 0;
}
