#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "tagreadandwrite.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music song tag.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSongTag
{
public:
    MusicSongTag();
    /*!
     * Object contsructor.
     */
    explicit MusicSongTag(const QString &file);
    /*!
     * Object contsructor.
     */
    ~MusicSongTag();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool readFile(const QString &file);
    /*!
     * Read music file to anaylsis.
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
    bool setArtist(const QString &artist);
    /*!
     * Set artist name.
     */
    bool setTitle(const QString &title);
    /*!
     * Set song title.
     */
    bool setAlbum(const QString &album);
    /*!
     * Set song album.
     */
    bool setComment(const QString &comment);
    /*!
     * Set song comment.
     */
    bool setYear(const QString &year);
    /*!
     * Set song year.
     */
    bool setTrackNum(const QString &track);
    /*!
     * Set song track number.
     */
    bool setGenre(const QString &genre);
    /*!
     * Set song genre.
     */
    bool setCover(const QByteArray &data);
    /*!
     * Set song image cover art.
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
    bool readOtherTaglibNotSupport(const QString &path);
    /*!
     * Read other taglib not support file format.
     */
    QString getNotSupportedPluginPath(const QString &format);
    /*!
     * Get not supported plugin path.
     */

    QString m_filePath;
    TagReadAndWrite *m_tag;
    QMap<TagReadAndWrite::MusicTag, QString> m_parameters;

};

#endif // MUSICSONGTAG_H
