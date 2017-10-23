#ifndef TTKFILESEARCHCORE_H
#define TTKFILESEARCHCORE_H

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

#include <QThread>
#include <QFileInfoList>
#include "musicmobileglobaldefine.h"

/*! @brief The class of the file search core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKFileSearchCore : public QThread
{
    Q_OBJECT
public:
    enum Type {
        Null_Type,
        Search_Type,
        Size_Type
    };

    explicit TTKFileSearchCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKFileSearchCore();

    Q_INVOKABLE void search(const QStringList &path);
    /*!
     * Search local item lists.
     */
    Q_INVOKABLE void search(const QString &path);
    /*!
     * Search local item.
     */
    Q_INVOKABLE void filesSize(const QString &dir);
    /*!
     * Search local item size.
     */

Q_SIGNALS:
    void searchFinished(const QStringList &path);
    /*!
     * Search local item finished.
     */
    void sizeFinished(qint64 size);
    /*!
     * Search local item size finished.
     */
    void findFilePath(const QString &path);
    /*!
     * Show current found path immediately.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

private:
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */
    QFileInfoList findFile(const QString &path, const QStringList &filter);
    /*!
     * To find file by dir path.
     */
    quint64 dirSize(const QString &dirName);

    bool m_run;
    Type m_type;
    QStringList m_path;

};

#endif // TTKFILESEARCHCORE_H
