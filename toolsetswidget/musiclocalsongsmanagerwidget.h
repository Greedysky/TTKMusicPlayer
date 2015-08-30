#ifndef MUSICLOCALSONGSMANAGERWIDGET_H
#define MUSICLOCALSONGSMANAGERWIDGET_H

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
#include "musicuiobject.h"
#include "musicmovedialogabstract.h"

class QMovie;
class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

class MUSIC_WIDGET_EXPORT MusicLocalSongsManagerWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongsManagerWidget(QWidget *parent = 0);
    ~MusicLocalSongsManagerWidget();

signals:
    void addSongToPlay(const QStringList &names);

public slots:
    void selectedAllItems(bool check);
    void auditionButtonClick();
    void addButtonClick();
    void itemCellOnClick(int row, int col);
    void itemDoubleClicked(int row, int col);
    void setSongNamePath(const QStringList &name, const QStringList &dir);
    void filterIndexChanged();
    void musicSearchIndexChanged(int row, int col);

    void setShowlistButton();
    void setShowPathButton();
    virtual int exec();

protected:
    Ui::MusicLocalSongsManagerWidget *ui;
    void clearAllItems();
    void addAllItems(const QStringList &filename, const QStringList &fileDir);
    void addLoaclSongList();
    void addDrivesList();
    void itemsSelected();

    QStringList m_filename;
    QStringList m_fileDir;
    MusicLocalSongsManagerThread *m_thread;
    MIntsListMap m_searchfileListCache;
    int m_currentIndex;
    QMovie *m_movie;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
