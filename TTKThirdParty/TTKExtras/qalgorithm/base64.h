#ifndef BASE64_H
#define BASE64_H

#include "ttkglobaldefine.h"

namespace Base64
{
    /*!
     * Base64 encode.
     */
    TTK_MODULE_EXPORT TTKString base64Encode(const unsigned char *bytes, unsigned int len);
    /*!
     * Base64 dncode.
     */
    TTK_MODULE_EXPORT TTKString base64Decode(const TTKString &bytes);
}

#endif //BASE64_H
