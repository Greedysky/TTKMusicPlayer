#ifndef MUSICRUNAPPLICATION_H
#define MUSICRUNAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QApplication>
#include "musicprivate.h"
#include "musicrunglobaldefine.h"

class MusicRunApplicationPrivate;

/*! @brief The class of the music run application.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT MusicRunApplication : public QApplication
{
    Q_OBJECT
public:
    MusicRunApplication(int &argc, char **argv, bool GUIenabled = true);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(const QString &id, int &argc, char **argv);
    /*!
     * Object contsructor.
    */
#if QT_VERSION < 0x050000
    MusicRunApplication(int &argc, char **argv, Type type);
    /*!
     * Object contsructor.
    */
#  if defined(Q_WS_X11)
    MusicRunApplication(Display *dpy, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap= 0);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(Display *dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    /*!
     * Object contsructor.
    */
#  endif
#endif

    void initialize(bool dummy = true);
    /*!
     * Initialize the current server.
    */
    bool isRunning() const;
    /*!
     * Check the current server is running or not.
    */
    QString id() const;
    /*!
     * Get current server id.
    */

    void setActivationWindow(QWidget *aw, bool activateOnMessage = true);
    /*!
     * Set current active window.
    */
    QWidget *activationWindow() const;
    /*!
     * Get current active window.
    */

Q_SIGNALS:
    void messageReceived(const QString &message);
    /*!
     * Emit when the current message received.
    */

public Q_SLOTS:
    bool sendMessage(const QString &message, int timeout = 5000);
    /*!
     * Emit when the current message received.
    */
    void activateWindow();
    /*!
     * Selected current active window.
    */

private:
    void sysInit(const QString &appId = QString());
    /*!
     * Init the system parameter.
    */
    TTK_DECLARE_PRIVATE(MusicRunApplication)

};

#endif // MUSICRUNAPPLICATION_H
