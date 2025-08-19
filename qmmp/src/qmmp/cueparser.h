/***************************************************************************
 *   Copyright (C) 2008-2025 by Ilya Kotov                                 *
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

#ifndef CUEPARSER_H
#define CUEPARSER_H

#include <QByteArray>
#include <QStringList>
#include <QFileInfo>
#include "trackinfo.h"

class QmmpTextCodec;

/*! @brief The CueParser class provides CUE parser.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT CueParser
{
public:
    /*!
     * Constructs empty CUE parser.
     */
    CueParser() = default;
    /*!
     * Constructs CUE parser and parses given content.
     * \param data Content of CUE file.
     * \param codecName Codec name ("UTF-8" by default).
     */
    CueParser(const QByteArray &data, const QByteArray &codecName = QByteArray());
    /*!
     * Object destructor.
     */
    ~CueParser();

    /*!
     * Parses CUE file content.
     * \param data Content of CUE file.
     * \param codecName Codec name ("UTF-8" by default).
     */
    void loadData(const QByteArray &data, const QByteArray &codecName = "UTF-8");
    /*!
     * Parses CUE file content.
     * \param data Content of CUE file.
     * \param codec Codec.
     */
    void loadData(const QByteArray &data, QmmpTextCodec *codec);
    /*!
     * Creates playlist using parsed CUE metadata.
     * \param track Track number to return (-1 for all playlist or 1..n for specific track)
     */
    QList<TrackInfo*> createPlayList(int track = -1) const;
    /*!
     * Returns a list of data files.
     */
    const QStringList &files() const;
    /*!
     * Returns offset in milliseconds of the given \b track.
     */
    qint64 offset(int track) const;
    /*!
     * Returns duration in milliseconds of the given \b track.
     */
    qint64 duration(int track) const;
    /*!
     * Returns data file of the given \b track.
     */
    QString file(int track) const;
    /*!
     * Returns URL of the given \b track.
     */
    QString url(int track) const;
    /*!
     * Returns number of tracks.
     */
    int count() const;
    /*!
     * Returns \b true if parser has no data, otherwise returns \b false.
     */
    bool isEmpty() const;
    /*!
     * Returns information for the given \b track.
     */
    const TrackInfo *info(int track) const;
    /*!
     * Sets duration for the given content file.
     * \param file Content file path.
     * \param duration Duration in milliseconds.
     */
    void setDuration(const QString &file, qint64 duration);
    /*!
     * Sets duration for the single content file (useful for embedded CUE).
     * \param duration Duration in milliseconds.
     */
    void setDuration(qint64 duration);
    /*!
     * Sets audio properties for the given content file.
     * \param file Content file path.
     * \param properties Audio properties.
     */
    void setProperties(const QString &file, const QMap<Qmmp::TrackProperty, QString> &properties);
    /*!
     * Sets audio properties for the single content file (useful for embedded CUE).
     * \param properties Audio properties.
     */
    void setProperties(const QMap<Qmmp::TrackProperty, QString> &properties);
    /*!
     * Updates metadata of the \b track.
     * \param track Track number (1..n).
     * \param key Metadata key.
     * \param value Metadata value.
     */
    void setMetaData(int track, Qmmp::MetaData key, const QVariant &value);
    /*!
     * Sets URLs for CUE tracks in the following format:
     * {scheme}://{path}#{track number}
     * \param scheme URL scheme.
     * \param path Content file path.
     */
    void setUrl(const QString &scheme, const QString &path);
    /*!
     * Removes all parsed data.
     */
    void clear();

private:
    struct CUETrack
    {
        TrackInfo info;
        QString file;
        qint64 offset = 0;
    };
	
    QList<CUETrack *> m_tracks;
    QStringList m_files;

    QStringList splitLine(const QString &line);
    qint64 getLength(const QString &str);

};

#endif
