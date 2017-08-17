#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

/*! @brief The class of the music song tag.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSongTag
{
public:
    enum MusicTag
    {
        TAG_UNKNOWN = -1,
        TAG_TITLE = 0,   /*!< Title */
        TAG_ARTIST,      /*!< Artist  */
        TAG_ALBUMARTIST, /*!< Album artist  */
        TAG_ALBUM,       /*!< Album */
        TAG_COMMENT,     /*!< Comment */
        TAG_GENRE,       /*!< Genre */
        TAG_COMPOSER,    /*!< Composer */
        TAG_YEAR,        /*!< Year */
        TAG_TRACK,       /*!< Track number */
        TAG_DISCNUMBER,  /*!< Disc number */
        TAG_URL,         /*!< Stream url or local file path */
        TAG_LENGTH,      /*!< Length */
        TAG_FORMAT,      /*!< Format */
        TAG_SAMPLERATE,  /*!< Sample rate */
        TAG_MODE,        /*!< Mode */
        TAG_BITRATE,     /*!< Bitrate */
        TAG_CHANNEL,     /*!< Chrannel */
        TAG_COVER        /*!< Cove */
    };

    MusicSongTag();
    /*!
     * Object contsructor.
     */
    explicit MusicSongTag(const QString &file);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool read(const QString &file);
    /*!
     * Read music file to anaylsis.
     */
    bool save();
    /*!
     * Save music tags to music file.
     */

    QString getDecoder() const;
    /*!
     * Read music decoder name.
     */
    QString getFilePath() const;
    /*!
     * Read music file path.
     */
    /////////////////////////////////////////////
    QString getArtist() const;
    /*!
     * Get artist name.
     */
    QString getTitle() const;
    /*!
     * Get song title.
     */
    QString getAlbum() const;
    /*!
     * Get song album.
     */
    QString getComment() const;
    /*!
     * Get song comment.
     */
    QString getYear() const;
    /*!
     * Get song year.
     */
    QString getTrackNum() const;
    /*!
     * Get song track number.
     */
    QString getGenre() const;
    /*!
     * Get song genre.
     */
    QString getAlbumArtist() const;
    /*!
     * Get song album artist.
     */
    QString getComposer() const;
    /*!
     * Get song composer.
     */
    QString getChannel() const;
    /*!
     * Get song channel.
     */
    QString getURL() const;
    /*!
     * Get song file path.
     */
    /////////////////////////////////////////////
    void setArtist(const QString &artist);
    /*!
     * Set artist name.
     */
    void setTitle(const QString &title);
    /*!
     * Set song title.
     */
    void setAlbum(const QString &album);
    /*!
     * Set song album.
     */
    void setComment(const QString &comment);
    /*!
     * Set song comment.
     */
    void setYear(const QString &year);
    /*!
     * Set song year.
     */
    void setTrackNum(const QString &track);
    /*!
     * Set song track number.
     */
    void setGenre(const QString &genre);
    /*!
     * Set song genre.
     */
    /////////////////////////////////////////////
    void setCover(const QPixmap &pix);
    /*!
     * Set song image cover art.
     */
    void setCover(const QByteArray &data);
    /*!
     * Set song image cover art.
     */
    QPixmap getCover() const;
    /*!
     * Get song image cover art.
     */
    /////////////////////////////////////////////
    QString getSamplingRate() const;
    /*!
     * Get song sample rate.
     */
    QString getFormat() const;
    /*!
     * Get song format.
     */
    QString getMode() const;
    /*!
     * Get song mode.
     */
    QString getBitrate() const;
    /*!
     * Get song bitrate.
     */
    QString getLengthString() const;
    /*!
     * Get song time length.
     */
    /////////////////////////////////////////////

protected:
    QString findPluginPath() const;
    /*!
     * Find current pluin store path.
     */
    bool readOtherTaglib();
    /*!
     * Read other taglib not by plugin.
     */
    bool saveOtherTaglib();
    /*!
     * Save other taglib not by plugin.
     */

    QString m_filePath;
    QMap<MusicTag, QVariant> m_parameters;

};

#endif // MUSICSONGTAG_H
