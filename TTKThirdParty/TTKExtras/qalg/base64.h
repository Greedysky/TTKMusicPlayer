#ifndef BASE64_H
#define BASE64_H

#include "musicextrasglobaldefine.h"

namespace Base64
{
    /*!
     * Base64 encode.
     */
    MUSIC_EXTRAS_EXPORT std::string base64Encode(const unsigned char *bytes, unsigned int len);
    /*!
     * Base64 dncode.
     */
    MUSIC_EXTRAS_EXPORT std::string base64Decode(const std::string &bytes);
}

#endif //BASE64_H
