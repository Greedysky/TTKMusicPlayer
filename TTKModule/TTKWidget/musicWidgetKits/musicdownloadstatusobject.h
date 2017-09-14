#ifndef MUSICDOWNLOADSTATUSOBJECT_H
#define MUSICDOWNLOADSTATUSOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

class MusicApplication;

/*! @brief The class of the show download state object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicDownloadStatusObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadStatusObject(QObject *w);

    ~MusicDownloadStatusObject();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Check the current song already has lrc or not,
     * if not just download it.
     */
    void musicCheckHasLrcAlready();
    /*!
     * Check the setting has open inline or desktop lrc on or not.
     */
    bool checkSettingParameterValue() const;

public Q_SLOTS:
    /*!
     * Download current music lrc from net.
     */
    void musicHaveNoLrcAlready();
    /*!
     * Show download state type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void showDownLoadInfoFor(MusicObject::DownLoadMode type);
    /*!
     * Show download state string type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void showDownLoadInfoFinished(const QString &type);
    /*!
     * Show current net connect state changed by net thread.
     */
    void networkConnectionStateChanged(bool state);

protected:
    bool m_previousState;
    MusicApplication *m_parentWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadLrcThread;

};

#endif // MUSICDOWNLOADSTATUSOBJECT_H
