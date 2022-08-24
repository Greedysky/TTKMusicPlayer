#ifndef MUSICBDTRANSLATIONREQUEST_H
#define MUSICBDTRANSLATIONREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musictranslationrequest.h"

/*! @brief The class of baidu translation words request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBDTranslationRequest : public MusicTranslationRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBDTranslationRequest)
public:
    enum class Language
    {
        Auto,  /*!< automatic detection*/
        Ara,   /*!< Arabic*/
        De,    /*!< German*/
        Ru,    /*!< Russian*/
        Fra,   /*!< French*/
        Kor,   /*!< Korean*/
        Nl,    /*!< Dutch*/
        Pt,    /*!< Portuguese*/
        Jp,    /*!< Japanese*/
        Th,    /*!< Thai*/
        Wyw,   /*!< Classical*/
        Spa,   /*!< Spanish*/
        El,    /*!< Greek*/
        It,    /*!< Italian*/
        En,    /*!< English*/
        Yue,   /*!< Cantonese*/
        Zh     /*!< Chinese*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicBDTranslationRequest(QObject *parent = nullptr);

    /*!
     * Start to translation data.
     */
    virtual void startToDownload(const QString &data) override final;

    /*!
     * Start to translation data.
     */
    void startToDownload(Language from, Language to, const QString &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

protected:
    /*!
     * Map type from enum to string.
     */
    QString mapTypeFromEnumToString(Language type) const;

};

#endif // MUSICBDTRANSLATIONREQUEST_H
