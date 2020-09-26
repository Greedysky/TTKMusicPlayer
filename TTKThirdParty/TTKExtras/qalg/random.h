#ifndef RANDOM_H
#define RANDOM_H

#include "musicextrasglobaldefine.h"

/*! @brief The class of the random wrapper.
 * @author Greedysky <greedysky@163.com>
 */
namespace Random
{
    /*!
     * Init random value seed.
     */
    MUSIC_EXTRAS_EXPORT void initRandom();
    /*!
     * Get random value.
     */
    MUSIC_EXTRAS_EXPORT int random(int value);
}

#endif //BASE64_H
