#ifndef MUSICTIMERWIDGET_H
#define MUSICTIMERWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicTimerWidget;
}

class MUSIC_TOOLSET_EXPORT MusicTimerWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicTimerWidget(QWidget *parent = 0);
    virtual ~MusicTimerWidget();

    void setSongStringList(const QStringList &list);

signals:
    void timerParameterChanged();

public slots:
    void changeFirstWidget();
    void changeSecondWidget();
    void changeThreeWidget();
    void commitTheResults();
    void setEnabledControlFalse1();
    void setEnabledControlTrue1();
    void setEnabledControlFalse2();
    void setEnabledControlTrue2();
    void setEnabledControlFalse3();
    void setEnabledControlTrue3();
    virtual int exec();

protected:
    Ui::MusicTimerWidget *ui;

    void initFirstWidget();
    void initSecondWidget();
    void initThreeWidget();
    void initComboParameter();
    void initParemeter();
    void writeParemeter() const;

    QStringList m_hour;
    QStringList m_second;
    QStringList m_repeat;

};

#endif // MUSICTIMERWIDGET_H
