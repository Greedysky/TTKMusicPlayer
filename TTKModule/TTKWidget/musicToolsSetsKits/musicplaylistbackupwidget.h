#ifndef MUSICPLAYLISTBACKUPWIDGET_H
#define MUSICPLAYLISTBACKUPWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "ttkabstractresizeinterface.h"
#include "musicabstractsongslisttablewidget.h"

/*! @brief The class of the playlist backup table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistBackupTableWidget : public MusicAbstractSongsListTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistBackupTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistBackupTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistBackupTableWidget();

    /*!
     * Update songs files in table.
     */
    virtual void updateSongsList(const MusicSongList &songs) override final;
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

Q_SIGNALS:
    /*!
     * Add current selected song to playlist.
     */
    void addSongToPlaylist(const QStringList &items);

private Q_SLOTS:
    /*!
     * Table widget item cell double clicked.
     */
    void itemDoubleClicked(int row, int column);

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};


class QListWidget;

/*! @brief The class of the playlist backup widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistBackupWidget : public QWidget, public TTKAbstractResizeInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSonglistManagerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistBackupWidget(QWidget *parent = nullptr);
    ~MusicPlaylistBackupWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

private Q_SLOTS:
    /*!
     * Restore button clicked.
     */
    void restoreButtonClicked();
    /*!
     * Remote background type selected by index.
     */
    void currentDateChanged(const QString &text);
    /*!
     * Remote background type selected by index.
     */
    void currentTimeChanged(const QString &text);
    /*!
     * Remote background type selected by index.
     */
    void currentItemChanged(int index);

private:
    /*!
     * Init parameters.
     */
    void initialize();

    QComboBox *m_dateBox, *m_timeBox;
    QLabel *m_titleLabel;
    QListWidget *m_listWidget;
    MusicSongItemList m_items;
    MusicPlaylistBackupTableWidget *m_tableWidget;

};

#endif // MUSICPLAYLISTBACKUPWIDGET_H
