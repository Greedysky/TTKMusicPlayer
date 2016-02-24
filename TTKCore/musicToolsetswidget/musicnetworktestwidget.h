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

#include <QTimer>

class QActionGroup;
class MusicNetworkTestThread;
class MusicDataDownloadThread;
class MusicNetworkSuspensionWidget;

const QString testUrl = "http://mirrors.ustc.edu.cn/qtproject/archive/qt/5.5/5.5.1/"
                        "qt-opensource-windows-x86-mingw492-5.5.1.exe";
const QString testName = "tempCache.tt";

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
    void networkData(ulong upload, ulong download);
    void suspensionOpen();
    void networkTestStart();
    void networkTestStop();
    void actionTriggered(QAction *action);

protected:
    void settingButton();

    Ui::MusicNetworkTestWidget *ui;
    QTimer m_testTimer;
    ulong m_testAverage, m_totalUp, m_totalDown;
    MusicNetworkTestThread *m_thread;
    MusicDataDownloadThread *m_testDownload;
    MusicNetworkSuspensionWidget *m_suspension;
    QActionGroup *m_actionGroup;

};

#endif // MUSICNETWORKTESTWIDGET_H
