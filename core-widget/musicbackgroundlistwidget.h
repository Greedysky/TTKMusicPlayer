#ifndef MUSICBACKGROUNDLISTWIDGET_H
#define MUSICBACKGROUNDLISTWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define MUSIC_BG_ROLE       Qt::UserRole + 1
#define MUSIC_FILEHASH_ROLE Qt::UserRole + 2

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
    void clearAllItems();

    void createItem(const QString &name, const QString &path);
    void createItem(const QString &name, const QIcon &icon);

    bool contains(const QString &name);
    bool contains(QListWidgetItem *item);

    int indexOf(const QString &name);
    int indexOf(QListWidgetItem *item);

    QString fileHash(const QString &name);

public slots:
    void recreateItem(const QString &name);

};

#endif // MUSICBACKGROUNDLISTWIDGET_H
