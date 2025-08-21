#ifndef MUSICUNITYQUERYMOVIEREQUEST_H
#define MUSICUNITYQUERYMOVIEREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicquerymovierequest.h"

/*! @brief The class of the unity query mv download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicUnityQueryMovieRequest : public MusicQueryMovieRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicUnityQueryMovieRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset) override;

protected:
    /*!
     * Set to unity query mode.
     */
    inline void setToUnity() noexcept
    {
        m_isUnity = true;
        m_pageIndex = 0;
        m_totalSize = TTK_HIGH_LEVEL;
    }
    /*!
     * Check unity query mode is valid or not.
     */
    inline bool needToUnity() const noexcept
    {
        return m_isUnity;
    }
    /*!
     * Reset unity query mode.
     */
    inline void resetUnity() noexcept
    {
        m_isUnity = false;
    }

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadUnityFinished();

private:
    /*!
     * Find all plugins.
     */
    void findAllPlugins(int count);

private:
    bool m_isUnity;
    int m_pluginSelector;
    int m_startPageIndex;
    QString m_value;

};

#endif // MUSICUNITYQUERYMOVIEREQUEST_H
