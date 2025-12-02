#ifndef MUSICSONGMETA_H
#define MUSICSONGMETA_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicglobaldefine.h"
#include <qmmp/tagmeta.h>

struct MusicMeta;

/*! @brief The class of the music song meta.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongMeta
{
public:
    /*!
     * Object constructor.
     */
    MusicSongMeta() noexcept;
    /*!
     * Object destructor.
     */
    ~MusicSongMeta() noexcept;

    /*!
     * Read media file to anaylsis.
     */
    bool read(const QString &url);
    /*!
     * Save media tags to file.
     */
    bool save();

    /*!
     * Read media file path.
     */
    QString filePath() const noexcept;
    /*!
     * Get file url.
     */
    QString fileBasePath() noexcept;
    /*!
     * Get file related path.
     */
    QString fileRelatedPath() noexcept;

    /*!
     * Get artist name.
     */
    QString artist() noexcept;
    /*!
     * Get song title.
     */
    QString title() noexcept;
    /*!
     * Get song album.
     */
    QString album() noexcept;
    /*!
     * Get song comment.
     */
    QString comment() noexcept;
    /*!
     * Get song year.
     */
    QString year() noexcept;
    /*!
     * Get song track number.
     */
    QString trackNum() noexcept;
    /*!
     * Get song genre.
     */
    QString genre() noexcept;
    /*!
     * Get song rating.
     */
    QString rating() noexcept;
    /*!
     * Get song channel.
     */
    QString channel() noexcept;
    /*!
     * Read media description.
     */
    QString description() noexcept;

    /*!
     * Set artist name.
     */
    void setArtist(const QString &artist) noexcept;
    /*!
     * Set song title.
     */
    void setTitle(const QString &title) noexcept;
    /*!
     * Set song album.
     */
    void setAlbum(const QString &album) noexcept;
    /*!
     * Set song comment.
     */
    void setComment(const QString &comment) noexcept;
    /*!
     * Set song year.
     */
    void setYear(const QString &year) noexcept;
    /*!
     * Set song track number.
     */
    void setTrackNum(const QString &track) noexcept;
    /*!
     * Set song genre.
     */
    void setGenre(const QString &genre) noexcept;
    /*!
     * Set song rating.
     */
    void setRating(const QString &rating) noexcept;

public:
    /*!
     * Set song image cover artist.
     */
    void setCover(const QPixmap &cover);
    /*!
     * Set song image cover artist.
     */
    void setCover(const QByteArray &data);
    /*!
     * Get song image cover artist.
     */
    QPixmap cover() noexcept;
    /*!
     * Get song lyrics buffer data.
     */
    QString lyrics() noexcept;
    /*!
     * Get song sample rate.
     */
    QString sampleRate() noexcept;
    /*!
     * Get song bitrate.
     */
    QString bitrate() noexcept;
    /*!
     * Get song time duration.
     */
    QString duration() noexcept;

public:
    MusicSongMeta(const MusicSongMeta &other) noexcept;
    MusicSongMeta(MusicSongMeta &&other) noexcept;

    MusicSongMeta& operator= (const MusicSongMeta &other) noexcept;
    MusicSongMeta& operator= (MusicSongMeta &&other) noexcept;

public:
    /*!
     * Set media song meta index offset.
     */
    void setSongMetaIndex(int index) noexcept;
    /*!
     * Get media song meta count.
     */
    int songMetaCount() const noexcept;

private:
    /*!
     * Clear media song meta.
     */
    void clearSongMeta() noexcept;
    /*!
     * Get media song meta pointer.
     */
    MusicMeta *songMeta() noexcept;
    /*!
     * Format legal data string.
     */
    QString formatString(TagMeta::Type type) noexcept;
    /*!
     * Read other taglib not by plugin.
     */
    bool readInformation();
    /*!
     * Save other taglib not by plugin.
     */
    bool saveInformation();

    int m_offset;
    QString m_path;
    QList<MusicMeta*> m_songMetas;

};

#endif // MUSICSONGMETA_H
