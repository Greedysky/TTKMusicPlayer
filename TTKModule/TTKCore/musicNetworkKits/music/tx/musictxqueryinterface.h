#ifndef MUSICTXQUERYINTERFACE_H
#define MUSICTXQUERYINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "musicglobaldefine.h"

///cookie url
const QString TX_UA_URL             = "NW9YVlRXYnBnNlBFS3Ardi9VR295R0l2aW43U3JvRlpWc1AxMkI2K1MvN3RMVjdsOGhZMVlPRmIrais2ajBJUW1UbmxBRjlyS25WNHRyU0FYbFFMeWFpMUo5YWxFNnkweHd6RERnPT0=";
///song url
const QString TX_SEARCH_URL         = "RVNEYVc1Z1l4Wk9Ma2QxMEl3bnNnNEJaS0hiZVlZNnBQZXlQTUIzZGFjMksyVkNPMkxaeXBnPT0=";
const QString TX_SEARCH_DATA_URL    = "bW8ybE03WEw1WmNmYzBuSTJWeTdPMFZNY0U2RDRYcjZla1djWmU2eU5ld2lZbytpaExzaG1MaDZ6U1FKTzVuWkZqZHJaL0gxN3ZiOTBIaFZwcmdpeU5NQllkQ1RaUzdVVmZ2UjFuWkF5SzlZSHYwajRKWEFXTUNBaGNab0V6U0UzOVdkMlpqM25CS212dEI2aU5KSUtLanJrcE5jU3JJUEEwSjhuVzFJV2xrM0FaWnFrUmladWxIUTlKUzF3RE5NR1RnUzJMNzRBTWF3Q0lLRmltZWc4QUh3b0dlZ1lxMTNXSXJLOGlWTlBEMmV4TkdlL2NzT0lqRW5UMHc9";


class QNetworkRequest;

namespace MusicTXInterface
{
    /*!
     * Make request query data.
     */
    void makeRequestRawHeader(QNetworkRequest *request);

}

#endif // MUSICTXQUERYINTERFACE_H
