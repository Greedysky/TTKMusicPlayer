#ifndef MUSICUSERDIALOG_H
#define MUSICUSERDIALOG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMouseEvent>
#include "musicabstractmovedialog.h"
#include "musicuserconfigmanager.h"

class MusicUserModel;

namespace Ui {
class MusicUserDialog;
}

class MUSIC_USER_EXPORT MusicUserDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicUserDialog(QWidget *parent = 0);
    ~MusicUserDialog();

signals:
    void userLoginSuccess(const QString &name);

public slots:
    void userLogin();
    void checkUserLogin();
    void registerUser();
    void checkRegisterUser();
    void userForgotPasswd();
    void checkUserForgotPasswd();
    void changeVerificationCode();
    void userComboBoxChanged(const QString &index);
    void userEditTextChanged(const QString &index);
    void checkToAutoLogin();
    void buttonClicked(int index);

protected:
    void firstStatckWidget();
    void secondStatckWidget();
    void thirdStatckWidget();
    void clearOriginData();
    void readFromUserConfig();
    void readFromUserSettings();
    void writeToUserConfig();
    void writeToUserSettings();
    void windowRectChanged(int index, const QRect &rect);

    Ui::MusicUserDialog *ui;
    MusicUserModel *m_userModel;
    MusicUserRecord m_record;
    QString m_userComboIndex;

};

#endif // MUSICUSERDIALOG_H
