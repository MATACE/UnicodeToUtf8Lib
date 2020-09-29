#ifndef UNICODE_TO_UTF8_TRANS_H
#define UNICODE_TO_UTF8_TRANS_H

#include <iostream>
#include <sstream>
#include <algorithm>

#include <sqlsimpleAPI.h>

using namespace std;

#define #define UNICODE_BUF_LENGTH 256

class UnicodeToUtf8Trans {
public:
    UnicodeToUtf8Trans();
    virtual ~UnicodeToUtf8Trans() = default;

    void TransUnicodeBufToUft8Str(char* unicode_buf, unsigned int unicode_buf_len, string& utf8_str);
    void SetUnicodeStrHightAndLowHexInversion(bool flag);

private:
    void TransUnicodeStrToUtf8Str(string unicode_str, string& utf8_str);
    string StrToHex(std::string str, std::string separator = "");
    void UnsignedCharToString(string &str, unsigned char *buf, unsigned int buf_len);
    void GetStandardUtf8Str(string src_str, string &dec_str);

    bool is_unicode_hight_low_hex_inversion_;

};

#endif // UNICODE_TO_UTF8_TRANS_H
