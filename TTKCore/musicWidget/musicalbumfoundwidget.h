#ifndef MUSICALBUMFOUNDWIDGET_H
#define MUSICALBUMFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicdownloadquerythreadabstract.h"

class QCheckBox;

/*! @brief The class of album music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAlbumFoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicAlbumFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicAlbumFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to search founds.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */

protected:
    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    MusicDownLoadQueryThreadAbstract *m_downloadThread;

};

#endif // MUSICALBUMFOUNDWIDGET_H
