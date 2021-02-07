#ifndef MUSICSONGMETA_H
#define MUSICSONGMETA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include "tagwrapper.h"
#include "musicglobaldefine.h"

class MusicMeta;

/*! @brief The class of the music song meta.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSongMeta
{
    TTK_DECLARE_MODULE(MusicSongMeta)
public:
    /*!
     * Object contsructor.
     */
    MusicSongMeta();

    ~MusicSongMeta();

    /*!
     * Read music file to anaylsis.
     */
    bool read(const QString &file);
    /*!
     * Save music tags to music file.
     */
    bool save();

    /*!
     * Read music decoder name.
     */
    QString getDecoder() const;
    /*!
     * Read music file path.
     */
    QString getFilePath() const;
    /*!
     * Get file url.
     */
    QString getFileBasePath();
    /*!
     * Get file related path.
     */
    QString getFileRelatedPath();

    /*!
     * Get artist name.
     */
    QString getArtist();
    /*!
     * Get song title.
     */
    QString getTitle();
    /*!
     * Get song album.
     */
    QString getAlbum();
    /*!
     * Get song comment.
     */
    QString getComment();
    /*!
     * Get song year.
     */
    QString getYear();
    /*!
     * Get song track number.
     */
    QString getTrackNum();
    /*!
     * Get song genre.
     */
    QString getGenre();
    /*!
     * Get song channel.
     */
    QString getChannel();

    /*!
     * Set artist name.
     */
    void setArtist(const QString &artist);
    /*!
     * Set song title.
     */
    void setTitle(const QString &title);
    /*!
     * Set song album.
     */
    void setAlbum(const QString &album);
    /*!
     * Set song comment.
     */
    void setComment(const QString &comment);
    /*!
     * Set song year.
     */
    void setYear(const QString &year);
    /*!
     * Set song track number.
     */
    void setTrackNum(const QString &track);
    /*!
     * Set song genre.
     */
    void setGenre(const QString &genre);

public:
    /*!
     * Set song image cover artist.
     */
    void setCover(const QPixmap &pix);
    /*!
     * Set song image cover artist.
     */
    void setCover(const QByteArray &data);
    /*!
     * Get song image cover artist.
     */
    QPixmap getCover();
    /*!
     * Get song sample rate.
     */
    QString getSampleRate();
    /*!
     * Get song bitrate.
     */
    QString getBitrate();
    /*!
     * Get song time length.
     */
    QString getLengthString();

public:
    MusicSongMeta(const MusicSongMeta &other);
    MusicSongMeta(MusicSongMeta &&other);

    MusicSongMeta& operator= (const MusicSongMeta &other);
    MusicSongMeta& operator= (MusicSongMeta &&other);

public:
    /*!
     * Set music song meta index offset.
     */
    void setSongMetaIndex(int index);
    /*!
     * Get music song meta size.
     */
    int getSongMetaSize() const;

protected:
    /*!
     * Clear music song meta.
     */
    void clearSongMeta();
    /*!
     * Get music song meta pointer.
     */
    MusicMeta *getSongMeta();
    /*!
     * Find legal data string.
     */
    QString findLegalDataString(TagWrapper::Type type);
    /*!
     * Read other taglib not by plugin.
     */
    bool readInformation();
    /*!
     * Save other taglib not by plugin.
     */
    bool saveInformation();

    int m_offset;
    QString m_filePath;
    QList<MusicMeta*> m_songMetas;

};

#endif // MUSICSONGMETA_H
