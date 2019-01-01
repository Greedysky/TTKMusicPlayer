#ifndef MUSICMVRADIOTHREADABSTRACT_H
#define MUSICMVRADIOTHREADABSTRACT_H

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

#include "musicdownloadquerythreadabstract.h"

const QString MV_CATEGORY_URL       = "a2JPOG81ckd3T1NtN004dnRBZ2JubTZYeFYxSFBXWCtDdkZmVVZKY1NVNGxDUGhaMjY0dXh2bk1XWWpXRlpQUmE3ZG5VZVErNURVPQ==";


/*! @brief The class of music mv radio thread abstract.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicMVRadioThreadAbstract : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMVRadioThreadAbstract)
public:
    explicit MusicMVRadioThreadAbstract(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;

Q_SIGNALS:
    /*!
     * Create the current category item.
     */
    void createCategoryItem(const MusicResultsItem &item);

};

#endif // MUSICMVRADIOTHREADABSTRACT_H
