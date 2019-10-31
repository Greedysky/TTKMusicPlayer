#ifndef BASE64_H
#define BASE64_H

#include <string>

namespace Base64
{
    /*!
     * Base64 encode.
     */
    std::string base64Encode(const unsigned char *bytes, uint len);
    /*!
     * Base64 dncode.
     */
    std::string base64Decode(const std::string &bytes);
}

#endif //BASE64_H
