#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicabstracttablewidget.h"

class QPushButton;

class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = 0);
    virtual ~MusicLocalSongSearchPopTableWidget();

    void clearAllItems();
    void createItems(int index, const QString &name,
                     const QString &time);
signals:
    void setText(const QString &text);

public slots:
    virtual void listCellClicked(int row, int column);

};


class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = 0);
    ~MusicLocalSongSearchPopWidget();

    void createItems();

signals:
    void setText(const QString &text);

public slots:
    void clearButtonClicked();

protected:
    virtual void leaveEvent(QEvent *event);
    QString utcTimeToLocal(const QString &time) const;

    MusicLocalSongSearchPopTableWidget *m_popTableWidget;
    QPushButton *m_clearButton;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
