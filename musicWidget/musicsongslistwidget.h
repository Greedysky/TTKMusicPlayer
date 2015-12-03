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
#include <QTimer>
#include <QContextMenuEvent>
#include "musicsong.h"
#include "musicabstracttablewidget.h"

class MusicSongsListPlayWidget;
class MusicSongsListItemInfoWidget;

class MUSIC_WIDGET_EXPORT MusicSongsListWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListWidget(QWidget *parent = 0);
    virtual ~MusicSongsListWidget();

    void setSongsFileName(MusicSongs *songs);
    void updateSongsFileName(const MusicSongs &songs);
    void clearAllItems();

    void selectRow(int index);
    inline void setPlaybackMode(MusicObject::SongPlayType type) { m_songplaymode = type;}
    void setTimerLabel(const QString &t) const;
    void replacePlayWidgetRow();

signals:
    void musicPlayOrder();
    void musicPlayRandom();
    void musicPlayListLoop();
    void musicPlayOneLoop();
    void musicPlayItemOnce();
    void musicAddNewFiles();
    void musicAddNewDir();

    void isCurrentIndexs(bool &state);
    void deleteItemAt(const MIntList &index, bool fileRemove);
    void musicSongToLovestListAt(int row);
    void getMusicIndexSwaped(int before, int after, int play,
                             QStringList &list);

public slots:
    virtual void listCellEntered(int row, int column) override;
    virtual void listCellClicked(int row, int column) override;
    void musicPlayClicked();
    void setDeleteItemAt();
    void setDeleteItemAll();
    void setDeleteItemWithFile();
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
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    void startToDrag();

    int m_playRowIndex, m_dragStartIndex;
    QPoint m_dragStartPoint;
    bool m_mouseMoved;

    QTimer m_timerShow, m_timerStay;
    MusicSongs *m_musicSongs;
    MusicSongsListItemInfoWidget *m_musicSongsInfoWidget;
    MusicSongsListPlayWidget *m_musicSongsPlayWidget;

    bool m_leftButtonPressed;
    bool m_renameActived, m_deleteItemWithFile;
    QTableWidgetItem *m_renameItem;
    MusicObject::SongPlayType m_songplaymode;

};

#endif // MUSICSONGSLISTWIDGET_H
