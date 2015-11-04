#ifndef MUSICENHANCEDWIDGET_H
#define MUSICENHANCEDWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QToolButton>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicEnhancedWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicEnhancedWidget(QWidget *parent = 0);
    ~MusicEnhancedWidget();

signals:
    void enhancedMusicChanged(int type);

public slots:
    void setEnhancedMusicConfig(int type);

protected:
    void initWidget();
    QMenu *m_menu;
    QToolButton *m_caseButton, *m_Button1, *m_Button2;
    QToolButton *m_Button3, *m_Button4;

};

#endif // MUSICENHANCEDWIDGET_H
