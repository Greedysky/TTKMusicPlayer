#ifndef MUSICDOWNLOADMGMTWIDGET_H
#define MUSICDOWNLOADMGMTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"
#include "musicdownloadquerythreadabstract.h"

namespace Ui{
class MusicDownloadResetWidget;
}

class MUSIC_WIDGET_EXPORT MusicDownloadResetWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicDownloadResetWidget(QWidget *parent = 0);
    ~MusicDownloadResetWidget();

protected:
    Ui::MusicDownloadResetWidget* ui;

};


class MUSIC_WIDGET_EXPORT MusicDownloadMgmtWidget : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadMgmtWidget(QObject *parent = 0);

    void setSongName(const QString &name, MusicDownLoadQueryThreadAbstract::QueryType type);
    /*!
     * Set current name to search and download musics.
     */

protected:
    QWidget *m_parentClass;

};

#endif // MUSICDOWNLOADMGMTWIDGET_H
