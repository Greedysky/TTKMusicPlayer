#ifndef MUSICABSTRACTTRANSLATIONREQUEST_H
#define MUSICABSTRACTTRANSLATIONREQUEST_H

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

/*! @brief The class of the translation words request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractTranslationRequest : public MusicAbstractNetwork
{
    Q_OBJECT
public:
    enum class Language
    {
        Auto,
        Chinese
    };

public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractTranslationRequest(QObject *parent = nullptr);

    /*!
     * Release the network object.
     */
    virtual void deleteAll() override final;

    /*!
     * Start to download translation data.
     * Subclass should implement this function.
     */
    virtual void startToRequest(const QString &data) = 0;

};

#endif // MUSICABSTRACTTRANSLATIONREQUEST_H
