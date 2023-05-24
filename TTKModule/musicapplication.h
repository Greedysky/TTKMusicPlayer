#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
 ***************************************************************************/

#include "musicobject.h"
#include "ttkabstractmoveresizewidget.h"

class MusicPlayer;
class MusicPlaylist;
class MusicBottomAreaWidget;
class MusicTopAreaWidget;
class MusicRightAreaWidget;
class MusicLeftAreaWidget;
class MusicApplicationModule;
class MusicSongsSummariziedWidget;

class MusicMPRISPlayerCore;

namespace Ui {
class MusicApplication;
}

/*! @brief The class of the app main widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicApplication : public TTKAbstractMoveResizeWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicApplication)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicApplication(QWidget *parent = nullptr);
    ~MusicApplication();

    /*!
     * Get class object instance.
     */
    static MusicApplication *instance();

    /*!
     * Get current file name.
     */
    QString currentFileName() const;
    /*!
     * Get current file path.
     */
    QString currentFilePath() const;

    /*!
     * Check current list index is -1 or not.
     */
    bool checkMusicListCurrentIndex() const;
    /*!
     * Load current song lrc.
     */
    void musicLoadCurrentSongLrc();

    /*!
     * Import outside music datas into container.
     */
    void musicImportSongsPathOutside(const QString &file, bool play);

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
    bool musicLovestContains(int index) const;
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
    TTK::PlayMode playMode() const;

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
    void playerStateChanged(TTK::PlayState state);
    /*!
     * Show current song some information.
     */
    void showCurrentSong();
    /*!
     * Set current player to play or not.
     */
    void playState();
    /*!
     * Set current player to stop.
     */
    void musicStateStop();
    /*!
     * Set current player to play previous.
     */
    void playPrevious();
    /*!
     * Set current player to play next.
     */
    void playNext();
    /*!
     * Set current play mdoe to order.
     */
    void playOrder();
    /*!
     * Set current play mdoe to random.
     */
    void playRandom();
    /*!
     * Set current play mdoe to list loop.
     */
    void playlistLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void playOneLoop();
    /*!
     * Set current play mdoe to just once.
     */
    void playOnce();

    /*!
     * Set current play volume to 0.
     */
    void musicVolumeMute();
    /*!
     * Current play volume changed.
     */
    void volumeChanged(int volume);
    /*!
     * Import music songs.
     */
    void musicImportSongs();
    /*!
     * Import music songs by files.
     */
    void musicImportSongsByFiles();
    /*!
     * Import music songs by dir.
     */
    void musicImportSongsByDir();
    /*!
     * Import music songs by url.
     */
    void musicImportSongsByUrl();
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
    void musicPlayIndex(int row, int column);
    /*!
     * Set current row index music to play.
     */
    void musicPlayIndexClicked(int row, int column);
    /*!
     * Set song speed and slow by given pos.
     */
    void musicPlayAnyTimeAt(int value);
    /*!
     * Set current player volume down.
     */
    void volumeDown();
    /*!
     * Set current player volume up.
     */
    void volumeUp();
    /*!
     * Show setting widget.
     */
    void musicSetting();
    /*!
     * Show current play index.
     */
    void musicCurrentPlayLocation();
    /*!
     * Add music song to lovest list.
     */
    void musicAddSongToLovestList();
    /*!
     * Add music song to lovest list.
     */
    void musicAddSongToLovestList(bool state);
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
     * Apply settings parameters.
     */
    void applyParameter();
    /*!
     * Remove love item from indexs.
     */
    void removeLoveItemAt(const QString &path, bool current);
    /*!
     * Delete items from indexs.
     */
    void removeItemAt(const QStringList &path, bool remove, bool current, int toolIndex);
    /*!
     * The current lrc should update.
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
     * Get current play list.
     */
    void currentPlaylist(QStringList &list);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void closeEvent(QCloseEvent *event) override final;
    virtual void dragEnterEvent(QDragEnterEvent *event) override final;
    virtual void dragMoveEvent(QDragMoveEvent *event) override final;
    virtual void dropEvent(QDropEvent *event) override final;
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override final;
    virtual bool eventFilter(QObject *object, QEvent *event) override final;

    /*!
     * Set music current play index.
     */
    void setMusicPlayIndex();
    /*!
     * Read system config from file.
     */
    void readSystemConfigFromFile();
    /*!
     * Write system config to file.
     */
    void writeSystemConfigToFile();

private:
    Ui::MusicApplication *m_ui;
    bool m_quitWindowClose;
    int m_currentSongTreeIndex;
    MusicPlayer* m_player;
    MusicPlaylist* m_playlist;
    MusicSongsSummariziedWidget *m_songTreeWidget;
    MusicBottomAreaWidget *m_bottomAreaWidget;
    MusicTopAreaWidget *m_topAreaWidget;
    MusicRightAreaWidget *m_rightAreaWidget;
    MusicLeftAreaWidget *m_leftAreaWidget;
    MusicApplicationModule *m_applicationObject;

    friend class MusicMPRISPlayerCore;
    static MusicApplication *m_instance;

};

#endif // MUSICAPPLICATION_H
