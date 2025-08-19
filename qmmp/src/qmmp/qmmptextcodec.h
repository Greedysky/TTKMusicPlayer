/***************************************************************************
 *   Copyright (C) 2021-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef QMMPTEXTCODEC_H
#define QMMPTEXTCODEC_H

#include <QByteArray>
#include <QStringList>
#include "qmmp_export.h"

class QmmpTextCodecPrivate;

/*! @brief The QmmpTextCodec class provides text encoding conversion.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT QmmpTextCodec
{
public:
    /*!
     * Constructor.
     * @param charset Conversion charset.
     */
    QmmpTextCodec(const QByteArray &charset);
    /*!
     * Destructor.
     */
    ~QmmpTextCodec();

    /*!
     * This function returns coversion charset.
     */
    QByteArray name() const;
    /*!
     * This function converts text encoding from coversion charset to unicode.
     * @param a Text array.
     */
    QString toUnicode(const QByteArray &a) const;
    /*!
     * This function converts text encoding from coversion charset to unicode.
     * @param chars Text array.
     */
    QString toUnicode(const char *chars) const;
    /*!
     * This function converts text encoding from unicode to conversion charset.
     * @param str Input string.
     */
    QByteArray fromUnicode(const QString &str) const;

    /*!
     * This function returns a list of the supported charsets.
     */
    static QStringList availableCharsets();

private:
    QmmpTextCodecPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QmmpTextCodec)

};

#endif // QMMPTEXTCODEC_H
