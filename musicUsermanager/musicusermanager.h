#ifndef MUSICUSERMANAGER_H
#define MUSICUSERMANAGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTime>
#include <QMenu>
#include "musicabstractmovedialog.h"

class MusicUserModel;

namespace Ui {
class MusicUserManager;
}

class MUSIC_USER_EXPORT MusicUserManager : public QDialog
{
    Q_OBJECT
public:
    explicit MusicUserManager(QWidget *parent = 0);
    ~MusicUserManager();

    void setUserUID(const QString &uid);

signals:
    void userStateChanged(const QString &uid, const QString &icon);

public slots:
    void musicUserLogoff();
    void popupUserRecordWidget();
    void resetUserName(const QString &name);
    virtual int exec();
    virtual void leaveEvent(QEvent *event) override;

protected:
    void createUserTime() const;
    void createButtonPopMenu();

    Ui::MusicUserManager *ui;
    MusicUserModel* m_userModel;
    QString m_currentUserUID;
    QTime m_time;
    QMenu m_popMenu;


};

#endif // MUSICUSERMANAGER_H
