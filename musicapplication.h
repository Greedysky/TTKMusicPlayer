#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QCloseEvent>
#include <QMenu>
#include <QTimer>
#include "musicabstractmovewidget.h"
#include "musicobject.h"

class MusicPlayer;
class MusicPlaylist;
class MusicSongsSummarizied;
class MusicBottomAreaWidget;
class MusicTopAreaWidget;
class MusicRightAreaWidget;
class MusicLeftAreaWidget;
class MusicApplicationObject;

namespace Ui {
class MusicApplication;
}

class MUSIC_GUI_EXPORT MusicApplication : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicApplication(QWidget *parent = 0);
    virtual ~MusicApplication();

    QString getCurrentFileName() const;
    bool checkMusicListCurrentIndex() const;
    void musicLoadCurrentSongLrc();

public slots:
    void quitWindowClose();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void stateChanged();
    void showCurrentSong(int);
    void musicKey();
    void musicPlayPrivious();
    void musicPlayNext();
    void musicPlayOrder();
    void musicPlayRandom();
    void musicPlayListLoop();
    void musicPlayOneLoop();
    void musicPlayItemOnce();
    void musicVolumeNULL();
    void musicVolumeChanged(int volume);
    void musicImportSongs();
    void musicImportSongsOnlyFile();
    void musicImportSongsOnlyDir();
    void musicPlayIndex(int row, int col);
    void musicPlayAnyTimeAt(int posValue);
    void musicActionVolumeSub();
    void musicActionVolumePlus();
    void musicSetting();
    void musicCurrentPlayLocation();
    void musicAddSongToLovestListAt();
    void musicWindowConciseChanged();
    void musicEnhancedMusicChanged(int type);
    /////////////////////////////////////////////
    //This is a slot by MusicLocalSongSearch's signal emit
    void musicSearchIndexChanged(int row, int col);
    /////////////////////////////////////////////
    //This is a slot by MusicSettingWidget's signal emit
    void getParameterSetting();
    /////////////////////////////////////////////
    //This is a slot by MusicSongsSummarizied's signal emit
    void setDeleteItemAt(const MIntList &index);
    /////////////////////////////////////////////
    //This is a slot by MusicInlineLrcContainer's signal emit
    void musicCurrentLrcUpdated();
    void updateCurrentTime(qint64 pos);
    //This is a slot by MusicTimerAutoObject's signal emit
    void setPlaySongChanged(int);
    void setStopSongChanged();
    /////////////////////////////////////////////
    //This is a slot by MusicLeftAreaWidget's signal emit
    void addSongToPlayList(const QStringList &item);
    void getCurrentPlayList(QStringList &list);

protected:
    void initWindowSurface();
    void createPlayModeMenu(QMenu &menu);
    void createPlayModeMenuIcon(QMenu &menu);
    virtual void closeEvent(QCloseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual bool nativeEvent(const QByteArray &, void *, long *);

    void readXMLConfigFromText();
    void writeXMLConfigToText();
    void musicImportSongsSettingPath(const QStringList &path);

private:
    Ui::MusicApplication *ui;
    bool m_playControl;
    int m_currentMusicSongTreeIndex;
    QMenu m_playModeMenu;
    MIntsListMap m_searchfileListCache;

    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicList;
    MusicSongsSummarizied *m_musicSongTree;
    MusicBottomAreaWidget *m_bottomAreaWidget;
    MusicTopAreaWidget *m_topAreaWidget;
    MusicRightAreaWidget *m_rightAreaWidget;
    MusicLeftAreaWidget *m_leftAreaWidget;
    MusicApplicationObject *m_object;

};

#endif // MAINWINDOW_H
