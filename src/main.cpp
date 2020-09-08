#include <iostream>

#include <unicode_to_utf8_trans.h>

using namespace std;

int main(int argc, char const *argv[])
{
    string utf8_str = "";

    char buf[] = {0x90, 0x57, 0x10, 0x53, 0xa0, 0x4e, 0xf0, 0x58};

    UnicodeToUtf8Trans trans;
    // The Inversion flag:
    // If the unicode right hex is 0x5790, then get the unicode is 0x9057 need to set the Inversion flag to be true.
    // The default Inversion flag is false.
    trans.SetUnicodeStrHightAndLowHexInversion(true);
    trans.TransUnicodeBufToUft8Str(buf, sizeof(buf), utf8_str);

    std::cout << utf8_str << std::endl;


    return 0;
}
