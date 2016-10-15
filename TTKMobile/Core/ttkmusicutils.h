#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

class MusicSongTag;

/*! @brief The class of the music utils.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicUtils : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicUtils(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKMusicUtils();

    Q_INVOKABLE QString getRoot() const;
    /*!
     * Get root.
     */
    Q_INVOKABLE QString getRootPath() const;
    /*!
     * Get root path.
     */
    Q_INVOKABLE QString normalizeTime(qint64 time, const QString &format);
    /*!
     * Transfer integer time to string by format.
     */

    Q_INVOKABLE int stringFontWidth(const QFont &font, const QString &str);
    /*!
     * Get string width by font.
     */

    Q_INVOKABLE bool readTagFromFile(const QString &path);
    /*!
     * Open tag reader object by file path.
     */
    Q_INVOKABLE QString getTag(int index);
    /*!
     * Get music song tag.
     */
    Q_INVOKABLE void closeTagFromFile();
    /*!
     * Close tag reader object.
     */

    void checkTheFileNeededExist();
    /*!
     * Check the related file is exist.
     */
    void dirIsExist(const QString &name);
    /*!
     * Check current dir is exist, no, just create it.
     */
    void checkTheDirectoryExist();
    /*!
     * Check the related dir is exist.
     */

protected:
    MusicSongTag *m_songTag;

};

#endif // TTKMUSICUTILS_H
