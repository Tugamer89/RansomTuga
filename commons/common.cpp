#define _CRT_SECURE_NO_WARNINGS
#include "common.h"

using namespace std;
using namespace CryptoPP;

static const string base64_chars = (string)skCrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

static inline bool is_base64(BYTE c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

string aes_decrypt(string key, string cipher, string iv) {
	string result;
	CBC_Mode<AES>::Decryption cbc_decryptor;
	cbc_decryptor.SetKeyWithIV((byte*)key.c_str(), AES::MAX_KEYLENGTH, (byte*)iv.c_str());
	auto decryptor = new Base64Decoder(new StreamTransformationFilter(cbc_decryptor,
		new StringSink(result),
		StreamTransformationFilter::DEFAULT_PADDING));
	StringSource(cipher, true, decryptor);

	return result;
}

string aes_encrypt(string key, string plain, string iv) {
	string result;
	CBC_Mode<AES>::Encryption cbc_encryptor;
	cbc_encryptor.SetKeyWithIV((byte*)key.c_str(), AES::MAX_KEYLENGTH, (byte*)iv.c_str());
	auto encryptor = new StreamTransformationFilter(cbc_encryptor,
		new Base64Encoder(new StringSink(result), false),
		StreamTransformationFilter::DEFAULT_PADDING);
	StringSource(plain, true, encryptor);

	return result;
}

string base64_encode(BYTE const* buf, unsigned int bufLen) {
    string ret;
    int i = 0;
    int j = 0;
    BYTE char_array_3[3];
    BYTE char_array_4[4];

    while (bufLen--) {
        char_array_3[i++] = *(buf++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

vector<BYTE> base64_decode(const string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    BYTE char_array_4[4], char_array_3[3];
    vector<BYTE> ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) 
            ret.push_back(char_array_3[j]);
    }

    return ret;
}
