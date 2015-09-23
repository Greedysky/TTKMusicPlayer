#ifndef MUSICBACKGROUNDLISTWIDGET_H
#define MUSICBACKGROUNDLISTWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define MUSIC_BG_ROLE Qt::UserRole + 1

#include <QListWidget>
#include "musicuiobject.h"
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicBackgroundListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundListWidget(QWidget *parent = 0);
    ~MusicBackgroundListWidget();

    void setCurrentItemName(const QString &name);
    void createItem(const QString &name, const QIcon &icon);
    void clearAllItems();

signals:
public slots:
    void recreateItem(const QString &name);

protected:

};

#endif // MUSICBACKGROUNDLISTWIDGET_H
