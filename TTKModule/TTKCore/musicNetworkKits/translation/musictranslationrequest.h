#ifndef MUSICTRANSLATIONREQUEST_H
#define MUSICTRANSLATIONREQUEST_H

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

#include "musicabstractnetwork.h"

/*! @brief The class of the translation words manager request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTranslationRequest : public MusicAbstractNetwork
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicTranslationRequest(QObject *parent = nullptr);

    /*!
     * Start to download translation data.
     */
    void startToRequest();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadDataFinished(const QString &bytes);

private:
    /*!
     * Find all plugins.
     */
    void findAllPlugins();

private:
    int m_pluginSelector;

};

#endif // MUSICTRANSLATIONREQUEST_H
