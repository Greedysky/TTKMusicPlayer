/***************************************************************************
 *   Copyright (C) 2018-2019 by Ilya Kotov                                 *
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

#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <QMap>
#include <QString>
#include <QVariant>
#include "qmmp.h"

class QMMP_EXPORT TrackInfo
{
public:
    TrackInfo();
    TrackInfo(const QString &path);
    TrackInfo(const TrackInfo &other);
    ~TrackInfo();

    enum Part
    {
        NoParts = 0x0,
        MetaData = 0x1,
        Properties = 0x2,
        ReplayGainInfo = 0x4,
        AllParts = MetaData | Properties | ReplayGainInfo
    };

    Q_DECLARE_FLAGS(Parts, Part)

    TrackInfo &operator=(const TrackInfo &info);
    bool operator==(const TrackInfo &info) const;
    bool operator!=(const TrackInfo &info) const;

    qint64 duration () const;
    bool isEmpty() const;
    const QString path() const;
    const QString value(Qmmp::MetaData key) const;
    const QString value(Qmmp::TrackProperty key) const;
    double value(Qmmp::ReplayGainKey key) const;
    const QMap<Qmmp::MetaData, QString> &metaData() const;
    const QMap<Qmmp::TrackProperty, QString> &properties() const;
    const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo() const;
    Parts parts() const;
    void setDuration(qint64 duration);
    void setValue(Qmmp::MetaData key, const QVariant &value);
    void setValue(Qmmp::TrackProperty key, const QVariant &value);
    void setValue(Qmmp::ReplayGainKey key, double value);
    void setValue(Qmmp::ReplayGainKey key, const QString &value);
    void setValues(const QMap<Qmmp::MetaData, QString> &metaData);
    void setValues(const QMap<Qmmp::TrackProperty, QString> &properties);
    void setValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo);
    void updateValues(const QMap<Qmmp::MetaData, QString> &metaData);
    void updateValues(const QMap<Qmmp::TrackProperty, QString> &properties);
    void updateValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo);
    void setPath(const QString &path);
    void clear(Parts parts);
    void clear();

private:
    QMap<Qmmp::MetaData, QString> m_metaData;
    QMap<Qmmp::TrackProperty, QString> m_properties;
    QMap<Qmmp::ReplayGainKey, double> m_replayGainInfo;
    Parts m_parts;
    QString m_path;
    qint64 m_duration;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TrackInfo::Parts)


#endif
