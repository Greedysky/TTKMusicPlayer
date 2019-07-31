#ifndef MUSICDEVICEINFOCORE_H
#define MUSICDEVICEINFOCORE_H

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

#include <QProcess>
#include "musicglobaldefine.h"

/*! @brief The class of the system device info item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOL_EXPORT MusicDeviceInfoItem
{
    QString m_name;
    QString m_path;
    int m_availableBytes;
    int m_totalBytes;
}MusicDeviceInfoItem;
TTK_DECLARE_LISTS(MusicDeviceInfoItem)


/*! @brief The class of the system device info.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicDeviceInfoCore : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDeviceInfoCore(QObject *parent = nullptr);

    ~MusicDeviceInfoCore();

    /*!
     * Get removable drive property.
     */
    bool GetDisksProperty(const QString &drive) const;
    /*!
     * Get removable drive name.
     */
    MusicDeviceInfoItems getRemovableDrive();

#ifdef Q_OS_UNIX
private Q_SLOTS:
    void readData();

private:
    QProcess* m_dfProcess;
#endif
private:
    MusicDeviceInfoItems m_items;

};

#endif // MUSICDEVICEINFOCORE_H
