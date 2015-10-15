#ifndef MUSICCONNECTMOBILEWIDGET_H
#define MUSICCONNECTMOBILEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

class QStackedWidget;

class MUSIC_TOOL_EXPORT MusicConnectMobileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicConnectMobileWidget(QWidget *parent = 0);
    ~MusicConnectMobileWidget();

public slots:
    void changeStatckedWidgetFirst();
    void changeStatckedWidgetSecond();
    void changeStatckedWidgetThird();

protected:
    void initFirstWidget();
    void initSecondWidget();
    void initThirdWidget();

    QStackedWidget *m_stackedWidget;

};

#endif // MUSICCONNECTMOBILEWIDGET_H
