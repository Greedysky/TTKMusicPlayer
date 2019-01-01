#ifndef MUSICTRANSLATIONTHREADABSTRACT_H
#define MUSICTRANSLATIONTHREADABSTRACT_H

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

#include "musicnetworkabstract.h"

/*! @brief The class of translation words abstract thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicTranslationThreadAbstract : public MusicNetworkAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTranslationThreadAbstract)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTranslationThreadAbstract(QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to translation data.
     * Subclass should implement this function.
     */
    virtual void startToDownload(const QString &data) = 0;

Q_SIGNALS:
    /*!
     * Send translated data from net.
     */
    void downLoadDataChanged(const QString &data);

protected:
    QVariantMap m_rawData;

};

#endif // MUSICTRANSLATIONTHREADABSTRACT_H
