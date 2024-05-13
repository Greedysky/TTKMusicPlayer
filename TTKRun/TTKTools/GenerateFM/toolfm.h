#ifndef TOOLFM_H
#define TOOLFM_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicfmconfigmanager.h"
#include "musicpagequeryrequest.h"

/*! @brief The class of the qingting fm radio tool.
 * @author Greedysky <greedysky@163.com>
 */
class ToolFM : public MusicPageQueryRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit ToolFM(QObject *parent = nullptr);

    /*!
     * Start to download radio data.
     */
    void startToRequest();

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

private:
    /*!
     * Start to download radio list data.
     */
    void startToListRequest(int id, MusicFMChannelList *channels);

};

#endif // TOOLFM_H
