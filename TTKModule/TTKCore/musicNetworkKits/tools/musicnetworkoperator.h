#ifndef MUSICNETWORKOPERATOR_H
#define MUSICNETWORKOPERATOR_H

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

#include "musicglobaldefine.h"

/*! @brief The class of the music network operator.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNetworkOperator : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNetworkOperator)
public:
    /*!
     * Object constructor.
     */
    explicit MusicNetworkOperator(QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    void startRequest();

Q_SIGNALS:
    /*!
     * Query network operator is finished.
     */
    void queryNetworkOperatorFinished(const QString &name);

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

};

#endif // MUSICNETWORKOPERATOR_H
