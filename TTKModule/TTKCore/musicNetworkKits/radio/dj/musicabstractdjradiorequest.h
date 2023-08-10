#ifndef MUSICABSTRACTDJRADIOREQUEST_H
#define MUSICABSTRACTDJRADIOREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicwyqueryinterface.h"
#include "musicabstractqueryrequest.h"

static const QString DJ_CATEGORY_URL          = "NVkzVk1qY3JGODB1MEpMWEVWMzBkVGZKcFkzK0ppcks1L0tKdnc5emZWUndFYWtjNEZTbWo0dytIakRJNWJyTE5pOXExUT09";
static const QString DJ_RADIO_LIST_URL        = "RkpnbVYvN3RUOUFZY2J2RXpBMFNZLy9lTHU2bnF6dUp1M1VIUjkzSDJJNzJYZVFiSWx3cHFqbjlNSkRhU1JuRERHamVoWWRiZ3RNb000VXQ3Y0dJbm1CMnpUbz0=";
static const QString DJ_DETAIL_URL            = "WjRXcVYzQktuOHlRaHlYWVR3bXpUcnpHTmVYS1c5SkVBTktmQ1AvbDBqMUNWYnRiRUh3eDRzclhibnp4SDZ2OGsweWxkQT09";
static const QString DJ_DETAIL_DATA_URL       = "RldscFN4UXIreThMWlNOSWZ1TFRuZGp2dWlDL2xRZGJtY3EzM0kxVlFrNGVLUGhO";
static const QString DJ_PROGRAM_INFO_URL      = "WFZGVlg5aDJWYmpyUWtMMHRoUTJFU2RBQnZqTXlzT1RaWmdJc0hYT3hRWEx3VndBL0F2YXdlK01FWUU9";
static const QString DJ_PROGRAM_INFO_DATA_URL = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
static const QString DJ_RECOMMEND_URL         = "Rko5M01xTW9ldnUyM0NZbDJjL2RvVVZicGEvVmE4Ky9wZU5KQjUzdkZ6Myt0bVZpa1N0TEZwbjZkUnl5UnZZT0E4V3R4dz09";
static const QString DJ_HOT_URL               = "UklIY0duakY0L0lPcktkSU90N0huckNuRHY0ZWh0Q0lzbndaZ0IvakhwMjhHdVBEa0poQ0dDMFZvTjR0OGdoVA==";
static const QString DJ_HOT_DATA_URL          = "NEZ4U1hlQnQ4SW1MQnNidE9ycytIWTZLR1E0PQ==";


/*! @brief The class of the music dj radio request abstract.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractDJRadioRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractDJRadioRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractDJRadioRequest(QObject *parent = nullptr);

    /*!
     * Get suggest list items.
     */
    inline const MusicResultDataItemList& searchedItems() const { return m_items; }

Q_SIGNALS:
    /*!
     * Create the current category item.
     */
    void createProgramItem(const MusicResultDataItem &item);

protected:
    MusicResultDataItemList m_items;

};

#endif // MUSICABSTRACTDJRADIOREQUEST_H
