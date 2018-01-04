#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include <QObject>
#include "musicmobileglobaldefine.h"

#define APP_PKG_NAME    "org/greedysky/ttkmobile/TTKMobile"

/*! @brief The class of the music utils.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicUtils : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicUtils(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    Q_INVOKABLE void showWindowNotify(int value);
    /*!
     * Show window notify dialog.
     */
    Q_INVOKABLE void showWindowNotify(const QString &title, const QString &text, int value);
    /*!
     * Show window notify dialog.
     */
    Q_INVOKABLE void showMessageBox(const QString &text, const QString &title = 0, QWidget *parent = 0);
    /*!
     * Show message box.
     */
    Q_INVOKABLE void updateApplicationDialog();
    /*!
     * Update application dialog.
     */

    Q_INVOKABLE int stringFontWidth(const QFont &font, const QString &str);
    /*!
     * Get string width by font.
     */

    Q_INVOKABLE bool removeDir(const QString &dir);
    /*!
     * Remove dir in ecursive.
     */
    void checkTheFileNeededExist();
    /*!
     * Check the related file is exist.
     */
    void dirIsExist(const QString &name);
    /*!
     * Check current dir is exist, no, just create it.
     */
    void checkTheDirectoryExist();
    /*!
     * Check the related dir is exist.
     */

};

#endif // TTKMUSICUTILS_H
