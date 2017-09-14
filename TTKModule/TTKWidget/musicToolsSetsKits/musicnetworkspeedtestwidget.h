#ifndef MUSICNETWORKTESTSPEEDWIDGET_H
#define MUSICNETWORKTESTSPEEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicobject.h"
#include "musicabstractmovewidget.h"

class QActionGroup;
class MusicDataDownloadThread;
class MusicNetworkSpeedTestThread;
class MusicNetworkSpeedSuspensionWidget;

const QString testUrl = "Mnk5YVlmSkd3VzB3WTYvMkFlNVZpa0pycmFaSEFyWTJuSDNNV1pvOXBWUWluVVduL0cveENGSHM1OGJnL0MxWmV2MlQ2ZEx5ak5sUDlWREtnSC9tSmxQOXRtY0N1ZHBXeWlzRmtaNXBMWUUzM09HOFYrYkJJdTZ6R3dUbDVyQmRpMERmYy9VVmVCeHUvdFhL";

namespace Ui {
class MusicNetworkSpeedTestWidget;
}

/*! @brief The class of the network speed test widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkSpeedTestWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNetworkSpeedTestWidget(QWidget *parent = 0);

    virtual ~MusicNetworkSpeedTestWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();

private Q_SLOTS:
    /*!
     * Get current upload and download speed data.
     */
    void networkData(ulong upload, ulong download);
    /*!
     * Open suspension widget.
     */
    void suspensionOpen();
    /*!
     * Network test start.
     */
    void networkTestStart();
    /*!
     * Network test stop.
     */
    void networkTestStop();
    /*!
     * Select diff network interface action.
     */
    void actionTriggered(QAction *action);

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Set button popup menu.
     */
    void settingButton();

    Ui::MusicNetworkSpeedTestWidget *m_ui;
    QTimer m_testTimer;
    ulong m_testAverage, m_totalUp, m_totalDown;
    MusicNetworkSpeedTestThread *m_thread;
    MusicDataDownloadThread *m_testDownload;
    MusicNetworkSpeedSuspensionWidget *m_suspension;
    QActionGroup *m_actionGroup;

};

#endif // MUSICNETWORKTESTSPEEDWIDGET_H
