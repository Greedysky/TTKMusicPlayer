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

class MUSIC_RUN_EXPORT MusicRunApplication : public QApplication
{
    Q_OBJECT
public:
    MusicRunApplication(int &argc, char **argv, bool GUIenabled = true);
    MusicRunApplication(const QString &id, int &argc, char **argv);
#if QT_VERSION < 0x050000
    MusicRunApplication(int &argc, char **argv, Type type);
#  if defined(Q_WS_X11)
    MusicRunApplication(Display *dpy, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    MusicRunApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap= 0);
    MusicRunApplication(Display *dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
#  endif
#endif

    void initialize(bool dummy = true);

    bool isRunning() const;
    QString id() const;

    void setActivationWindow(QWidget *aw, bool activateOnMessage = true);
    QWidget* activationWindow() const;

Q_SIGNALS:
    void messageReceived(const QString &message);

public Q_SLOTS:
    bool sendMessage(const QString &message, int timeout = 5000);
    void activateWindow();

private:
    void sysInit(const QString &appId = QString());
    TTK_DECLARE_PRIVATE(MusicRunApplication)

};

#endif // MUSICRUNAPPLICATION_H
