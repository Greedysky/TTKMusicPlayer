#ifndef DLNAXML_H
#define DLNAXML_H

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

#include "dlnaservice.h"
#include <QtXml/QDomDocument>
#include "musicextrasglobaldefine.h"

/*! @brief The class of the dlna xml.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT DLNAXml
{
public:
    DLNAXml();
    ~DLNAXml();

    bool fromString(const QString &data);
    QString toString() const;

    QString tagNameToLower(const QString &data) const;
    QString readTagNameValue(const QString &tagName) const;
    DLNAService readServiceTag(const QString &type, const QString &tagName) const;

private:
    QDomDocument *m_document;

};

#endif // DLNAXML_H
