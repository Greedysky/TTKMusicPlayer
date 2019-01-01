#ifndef MUSICAUTHENTICATIONTHREAD_H
#define MUSICAUTHENTICATIONTHREAD_H

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

#include "musicdatabaseobject.h"
#include "musicnetworkabstract.h"

/*! @brief The class to login authentication.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicAuthenticationThread : public MusicNetworkAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAuthenticationThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAuthenticationThread(QObject *parent = nullptr);

    virtual ~MusicAuthenticationThread();

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to translation data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &usr, const QString &pwd) = 0;
    /*!
     * Get info record.
     */
    inline const MusicUserInfoRecord& getInfoRecord() const { return m_info; }

protected:
    MusicUserInfoRecord m_info;

};

#endif // MUSICAUTHENTICATIONTHREAD_H
