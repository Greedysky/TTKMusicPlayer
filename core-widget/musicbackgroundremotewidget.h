#ifndef MUSICBACKGROUNDREMOTEWIDGET_H
#define MUSICBACKGROUNDREMOTEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

class QButtonGroup;
class QListWidgetItem;
class MusicBackgroundListWidget;

class MUSIC_WIDGET_EXPORT MusicBackgroundRemoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundRemoteWidget(QWidget *parent = 0);
    ~MusicBackgroundRemoteWidget();

signals:
    void showCustomSkin(const QString &path);

public slots:
    void buttonClicked(int index);
    void itemUserClicked(QListWidgetItem *item);

protected:
    void initWidget();
    void createButton();

    MusicBackgroundListWidget *m_listWidget;
    QButtonGroup *m_group;
    int m_currentIndex;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
