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

class QListWidget;
class QButtonGroup;

class MUSIC_WIDGET_EXPORT MusicBackgroundRemoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundRemoteWidget(QWidget *parent = 0);
    ~MusicBackgroundRemoteWidget();

signals:

public slots:
    void buttonClicked(int index);

protected:
    void initWidget();
    void createButton();
    void clearAllItems();
    void createItem(const QString &name, const QIcon &icon);

    QListWidget *m_listWidget;
    QButtonGroup *m_group;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
