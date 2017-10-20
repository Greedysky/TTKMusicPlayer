#ifndef MUSICTRANSLATIONTHREAD_H
#define MUSICTRANSLATIONTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

const QString TRANSLATION_URL = "TXRkdVhlYnQzSEtZUmpJMVpDeHpaVG5DVzhId0NyVE42YXBPYkw2d25YeGJENDBONm9kSVZ2My95eHgvbVJSQjlDSE92clVkam85OG9uYjU=";

/*! @brief The class of translation words thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicTranslationThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    enum TranslationType
    {
        Type_Auto,  /*!< automatic detection*/
        Type_Ara,   /*!< Arabic*/
        Type_De,    /*!< German*/
        Type_Ru,    /*!< Russian*/
        Type_Fra,   /*!< French*/
        Type_Kor,   /*!< Korean*/
        Type_Nl,    /*!< Dutch*/
        Type_Pt,    /*!< Portuguese*/
        Type_Jp,    /*!< Japanese*/
        Type_Th,    /*!< Thai*/
        Type_Wyw,   /*!< Classical*/
        Type_Spa,   /*!< Spanish*/
        Type_El,    /*!< Greek*/
        Type_It,    /*!< Italian*/
        Type_En,    /*!< English*/
        Type_Yue,   /*!< Cantonese*/
        Type_Zh     /*!< Chinese*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicTranslationThread(QObject *parent = 0);

    virtual ~MusicTranslationThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to translation data.
     */
    void startToDownload(TranslationType from, TranslationType to, const QString &data);

Q_SIGNALS:
    /*!
     * Send translated data from net.
     */
    void downLoadDataChanged(const QString &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Map type from enum to string.
     */
    QString mapTypeFromEnumToString(TranslationType type);

};

#endif // MUSICTRANSLATIONTHREAD_H
