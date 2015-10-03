#ifndef MUSICSONGSLISTWIDGET_H
#define MUSICSONGSLISTWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QContextMenuEvent>
#include "musicsonginformation.h"
#include "musicabstracttablewidget.h"

class QAction;
class QTimer;
class MusicSongsListPlayWidget;
class MusicSongsListItemInfoWidget;

class MUSIC_WIDGET_EXPORT MusicSongsListWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListWidget(QWidget *parent = 0);
    virtual ~MusicSongsListWidget();

    void musicSongsFileName(const QStringList &filenamelists);
    void clearAllItems();
    void selectRow(int index);
    inline void setPlaybackMode(MusicObject::SongPlayType type)
        { m_songplaymode = type;}
    void setTimerLabel(const QString &t) const;
    void updateArtPicture() const;
    void replacePlayWidgetRow();

signals:
    void deleteItemAt(const MIntList &index, bool fileRemove);
    void musicSongToLovestListAt(int row);
    void currentTextChanged(int index, const QString &text);

    void musicPlay(int index);
    void musicPlayOrder();
    void musicPlayRandom();
    void musicPlayListLoop();
    void musicPlayOneLoop();
    void musicPlayItemOnce();
    void musicAddNewFiles();
    void musicAddNewDir();
    void musicOpenFileDir(int index);

    void getMusicSongInformation(int row, MusicSong &song);
    void getMusicSongFileInformation(int row, QString &name,
                                     QString &path, bool st = true);
    void getMusicIndexSwaped(int before, int after, int play,
                             QStringList &list);

public slots:
    void musicPlay();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void setDeleteItemWithFile();
    void listCellEntered(int row, int column);
    void listCellClicked(int row, int column);
    void showTimeOut();
    void stayTimeOut();
    void addMusicSongToLovestListAt();
    void addPlaySongToLovestListAt();
    void setChangSongName();
    void musicOpenFileDir();
    void musicMakeRingWidget();
    void musicTransformWidget();
    void musicFileInformation();
    void setItemRenameFinished(const QString &name);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QTimer *m_timerShow;
    QTimer *m_timerStay;

    int m_playRowIndex;
    int m_dragStartIndex;
    QPoint m_dragStartPoint;
    bool m_mouseMoved;

    MusicSongsListItemInfoWidget *m_musicSongsListItem;
    MusicSongsListPlayWidget *m_musicSongsPlayWidget;

    bool m_leftButtonPressed;
    bool m_renameActived;
    QTableWidgetItem *m_renameItem;
    bool m_deleteItemWithFile;
    MusicObject::SongPlayType m_songplaymode;

};

#endif // MUSICSONGSLISTWIDGET_H
