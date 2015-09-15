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
#include "musicabstractmovedialog.h"
#include <QFileInfo>

class QMovie;
class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

class MUSIC_TOOLSET_EXPORT MusicLocalSongsManagerWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongsManagerWidget(QWidget *parent = 0);
    virtual ~MusicLocalSongsManagerWidget();

signals:
    void addSongToPlay(const QStringList &names);

public slots:
    void selectedAllItems(bool check);
    void auditionButtonClick();
    void addButtonClick();
    void itemCellOnClick(int row, int col);
    void itemDoubleClicked(int row, int col);
    void setSongNamePath(const QFileInfoList &name);
    void filterIndexChanged();
    void musicSearchIndexChanged(int row, int col);

    void setShowlistButton();
    void setShowPathButton();
    virtual int exec();

protected:
    Ui::MusicLocalSongsManagerWidget *ui;
    void clearAllItems();
    void addAllItems(const QFileInfoList &name);
    void addDrivesList();
    void itemsSelected();

    QFileInfoList m_filenames;
    MusicLocalSongsManagerThread *m_thread;
    MIntsListMap m_searchfileListCache;
    int m_currentIndex;
    QMovie *m_movie;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
