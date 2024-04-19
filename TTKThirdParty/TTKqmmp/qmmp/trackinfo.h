/***************************************************************************
 *   Copyright (C) 2018-2024 by Ilya Kotov                                 *
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
#include <QVariant>
#include "qmmp.h"
#include "regularexpression.h"

/*! @brief The TrackInfo class stores metadata and other information about track.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT TrackInfo
{
public:
    /*!
     * Constructs a new empty TrackInfo object.
     */
    TrackInfo() = default;
    /*!
     * Constructs a new empty TrackInfo that with file \b path (local path or URL).
     */
    explicit TrackInfo(const QString &path);
    /*!
     * Constructs a new TrackInfo that is a copy of the given \b other
     */
    TrackInfo(const TrackInfo &other);

    /*!
     * This enum describes stored parts of track information.
     */
    enum Part
    {
        MetaData = 0x1,                                       /*!< Metadata (artist, album, etc). */
        Properties = 0x2,                                     /*!< File/Stream properties. */
        ReplayGainInfo = 0x4,                                 /*!< ReplayGain information. */
        AllParts = MetaData | Properties | ReplayGainInfo     /*!< All parts. */
    };

    Q_DECLARE_FLAGS(Parts, Part)
    /*!
     * Makes a copy of the given \b info
     */
    TrackInfo &operator=(const TrackInfo &info);
    /*!
     * Returns \b true if this FileInfo object refers to \b info; otherwise returns \b false.
     */
    bool operator==(const TrackInfo &info) const;
    /*!
     * Returns \b false if this FileInfo object refers to \b info; otherwise returns \b true.
     */
    bool operator!=(const TrackInfo &info) const;
    /*!
     * Returnds file duration in milliseconds.
     */
    qint64 duration() const;
    /*!
     * Returns \b true if stream/file has no infomation and empty path, otherwise returns \b false.
     */
    bool isEmpty() const;
    /*!
     * Returns file path or stream URL.
     */
    const QString &path() const;
    /*!
     * Returns the metdata string associated with the given \b key.
     */
    const QString value(Qmmp::MetaData key) const;
    /*!
     * Returns the track property string associated with the given \b key.
     */
    const QString value(Qmmp::TrackProperty key) const;
    /*!
     * Returns the ReplayGain value associated with the given \b key.
     */
    double value(Qmmp::ReplayGainKey key) const;
    /*!
     * Returns the track metadata map.
     */
    const QMap<Qmmp::MetaData, QString> &metaData() const;
    /*!
     * Returns the track property map.
     */
    const QMap<Qmmp::TrackProperty, QString> &properties() const;
    /*!
     * Returns the ReplayGain information.
     */
    const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo() const;
    /*!
     * Returns parts of the stored track information.
     */
    Parts parts() const;
    /*!
     * Sets file duration to \b duration in milliseconds.
     */
    void setDuration(qint64 duration);
    /*!
     * Sets metadata \b key to \b value.
     */
    void setValue(Qmmp::MetaData key, const QVariant &value);
    /*!
     * Sets track property \b key to \b value.
     */
    void setValue(Qmmp::TrackProperty key, const QVariant &value);
    /*!
     * Sets ReplayGain value \b key to \b value.
     */
    void setValue(Qmmp::ReplayGainKey key, double value);
    /*!
     * Sets ReplayGain value \b key to \b value (string is converted to double).
     */
    void setValue(Qmmp::ReplayGainKey key, const QString &value);
    /*!
     * Sets metadata to \b metaData.
     */
    void setValues(const QMap<Qmmp::MetaData, QString> &metaData);
    /*!
     * Sets track properties to \b properties.
     */
    void setValues(const QMap<Qmmp::TrackProperty, QString> &properties);
    /*!
     * Sets ReplayGain information to \b replayGainInfo.
     */
    void setValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo);
    /*!
     * Updates existing metadata using values from \b metaData.
     */
    void updateValues(const QMap<Qmmp::MetaData, QString> &metaData);
    /*!
     * Updates existing track properties using values from \b properties.
     */
    void updateValues(const QMap<Qmmp::TrackProperty, QString> &properties);
    /*!
     * Updates existing ReplayGain information using values from \b replayGainInfo.
     */
    void updateValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo);
    /*!
     * Sets file path or stream URL to \b path.
     */
    void setPath(const QString &path);
    /*!
     * Removes specified parts \b parts of the stored track information.
     */
    void clear(Parts parts);
    /*!
     * Removes all track information including path.
     */
    void clear();

private:
    QMap<Qmmp::MetaData, QString> m_metaData;
    QMap<Qmmp::TrackProperty, QString> m_properties;
    QMap<Qmmp::ReplayGainKey, double> m_replayGainInfo;
    Parts m_parts = Parts();
    QString m_path;
    qint64 m_duration = 0;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TrackInfo::Parts)


#endif
