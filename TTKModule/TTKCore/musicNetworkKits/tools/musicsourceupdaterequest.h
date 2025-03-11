#ifndef MUSICSOURCEUPDATEREQUEST_H
#define MUSICSOURCEUPDATEREQUEST_H

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

#include <QVariantMap>
#include "ttkversion.h"
#include "musicglobaldefine.h"

/*! @brief The class of the source data query update request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSourceUpdateRequest : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSourceUpdateRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSourceUpdateRequest(QObject *parent = nullptr);

    /*!
     * Start to download update data.
     */
    void startToRequest();

    /*!
     * Get lasted version.
     */
    QString version() const noexcept;
    /*!
     * Get lasted version description.
     */
    QString description() const noexcept;

    /*!
     * Is lasted version.
     */
    bool isLastedVersion() const noexcept;

Q_SIGNALS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const QString &bytes);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadFinished(const QByteArray &bytes);

private:
    QVariantMap m_rawData;

};

#endif // MUSICSOURCEUPDATEREQUEST_H
