#ifndef MUSICRUNAPPLICATION_H
#define MUSICRUNAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
    */
    MusicRunApplication(int &argc, char **argv, bool GUIenabled = true);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(const QString &id, int &argc, char **argv);

#if QT_VERSION < 0x050000
    /*!
     * Object contsructor.
    */
    MusicRunApplication(int &argc, char **argv, Type type);
#  if defined(Q_WS_X11)
    /*!
     * Object contsructor.
    */
    MusicRunApplication(Display *dpy, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap= 0);
    /*!
     * Object contsructor.
    */
    MusicRunApplication(Display *dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
#  endif
#endif

    /*!
     * Initialize the current server.
    */
    void initialize(bool dummy = true);
    /*!
     * Check the current server is running or not.
    */
    bool isRunning() const;
    /*!
     * Get current server id.
    */
    QString id() const;

    /*!
     * Set current active window.
    */
    void setActivationWindow(QWidget *aw, bool activateOnMessage = true);
    /*!
     * Get current active window.
    */
    QWidget *activationWindow() const;

Q_SIGNALS:
    /*!
     * Emit when the current message received.
    */
    void messageReceived(const QString &message);

public Q_SLOTS:
    /*!
     * Emit when the current message received.
    */
    bool sendMessage(const QString &message, int timeout = 5000);
    /*!
     * Selected current active window.
    */
    void activateWindow();

private:
    /*!
     * Init the system parameter.
    */
    void sysInit(const QString &appId = QString());

    TTK_DECLARE_PRIVATE(MusicRunApplication)

};

#endif // MUSICRUNAPPLICATION_H
