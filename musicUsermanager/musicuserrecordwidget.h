#ifndef MUSICUSERRECORDWIDGET_H
#define MUSICUSERRECORDWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicUserRecordWidget;
}

class QComboBox;
class MusicUserModel;

class MUSIC_USER_EXPORT MusicUserRecordWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicUserRecordWidget(QWidget *parent = 0);
    ~MusicUserRecordWidget();

    void setUserModel(MusicUserModel *model, const QString &uid);

signals:
    void resetUserName(const QString &name);
    void userIconChanged(const QString &uid, const QString &icon);

public slots:
    void cityComboBoxIndexChanged(const QString &city);
    void confirmButtonClickedF();
    void openFileButtonClickedS();
    void changeVerificationCodeT();
    void confirmButtonClickedT();
    void checkPwdStrength(int code);
    virtual int exec();

protected:
    void initTabF();
    void initTabS();
    void initTabT();
    void setComboboxText(QComboBox *object, const QString &text);

    Ui::MusicUserRecordWidget *ui;
    MusicUserModel *m_userModel;

};

#endif // MUSICUSERRECORDWIDGET_H
