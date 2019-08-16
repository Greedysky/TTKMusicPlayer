#ifndef MUSICSOUNDKMICROSEARCHWIDGET_H
#define MUSICSOUNDKMICROSEARCHWIDGET_H

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

#include "musicquerytablewidget.h"
#include "musicabstractmovewidget.h"

/*! @brief The class of the sound kmicro search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundKMicroSearchTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSearchTableWidget(QWidget *parent = nullptr);

    virtual ~MusicSoundKMicroSearchTableWidget();

    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Data download to local file.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Set query Movie flag.
     */
    void setQueryMovieFlag(bool flag);

Q_SIGNALS:
    /*!
     * Set current media url.
     */
    void mediaUrlChanged(bool mv, const QString &url, const QString &lrcUrl);

public Q_SLOTS:
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItem(const MusicSearchedItem &songItem) override;
    /*!
     * Item has double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

protected:
    /*!
     * Data download to play.
     */
    void dataDownloadPlay(int row);
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    bool m_queryMovieMode;

};


class MusicLocalSongSearchEdit;

/*! @brief The class of the sound kmicro search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSearchWidget : public MusicAbstractMoveSingleWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundKMicroSearchWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSearchWidget(QWidget *parent = nullptr);

    ~MusicSoundKMicroSearchWidget();

    /*!
     * Connect to parent.
     */
    void connectTo(QObject *obj);
    /*!
     * Start seach KMicro.
     */
    void startSeachKMicro(const QString &name);

public Q_SLOTS:
    /*!
     * Start to search.
     */
    void startToSearch();
    /*!
     * Set query MV flag.
     */
    void setQueryMovieFlag(int flag);
    /*!
     * Set current search song name.
     */
    void setCurrentSongName(const QString &name);

protected:
    bool m_queryMovieMode;
    MusicLocalSongSearchEdit *m_searchEdit;
    MusicSoundKMicroSearchTableWidget *m_searchTableWidget;

};

#endif // MUSICSOUNDKMICROSEARCHWIDGET_H
