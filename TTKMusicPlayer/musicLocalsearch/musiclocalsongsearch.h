#ifndef MUSICLOCALSONGSEARCH_H
#define MUSICLOCALSONGSEARCH_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musicglobaldefine.h"

class QLabel;
class QToolButton;
class MusicLocalSongSearchEdit;

class MUSIC_SEARCH_EXPORT MusicLocalSongSearch : public QDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearch(QWidget *parent = 0);
    ~MusicLocalSongSearch();

    QString getSearchedText() const;
    void clearSearchedText() const;

public Q_SLOTS:
    bool close();

private:
    QLabel *m_showIcon;
    MusicLocalSongSearchEdit *m_searchLine;
    QToolButton *m_closeButton;

};

#endif // MUSICLOCALSONGSEARCH_H
