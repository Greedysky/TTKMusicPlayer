#ifndef TTKMUSICUTILS_H
#define TTKMUSICUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

#define APP_PKG_NAME    "org/greedysky/ttkmobile/TTKMobile"

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

    Q_INVOKABLE QVariant getValue(const QString &key) const;
    /*!
     * Get global setting vale.
     */
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value) const;
    /*!
     * Set global setting vale.
     */

    Q_INVOKABLE QString getRoot() const;
    /*!
     * Get root.
     */
    Q_INVOKABLE QString getRootPath() const;
    /*!
     * Get root path.
     */
    Q_INVOKABLE QString getCachedPath() const;
    /*!
     * Get cached path.
     */

    Q_INVOKABLE void showWindowNotify(int value);
    /*!
     * Show window notify dialog.
     */
    Q_INVOKABLE void showWindowNotify(const QString &title, const QString &text, int value);
    /*!
     * Show window notify dialog.
     */
    Q_INVOKABLE void showMessageBox(const QString &text, const QString &title = 0, QWidget *parent = 0);
    /*!
     * Show message box.
     */
    Q_INVOKABLE bool currentNetIsWifi();
    /*!
     * Check current network is wifi.
     */
    Q_INVOKABLE void setNetworkBlockNotWifi();
    /*!
     * Set network block if it is not Wifi.
     */
    Q_INVOKABLE void updateApplicationDialog();
    /*!
     * Update application dialog.
     */

    Q_INVOKABLE QString normalizeTime(qint64 time, const QString &format);
    /*!
     * Transfer integer time to string by format.
     */
    Q_INVOKABLE QString fromMSecsSinceEpoch(qint64 msecs, const QString &format);
    /*!
     * Transform ms time from utc since epoch by format.
     */
    Q_INVOKABLE QString size2Label(qint64 size);
    /*!
     * Transform value to cp value.
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

    Q_INVOKABLE bool removeCacheDir(const QString &dir);
    /*!
     * Remove cache dir in ecursive.
     */
    Q_INVOKABLE bool removeDir(const QString &dir);
    /*!
     * Remove dir in ecursive.
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
