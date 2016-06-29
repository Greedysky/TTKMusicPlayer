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

/*! @brief The class of the network suspension widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicNetworkSuspensionWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicNetworkSuspensionWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkSuspensionWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setAvailableNewtworkNames(const QStringList &names);
    /*!
     * Set available newtwork names.
     */

private Q_SLOTS:
    void networkData(ulong upload, ulong download);
    /*!
     * Send current upload and download speed data.
     */
    void actionTriggered(QAction *action);
    /*!
     * Select diff network interface action.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicNetworkTestThread *m_thread;
    QString m_upload, m_download;
    QActionGroup *m_actionGroup;

};

#endif // MUSICNETWORKSUSPENSIONWIDGET_H
