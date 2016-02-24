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

class MusicNetworkTestThread;

class MUSIC_TOOLSET_EXPORT MusicNetworkSuspensionWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicNetworkSuspensionWidget(QWidget *parent = 0);
    ~MusicNetworkSuspensionWidget();

private Q_SLOTS:
    void networkData(ulong upload, ulong download);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    MusicNetworkTestThread *m_thread;

};

#endif // MUSICNETWORKSUSPENSIONWIDGET_H
