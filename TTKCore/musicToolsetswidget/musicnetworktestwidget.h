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
#include "musicabstractmovewidget.h"

#include <QTimer>

class QActionGroup;
class MusicNetworkTestThread;
class MusicDataDownloadThread;
class MusicNetworkSuspensionWidget;

const QString testUrl = "Mnk5YVlmSkd3VzB3WTYvMkFlNVZpa0pycmFaSEFyWTJuSDNNV1pvOXBWUWluVVduL0cveENGSHM1OGJnL0MxWmV2MlQ2ZEx5ak5sUDlWREtnSC9tSmxQOXRtY0N1ZHBXeWlzRmtaNXBMWUUzM09HOFYrYkJJdTZ6R3dUbDVyQmRpMERmYy9VVmVCeHUvdFhL";
const QString testName = "tempCache.tt";

namespace Ui {
class MusicNetworkTestWidget;
}

/*! @brief The class of the network test widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkTestWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicNetworkTestWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkTestWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */

private Q_SLOTS:
    void networkData(ulong upload, ulong download);
    /*!
     * Get current upload and download speed data.
     */
    void suspensionOpen();
    /*!
     * Open suspension widget.
     */
    void networkTestStart();
    /*!
     * Network test start.
     */
    void networkTestStop();
    /*!
     * Network test stop.
     */
    void actionTriggered(QAction *action);
    /*!
     * Select diff network interface action.
     */

protected:
    void settingButton();
    /*!
     * Set button popup menu.
     */

    Ui::MusicNetworkTestWidget *ui;
    QTimer m_testTimer;
    ulong m_testAverage, m_totalUp, m_totalDown;
    MusicNetworkTestThread *m_thread;
    MusicDataDownloadThread *m_testDownload;
    MusicNetworkSuspensionWidget *m_suspension;
    QActionGroup *m_actionGroup;

};

#endif // MUSICNETWORKTESTWIDGET_H
