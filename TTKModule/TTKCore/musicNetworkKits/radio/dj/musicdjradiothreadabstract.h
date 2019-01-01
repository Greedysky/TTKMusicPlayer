#ifndef MUSICDJRADIOTHREADABSTRACT_H
#define MUSICDJRADIOTHREADABSTRACT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicdownloadwyinterface.h"
#include "musicdownloadquerythreadabstract.h"

const QString DJ_CATEGORY_N_URL       = "NVkzVk1qY3JGODB1MEpMWEVWMzBkVGZKcFkzK0ppcks1L0tKdnc5emZWUndFYWtjNEZTbWo0dytIakRJNWJyTE5pOXExUT09";
const QString DJ_RADIO_LIST_URL       = "RkpnbVYvN3RUOUFZY2J2RXpBMFNZLy9lTHU2bnF6dUp1M1VIUjkzSDJJNzJYZVFiSWx3cHFqbjlNSkRhU1JuRERHamVoWWRiZ3RNb000VXQ3Y0dJbm1CMnpUbz0=";
const QString DJ_DETAIL_NDT_URL       = "RldscFN4UXIreThMWlNOSWZ1TFRuZGp2dWlDL2xRZGJtY3EzM0kxVlFrNGVLUGhO";
const QString DJ_DETAIL_N_URL         = "WjRXcVYzQktuOHlRaHlYWVR3bXpUcnpHTmVYS1c5SkVBTktmQ1AvbDBqMUNWYnRiRUh3eDRzclhibnp4SDZ2OGsweWxkQT09";
const QString DJ_PROGRAM_INFO_NDT_URL = "dmt4VnNDdWErYjRYUXBnOWdubVBIQT09";
const QString DJ_PROGRAM_INFO_N_URL   = "WFZGVlg5aDJWYmpyUWtMMHRoUTJFU2RBQnZqTXlzT1RaWmdJc0hYT3hRWEx3VndBL0F2YXdlK01FWUU9";
const QString DJ_RECOMMEND_N_URL      = "Rko5M01xTW9ldnUyM0NZbDJjL2RvVVZicGEvVmE4Ky9wZU5KQjUzdkZ6Myt0bVZpa1N0TEZwbjZkUnl5UnZZT0E4V3R4dz09";
const QString DJ_HOT_NDT_URL          = "NEZ4U1hlQnQ4SW1MQnNidE9ycytIWTZLR1E0PQ==";
const QString DJ_HOT_N_URL            = "UklIY0duakY0L0lPcktkSU90N0huckNuRHY0ZWh0Q0lzbndaZ0IvakhwMjhHdVBEa0poQ0dDMFZvTjR0OGdoVA==";


/*! @brief The class of music dj radio thread abstract.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDJRadioThreadAbstract : public MusicNetworkAbstract,
                                                        protected MusicDownLoadWYInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDJRadioThreadAbstract)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDJRadioThreadAbstract(QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Get suggest list items.
     */
    inline const MusicResultsItems& getSearchedItems() const { return m_items; }

Q_SIGNALS:
    /*!
     * Create the current category item.
     */
    void createProgramItem(const MusicResultsItem &item);

protected:
    MusicResultsItems m_items;

};

#endif // MUSICDJRADIOTHREADABSTRACT_H
