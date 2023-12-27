#include "base64.h"

namespace QAlgorithm
{
    static const char *base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    /*!
     * Char is base64 or not.
     */
    inline static bool isBase64(unsigned char c)
    {
        return isalnum(c) || (c == '+') || (c == '/');
    }
}

TTKString QAlgorithm::base64Encode(const unsigned char *bytes, unsigned int length)
{
    TTKString ret;
    int i = 0, j = 0;
    unsigned char char_array_3[3], char_array_4[4];

    while(length--)
    {
        char_array_3[i++] = *(bytes++);
        if(i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for(i = 0; i < 4; ++i)
            {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if(i)
    {
        for(j = i; j < 3; ++j)
        {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xFC) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xF0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0F) << 2) + ((char_array_3[2] & 0xC0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3F;

        for(j = 0; j < i + 1; ++j)
        {
            ret += base64_chars[char_array_4[j]];
        }

        while((i++ < 3))
        {
            ret += '=';
        }

    }
    return ret;
}

TTKString QAlgorithm::base64Decode(const TTKString &bytes)
{
    int length = bytes.length();
    int i = 0, j = 0, in = 0;
    unsigned char char_array_4[4], char_array_3[3];

    TTKString ret;
    const TTKString container(base64_chars);

    while(length-- && (bytes[in] != '=') && isBase64(bytes[in]))
    {
        char_array_4[i++] = bytes[in]; in++;
        if(i ==4)
        {
            for(i = 0; i < 4; ++i)
            {
                char_array_4[i] = container.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xF) << 4) + ((char_array_4[2] & 0x3C) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for(i = 0; i < 3; ++i)
            {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if(i)
    {
        for(j = i; j < 4; ++j)
        {
            char_array_4[j] = 0;
        }

        for(j = 0; j <4; j++)
        {
            char_array_4[j] = container.find(char_array_4[j]);
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xF) << 4) + ((char_array_4[2] & 0x3C) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for(j = 0; j < i - 1; ++j)
        {
            ret += char_array_3[j];
        }
    }

    return ret;
}
