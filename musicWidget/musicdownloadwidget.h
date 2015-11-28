#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicdownloadquerythread.h"

class MusicDownLoadQueryThread;

namespace Ui {
class MusicDownloadWidget;
}

class MUSIC_WIDGET_EXPORT MusicDownloadWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicDownloadWidget(QWidget *parent = 0);
    virtual ~MusicDownloadWidget();

    void setSongName(const QString &name, QueryType type);

signals:
public slots:
    virtual int exec();
    void queryAllFinished();
    void downloadDirSelected();
    void startToDownload();

protected:
    void initWidget();
    void setMusicSTState(bool show);
    void setMusicHDState(bool show);
    void setMusicSDState(bool show);

    Ui::MusicDownloadWidget *ui;
    MusicDownLoadQueryThread *m_downloadThread;
    QueryType m_queryType;

};

#endif // MUSICDOWNLOADWIDGET_H
