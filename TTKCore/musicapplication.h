#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QTimer>
#include <QCloseEvent>
#include "musicabstractmoveresizewidget.h"

class MusicPlayer;
class MusicPlaylist;
class MusicSongsSummariziedWidget;
class MusicBottomAreaWidget;
class MusicTopAreaWidget;
class MusicRightAreaWidget;
class MusicLeftAreaWidget;
class MusicApplicationObject;

namespace Ui {
class MusicApplication;
}

/*! @brief The class of the app main widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicApplication : public MusicAbstractMoveResizeWidget
{
    Q_OBJECT
public:
    explicit MusicApplication(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicApplication();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    static MusicApplication *instance();
    /*!
     * Get class object instance.
     */

    QString getCurrentFileName() const;
    /*!
     * Get current file name.
     */
    QString getCurrentFilePath() const;
    /*!
     * Get current file path.
     */

    bool checkMusicListCurrentIndex() const;
    /*!
     * Check current list index is -1 or not.
     */
    void musicLoadCurrentSongLrc();
    /*!
     * Load current song lrc.
     */
    void musicImportSongsSettingPath(const QStringList &path);
    /*!
     * Import music datas into container.
     */

    QString musicDownloadContains(bool &contains) const;
    /*!
     * Get music current song download contains.
     */
    bool musicLovestContains() const;
    /*!
     * Get music current song lovest contains.
     */
    bool musicListLovestContains(int index) const;
    /*!
     * Get music list current song lovest contains.
     */

    void updateCurrentArtist();
    /*!
     * Update current artist when it download finished.
     */
    int getPlayState() const;
    /*!
     * Get current play state.
     */
    int getPlayMode() const;
    /*!
     * Get current play mode.
     */

public Q_SLOTS:
    void quitWindowClose();
    /*!
     * Application window close.
     */
    void positionChanged(qint64 position);
    /*!
     * Set current player position.
     */
    void durationChanged(qint64 duration);
    /*!
     * Set current player duration.
     */
    void stateChanged();
    /*!
     * Current state changed.
     */
    void showCurrentSong(int index);
    /*!
     * Show current song some information.
     */
    void musicStatePlay();
    /*!
     * Set current player to play or not.
     */
    void musicPlayPrevious();
    /*!
     * Set current player to play previous.
     */
    void musicPlayNext();
    /*!
     * Set current player to play next.
     */
    void musicPlayOrder();
    /*!
     * Set current play mdoe to order.
     */
    void musicPlayRandom();
    /*!
     * Set current play mdoe to random.
     */
    void musicPlayListLoop();
    /*!
     * Set current play mdoe to list loop.
     */
    void musicPlayOneLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void musicPlayItemOnce();
    /*!
     * Set current play mdoe to just once.
     */
    void musicVolumeMute();
    /*!
     * Set current play volume to 0.
     */
    void musicVolumeChanged(int volume);
    /*!
     * Current play volume changed.
     */
    void musicImportSongs();
    /*!
     * Import music songs.
     */
    void musicImportSongsOnlyFile();
    /*!
     * Import music songs by file.
     */
    void musicImportSongsOnlyDir();
    /*!
     * Import music songs by dir.
     */
    void musicImportSongsItemList();
    /*!
     * Import music songs by item list.
     */
    void musicExportSongsItemList(int index);
    /*!
     * Export music songs by item list.
     */
    void musicPlayIndex(int row);
    /*!
     * Set current row index music to play.
     */
    void musicPlayIndex(int row, int col);
    /*!
     * Set current row index music to play.
     */
    void musicPlayIndexClicked(int row, int col);
    /*!
     * Set current row index music to play.
     */
    void musicPlayAnyTimeAt(int posValue);
    /*!
     * Set song speed and slow by given pos.
     */
    void musicActionVolumeSub();
    /*!
     * Set current player volume down.
     */
    void musicActionVolumePlus();
    /*!
     * Set current player volume up.
     */
    void musicSetting();
    /*!
     * Show setting widget.
     */
    void musicCurrentPlayLocation();
    /*!
     * Show current play index.
     */
    void musicAddSongToLovestListAt();
    /*!
     * Add music song to lovest list by row.
     */
    void musicAddSongToLovestListAt(bool state);
    /*!
     * Add music song to lovest list by row.
     */
    void musicWindowConciseChanged();
    /*!
     * Set window concise.
     */
    void musicEnhancedMusicChanged(int type);
    /*!
     * Set current music enhanced effect by type.
     */
    void musicImportPlay();
    /*!
     * Import outside song to play list and play.
     */
    void musicCreateRightMenu();
    /*!
     * Create right menu.
     */
    /////////////////////////////////////////////
    ///This is a slot by MusicLocalSongSearch's signal emit
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Search file from list.
     */
    /////////////////////////////////////////////
    ///This is a slot by MusicSettingWidget's signal emit
    void getParameterSetting();
    /*!
     * Get settings parameters.
     */
    /////////////////////////////////////////////
    ///This is a slot by MusicSongsSummariziedWidget's signal emit
    void setDeleteItemAt(const MusicObject::MIntList &index, bool remove);
    /*!
     * Delete items from indexs.
     */
    /////////////////////////////////////////////
    ///This is a slot by MusicInlineLrcContainer's signal emit
    void musicCurrentLrcUpdated();
    /*!
     * The current lrc should update, emit it.
     */
    void updateCurrentTime(qint64 pos);
    /*!
     * Update current play time when user adjust the progress.
     */
    ///This is a slot by MusicTimerAutoObject's signal emit
    void setPlaySongChanged(int index);
    /*!
     * Set current song to play in play list.
     */
    void setStopSongChanged();
    /*!
     * Set current song to stop in play list.
     */
    /////////////////////////////////////////////
    ///This is a slot by MusicLeftAreaWidget's signal emit
    void getCurrentPlayList(QStringList &list);
    /*!
     * Get current play lists.
     */

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
#if defined(Q_OS_WIN)
#  ifdef MUSIC_GREATER_NEW
    virtual bool nativeEvent(const QByteArray &, void *, long *) override;
#  else
    virtual bool winEvent(MSG *message, long *result) override;
#  endif
#endif
    /*!
     * Override the widget event.
     */

    void setMusicPlayIndex();
    /*!
     * Set music current play index.
     */
    void readXMLConfigFromText();
    /*!
     * Read XML config from text.
     */
    void writeXMLConfigToText();
    /*!
     * Write XML config to text.
     */

private:
    Ui::MusicApplication *m_ui;
    bool m_playControl;
    int m_currentMusicSongTreeIndex;

    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicList;
    MusicSongsSummariziedWidget *m_musicSongTree;
    MusicBottomAreaWidget *m_bottomAreaWidget;
    MusicTopAreaWidget *m_topAreaWidget;
    MusicRightAreaWidget *m_rightAreaWidget;
    MusicLeftAreaWidget *m_leftAreaWidget;
    MusicApplicationObject *m_applicationObject;

    static MusicApplication *m_instance;
};

#endif // MUSICAPPLICATION_H
