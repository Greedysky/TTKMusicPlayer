#ifndef MUSICWEBRADIOWIDGET_H
#define MUSICWEBRADIOWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"

class QProcess;
class QListWidgetItem;
class MusicWebRadioDatabase;

namespace Ui {
class MusicWebRadioWidget;
}

class MUSIC_TOOL_EXPORT MusicWebRadioWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicWebRadioWidget(QWidget *parent = 0);
    virtual ~MusicWebRadioWidget();

signals:

public slots:
    void radioPlay();
    void radioStop();
    void radioVolume(int num);
    void timeout();
    void updateRadioList(const QString &category);
    void updateRecentList();
    void updateFavouriteList();
    void itemHasDoubleClicked(QListWidgetItem *item);
    void radioStandardOutput();
    void radioColletButton();
    void radioDiscolletButton();
    void show();

protected:
    virtual void closeEvent(QCloseEvent *event);
    void clearAllItems();
    Ui::MusicWebRadioWidget *ui;
    QProcess *m_radio;
    QTimer m_timer;
    int m_timerCount;
    QString m_radioUrl;
    QString m_currentRadioName;
    QIcon *m_collecticon;
    QIcon *m_discollecticon;
    MusicWebRadioDatabase *m_database;

};

#endif // MUSICWEBRADIOWIDGET_H
