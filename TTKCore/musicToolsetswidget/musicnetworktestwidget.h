#ifndef MUSICNETWORKTESTWIDGET_H
#define MUSICNETWORKTESTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"
#include "musicabstractmovedialog.h"

class MusicNetworkTestThread;

namespace Ui {
class MusicNetworkTestWidget;
}

class MUSIC_TOOLSET_EXPORT MusicNetworkTestWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicNetworkTestWidget(QWidget *parent = 0);
    ~MusicNetworkTestWidget();

public Q_SLOTS:
    virtual int exec();

private Q_SLOTS:
    void networkData(long upload, long download);
    void suspensionOpen();
    void networkTestStart();

protected:
    Ui::MusicNetworkTestWidget *ui;
    MusicNetworkTestThread *m_thead;
    long m_totalUp, m_totalDown;

};

#endif // MUSICNETWORKTESTWIDGET_H
