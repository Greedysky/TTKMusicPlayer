#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
class MusicSongsContainerWidget;

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
public:
    /*!
     * Object constructor.
     */
    explicit MusicApplication(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
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
    void loadCurrentSongLrc();

    /*!
     * Import outside media datas into container.
     */
    void importSongsByOutside(const QString &path, bool play);

    /*!
     * Get media current song item download contains.
     */
    QString containsDownloadMedia(bool &contains) const;
    /*!
     * Get media current song lovest item contains.
     */
    bool containsLovestMedia() const;
    /*!
     * Get media list current song lovest item contains.
     */
    bool containsLovestMedia(int index) const;
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
    TTK::PlayMode playMode() const noexcept;

public Q_SLOTS:
    /*!
     * Application window close.
     */
    void quitWindow();
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
    void switchToPlayState();
    /*!
     * Set current player to stop.
     */
    void switchToStopState();
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
     * Set current player volume down.
     */
    void volumeDown();
    /*!
     * Set current player volume up.
     */
    void volumeUp();
    /*!
     * Set current play volume to 0.
     */
    void volumeMute();
    /*!
     * Current play volume changed.
     */
    void volumeChanged(int volume);

    /*!
     * Import media songs.
     */
    void importSongsPopup();
    /*!
     * Import media songs by url.
     */
    void importSongsByUrl();
    /*!
     * Import media songs by item list.
     */
    void importSongsItemList();
    /*!
     * Export media songs by item.
     */
    void exportSongsItem(int index);
    /*!
     * Export media songs by item list.
     */
    void exportSongsItemList();

    /*!
     * Set current media play list sort.
     */
    void playSortBy(int row);
    /*!
     * Set played list row index media to play.
     */
    void playedIndexBy(int row);
    /*!
     * Set current row index media to play.
     */
    void playIndexBy(int row);
    /*!
     * Set current row index media to play.
     */
    void playIndexBy(int row, int column);
    /*!
     * Set current row index media to play.
     */
    void playIndexClicked(int row, int column);
    /*!
     * Set song time position by given pos.
     */
    void playAnyTimePosition(int time);

    /*!
     * Show setting widget.
     */
    void showSettingWidget();
    /*!
     * Show current play index.
     */
    void currentPlayLocation();
    /*!
     * Add media song to lovest list.
     */
    void addSongToLovestList(bool state = true);
    /*!
     * Set window concise.
     */
    void windowConciseChanged();
    /*!
     * Set current song enhanced effect by type.
     */
    void enhancedSongChanged(int type);
    /*!
     * Create right menu.
     */
    void createRightMenu();
    /*!
     * Apply settings parameters.
     */
    void applyParameter();
    /*!
     * Remove love item from rows.
     */
    void removeLoveItemAt(const QString &path, bool current);
    /*!
     * Delete items from rows.
     */
    void removeItemAt(const QStringList &path, bool remove, bool current, int playlistRow);
    /*!
     * The current lrc should update.
     */
    void currentLrcUpdated();
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
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override final;
    virtual bool eventFilter(QObject *object, QEvent *event) override final;

    /*!
     * Generate playlist items.
     */
    void generatePlaylistItems();
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

    bool m_quitWindowMode;
    int m_currentPlaylistRow;
    MusicPlayer *m_player;
    MusicPlaylist *m_playlist;
    MusicSongsContainerWidget *m_songTreeWidget;
    MusicBottomAreaWidget *m_bottomAreaWidget;
    MusicTopAreaWidget *m_topAreaWidget;
    MusicRightAreaWidget *m_rightAreaWidget;
    MusicLeftAreaWidget *m_leftAreaWidget;
    MusicApplicationModule *m_applicationModule;

    friend class MusicMPRISPlayerCore;
    static MusicApplication *m_instance;

};

#endif // MUSICAPPLICATION_H
