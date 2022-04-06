#ifndef MUSICSONGMETA_H
#define MUSICSONGMETA_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
#include <qmmp/tagwrapper.h>

struct MusicMeta;

/*! @brief The class of the music song meta.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongMeta
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
    bool read(const QString &url);
    /*!
     * Save music tags to music file.
     */
    bool save();

    /*!
     * Read music file path.
     */
    QString filePath() const;
    /*!
     * Get file url.
     */
    QString fileBasePath();
    /*!
     * Get file related path.
     */
    QString fileRelatedPath();

    /*!
     * Get artist name.
     */
    QString artist();
    /*!
     * Get song title.
     */
    QString title();
    /*!
     * Get song album.
     */
    QString album();
    /*!
     * Get song comment.
     */
    QString comment();
    /*!
     * Get song year.
     */
    QString year();
    /*!
     * Get song track number.
     */
    QString trackNum();
    /*!
     * Get song genre.
     */
    QString genre();
    /*!
     * Get song channel.
     */
    QString channel();
    /*!
     * Read music decoder name.
     */
    QString decoder();

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
    QPixmap cover();
    /*!
     * Get song sample rate.
     */
    QString sampleRate();
    /*!
     * Get song bitrate.
     */
    QString bitrate();
    /*!
     * Get song time duration.
     */
    QString duration();

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
     * Get music song meta count.
     */
    int songMetaCount() const;

protected:
    /*!
     * Clear music song meta.
     */
    void clearSongMeta();
    /*!
     * Get music song meta pointer.
     */
    MusicMeta *songMeta();
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
    QString m_path;
    QList<MusicMeta*> m_songMetas;

};

#endif // MUSICSONGMETA_H
