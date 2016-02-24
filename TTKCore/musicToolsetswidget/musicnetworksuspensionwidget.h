#ifndef MUSICNETWORKSUSPENSIONWIDGET_H
#define MUSICNETWORKSUSPENSIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

class QActionGroup;
class MusicNetworkTestThread;

class MUSIC_TOOLSET_EXPORT MusicNetworkSuspensionWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicNetworkSuspensionWidget(QWidget *parent = 0);
    ~MusicNetworkSuspensionWidget();

    void setAvailableNewtworkNames(const QStringList &names);

private Q_SLOTS:
    void networkData(ulong upload, ulong download);
    void actionTriggered(QAction *action);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    MusicNetworkTestThread *m_thread;
    QString m_upload, m_download;
    QActionGroup *m_actionGroup;

};

#endif // MUSICNETWORKSUSPENSIONWIDGET_H
