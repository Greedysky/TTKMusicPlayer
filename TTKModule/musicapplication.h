#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
    TTK_DECLARE_MODULE(MusicApplication)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicApplication(QWidget *parent = nullptr);

    virtual ~MusicApplication();

    /*!
     * Get class object instance.
     */
    static MusicApplication *instance();

    /*!
     * Get current file name.
     */
    QString getCurrentFileName() const;
    /*!
     * Get current file path.
     */
    QString getCurrentFilePath() const;

    /*!
     * Check current list index is -1 or not.
     */
    bool checkMusicListCurrentIndex() const;
    /*!
     * Load current song lrc.
     */
    void musicLoadCurrentSongLrc();

    //
    /*!
     * Execute outer radio by type.
     */
    void radioExecuteOuter(const QString &path);
    /*!
     * Import outer music datas into container.
     */
    void musicImportSongsSettingPathOuter(const QStringList &path, bool play);
    //

    /*!
     * Import music datas into container.
     */
    void musicImportSongsSettingPath(const QStringList &path);
    /*!
     * Get music current song download contains.
     */
    QString musicDownloadContains(bool &contains) const;
    /*!
     * Get music current song lovest contains.
     */
    bool musicLovestContains() const;
    /*!
     * Get music list current song lovest contains.
     */
    bool musicListLovestContains(int index) const;
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();

    /*!
     * Get current play state.
     */
    bool isPlaying() const;
    /*!
     * Get current player duration.
     */
    qint64 duration() const;
    /*!
     * Get current play mode.
     */
    MusicObject::PlayMode getPlayMode() const;

public Q_SLOTS:
    /*!
     * Application window close.
     */
    void quitWindowClose();
    /*!
     * Set current player position.
     */
    void positionChanged(qint64 position);
    /*!
     * Set current player duration.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current state changed.
     */
    void stateChanged();
    /*!
     * Show current song some information.
     */
    void showCurrentSong(int index);
    /*!
     * Set current player to play or not.
     */
    void musicStatePlay();
    /*!
     * Set current player to play previous.
     */
    void musicPlayPrevious();
    /*!
     * Set current player to play next.
     */
    void musicPlayNext();
    /*!
     * Set current play mdoe to order.
     */
    void musicPlayOrder();
    /*!
     * Set current play mdoe to random.
     */
    void musicPlayRandom();
    /*!
     * Set current play mdoe to list loop.
     */
    void musicPlaylistLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void musicPlayOneLoop();
    /*!
     * Set current play mdoe to just once.
     */
    void musicPlayItemOnce();
    /*!
     * Set current play volume to 0.
     */
    void musicVolumeMute();
    /*!
     * Current play volume changed.
     */
    void musicVolumeChanged(int volume);
    /*!
     * Import music songs.
     */
    void musicImportSongs();
    /*!
     * Import music songs by file.
     */
    void musicImportSongsOnlyFile();
    /*!
     * Import music songs by dir.
     */
    void musicImportSongsOnlyDir();
    /*!
     * Import music songs by item list.
     */
    void musicImportSongsItemList();
    /*!
     * Export music songs by item list.
     */
    void musicExportSongsItemList(int index);
    /*!
     * Set current music play list sort.
     */
    void musicPlaySort(int row);
    /*!
     * Set played list row index music to play.
     */
    void musicPlayedIndex(int row);
    /*!
     * Set current row index music to play.
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
     * Set song speed and slow by given pos.
     */
    void musicPlayAnyTimeAt(int posValue);
    /*!
     * Set current player volume down.
     */
    void musicActionVolumeSub();
    /*!
     * Set current player volume up.
     */
    void musicActionVolumePlus();
    /*!
     * Show setting widget.
     */
    void musicSetting();
    /*!
     * Show current play index.
     */
    void musicCurrentPlayLocation();
    /*!
     * Add music song to lovest list by row.
     */
    void musicAddSongToLovestListAt();
    /*!
     * Add music song to lovest list by row.
     */
    void musicAddSongToLovestListAt(bool state);
    /*!
     * Set window concise.
     */
    void musicWindowConciseChanged();
    /*!
     * Set current music enhanced effect by type.
     */
    void musicEnhancedMusicChanged(int type);
    /*!
     * Create right menu.
     */
    void musicCreateRightMenu();
    /*!
     * Search file from list.
     */
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Get settings parameters.
     */
    void getParameterSetting();
    /*!
     * Delete love item from indexs.
     */
    void setLoveDeleteItemAt(const QString &path, bool current);
    /*!
     * Delete items from indexs.
     */
    void setDeleteItemAt(const QStringList &path, bool remove, bool current, int toolIndex);
    /*!
     * The current lrc should update, emit it.
     */
    void musicCurrentLrcUpdated();
    /*!
     * Reset current song lrc index.
     */
    void resetCurrentSongLrcIndex();
    /*!
     * Update current play time when user adjust the progress.
     */
    void updateCurrentTime(qint64 pos);
    /*!
     * Set current song to play in play list.
     */
    void setPlaySongChanged(int index);
    /*!
     * Set current song to stop in play list.
     */
    void setStopSongChanged();
    /*!
     * Get current play lists.
     */
    void getCurrentPlaylist(QStringList &list);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    /*!
     * Set music current play index.
     */
    void setMusicPlayIndex();
    /*!
     * Read XML config from text.
     */
    void readXMLConfigFromText();
    /*!
     * Write XML config to text.
     */
    void writeXMLConfigToText();

private:
    Ui::MusicApplication *m_ui;
    bool m_playControl, m_quitWindowClose;
    int m_currentMusicSongTreeIndex;

    MusicPlayer* m_musicPlayer;
    MusicPlaylist* m_musicPlaylist;
    MusicSongsSummariziedWidget *m_musicSongTreeWidget;
    MusicBottomAreaWidget *m_bottomAreaWidget;
    MusicTopAreaWidget *m_topAreaWidget;
    MusicRightAreaWidget *m_rightAreaWidget;
    MusicLeftAreaWidget *m_leftAreaWidget;
    MusicApplicationObject *m_applicationObject;

    static MusicApplication *m_instance;
};

#endif // MUSICAPPLICATION_H
