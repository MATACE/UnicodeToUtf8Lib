#include "unicode_to_utf8_trans.h"

UnicodeToUtf8Trans::UnicodeToUtf8Trans()
{
    is_unicode_hight_low_hex_inversion_ = false;
}

void UnicodeToUtf8Trans::TransUnicodeBufToUft8Str(char* unicode_buf, unsigned int unicode_buf_len, string& utf8_str)
{
    string unicode_str = "";
    string unicode_buf_charcater_str;

    // Get the unicode hex from the unicode buf then transfor to the hex str.
    for (unsigned int i = 0; i < unicode_buf_len; i++) {
        unicode_buf_charcater_str.clear();
        unicode_buf_charcater_str = unicode_buf[i];
        unicode_str.append(StrToHex(unicode_buf_charcater_str));
    }

    // Get the each character 4 byte utf8 str.
    string tmp_utf8_str;
    TransUnicodeStrToUtf8Str(unicode_str, tmp_utf8_str);

    // Get the standard utf8 str.
    GetStandardUtf8Str(tmp_utf8_str, utf8_str);

}

void UnicodeToUtf8Trans::TransUnicodeStrToUtf8Str(string unicode_str, string& utf8_str)
{
    string tmp_utf8_str = "";
    long long hex_val = 0;
    string unicode_hex_str = unicode_str;
    string hex_str;
    string hex_h_str;
    string hex_l_str;
    string hight_low_hex_str;
    string utf8_charcater;

    unsigned short unicode_buf[1];
    unsigned char utf8_buf[4];

    if (is_unicode_hight_low_hex_inversion_) {
        // The unicode hex need invers: 0x4f00 --> 0x004f.
        for (unsigned int i = 0; i < unicode_hex_str.size() / 4; ++i) {
            // Get the right hight-hex unicode hex str by invers. 
            hex_str.clear();
            hex_h_str.clear();
            hex_l_str.clear();
            hex_str = unicode_hex_str.substr(i * 4, 4);

            hex_h_str = hex_str.substr(2, 2);
            hex_l_str = hex_str.substr(0, 2);
            hight_low_hex_str = hex_h_str + hex_l_str;

            try {
                hex_val = strtoull(hight_low_hex_str.c_str(), nullptr, 16);
            } catch (...) {
                std::cerr << "Get the unicode hex value failed." << std::endl;
                utf8_charcater = "";
                tmp_utf8_str += utf8_charcater;
                continue;
            }
            
            // Get the uft8 charcater.
            memset(unicode_buf, 0, sizeof(unicode_buf));
            memset(utf8_buf, 0, sizeof(utf8_buf));
            utf8_charcater = "";

            unicode_buf[0] = hex_val;
            UnicodeStrToUTF8Str(unicode_buf, utf8_buf, sizeof(utf8_buf));
            UnsignedCharToString(utf8_charcater, utf8_buf, sizeof(utf8_buf));
            tmp_utf8_str += utf8_charcater;
        } 
    } else {
        for (unsigned int i = 0; i < unicode_hex_str.size() / 4; ++i) {
            // Get the right hight-hex unicode hex str. 
            hex_str.clear();
            hex_h_str.clear();
            hex_l_str.clear();
            hex_str = unicode_hex_str.substr(i * 4, 4);

            hex_h_str = hex_str.substr(0, 2);
            hex_l_str = hex_str.substr(2, 2);
            hight_low_hex_str = hex_h_str + hex_l_str;

            try {
                hex_val = strtoull(hight_low_hex_str.c_str(), nullptr, 16);
            } catch (...) {
                std::cerr << "Get the unicode hex value failed." << std::endl;
                utf8_charcater = "";
                tmp_utf8_str += utf8_charcater;
                continue;
            }
            
            // Get the uft8 charcater.
            memset(unicode_buf, 0, sizeof(unicode_buf));
            memset(utf8_buf, 0, sizeof(utf8_buf));
            utf8_charcater = "";

            unicode_buf[0] = hex_val;
            UnicodeStrToUTF8Str(unicode_buf, utf8_buf, sizeof(utf8_buf));
            UnsignedCharToString(utf8_charcater, utf8_buf, sizeof(utf8_buf));
            tmp_utf8_str += utf8_charcater;
        } 
    }

    utf8_str = tmp_utf8_str;
}

/**
 * @brief Set the Unicode character hight hex and lox hex be inversion.
 * 
 * @param flag 
 */
void UnicodeToUtf8Trans::SetUnicodeStrHightAndLowHexInversion(bool flag)
{
    is_unicode_hight_low_hex_inversion_ = flag;
}

/**
 * @brief Transfor the str to the hex string.
 * 
 * @param str 
 * @param separator 
 * @return string 
 */
string UnicodeToUtf8Trans::StrToHex(std::string str, std::string separator)
{
    const std::string hex = "0123456789ABCDEF";
    std::stringstream ss;
 
    for (std::string::size_type i = 0; i < str.size(); ++i)
        ss << hex[(unsigned char)str[i] >> 4] << hex[(unsigned char)str[i] & 0xf] << separator;

    std::string hex_str = ss.str();
    transform(hex_str.begin(), hex_str.end(), hex_str.begin(), ::tolower);

    // std::cout << hex_str << std::endl;
    return hex_str;
}


void UnicodeToUtf8Trans::UnsignedCharToString(string &str, unsigned char *buf, unsigned int buf_len)
{
    string tmp_str = "";
    for (size_t i = 0; i < buf_len; ++i) {
        tmp_str += buf[i];
    }
    str = tmp_str;
}

void UnicodeToUtf8Trans::GetStandardUtf8Str(string src_str, string &dec_str)
{
    // Get the not standard utf8 hex string.
    char ch_buf[src_str.size()];
    strncpy(ch_buf, src_str.c_str(), src_str.size());
    string not_standard_utf8_str = StrToHex(src_str);

    // Delete the 00 in the not stand utf8 str to be the standard utf8 str.
    // Not Standard utf8 str                Standard utf8 str
    // 74000000 e4b88000 63000000 e4bd8400 --> 74 e4b880 63 e4bd84
    string standard_utf8_str;
    string tmp_str;
    for (unsigned int i = 0; i < not_standard_utf8_str.size(); i += 2) {
        tmp_str.clear();
        tmp_str = not_standard_utf8_str.substr(i, 2);
        if (tmp_str != "00") {
            standard_utf8_str += tmp_str;
        }
    }

    unsigned char standard_utf8_buf[standard_utf8_str.size() / 2];

    string standard_utf8_tmp_str = "";

    unsigned int i = 0;
    unsigned int j = 0;
    for (; i < standard_utf8_str.size() / 2; j += 2, ++i) {
        standard_utf8_tmp_str.clear();
        standard_utf8_tmp_str = standard_utf8_str.substr(j, 2);
        try {
            standard_utf8_buf[i] = strtol(standard_utf8_tmp_str.c_str(), nullptr, 16);
        } catch (...) {
            std::cerr << "GetStandardUtf8Str Function Use strtol faield." << std::endl;
            standard_utf8_buf[i] = 0x00;
        }
        
    }

    UnsignedCharToString(dec_str, standard_utf8_buf, sizeof(standard_utf8_buf));

}