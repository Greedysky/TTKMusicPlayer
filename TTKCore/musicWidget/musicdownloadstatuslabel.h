#ifndef MUSICDOWNLOADSTATUSLABEL_H
#define MUSICDOWNLOADSTATUSLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"
#include "musicdownloadquerythreadabstract.h"

class MusicApplication;

/*! @brief The class of the show download state.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicDownloadStatusLabel : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadStatusLabel(QObject *w);
    /*!
     * Object contsructor.
     */
    ~MusicDownloadStatusLabel();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void musicCheckHasLrcAlready();
    /*!
     * Check the current song already has lrc or not,
     * if not just download it.
     */
    bool checkSettingParameterValue() const;
    /*!
     * Check the setting has open inline or desktop lrc on or not.
     */

public Q_SLOTS:
    void musicHaveNoLrcAlready();
    /*!
     * Download current music lrc from net.
     */
    void showDownLoadInfoFor(MusicObject::DownLoadType type);
    /*!
     * Show download state type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void showDownLoadInfoFinished(const QString &type);
    /*!
     * Show download state string type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void networkConnectionStateChanged(bool state);
    /*!
     * Show current net connect state changed by net thread.
     */

protected:
    bool m_previousState;
    MusicApplication *m_parentWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadLrcThread;

};

#endif // MUSICDOWNLOADSTATUSLABEL_H
