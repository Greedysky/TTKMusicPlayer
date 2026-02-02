#ifndef MUSICMAINRECOMMENDWIDGET_H
#define MUSICMAINRECOMMENDWIDGET_H

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

#include <QLabel>
#include "musicquerytablewidget.h"

class QGridLayout;

/*! @brief The class of the new song recommend query item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNewSongRecommendQueryTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicNewSongRecommendQueryTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicNewSongRecommendQueryTableWidget();

    /*!
     * Start to search query by value.
     */
    virtual void startToSearchByValue(const QString &value) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
    /*!
     * Table widget item cell double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Create search result items.
     */
    virtual void createResultItem(const MusicResultInfoItem &result) override;

private:
    /*!
     * Add search media to play list by index.
     */
    void addSearchMusicToPlaylist(int row, bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(TTK::MusicSongInformation *info, bool play);

};


/*! @brief The class of the item recommend query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemRecommendQueryWidget : public QLabel
{
    Q_OBJECT
public:
    enum Module
    {
        Album,
        Artist,
        Playlist
    };

    /*!
     * Object constructor.
     */
    explicit MusicItemRecommendQueryWidget(Module module, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicItemRecommendQueryWidget();

    /*!
     * Set network query input.
     */
    void setQueryInput(MusicAbstractQueryRequest *query);
    /*!
     * Resize widget bound by resize called.
     */
    void resizeGeometry();

public Q_SLOTS:
    /*!
     * Create the current item info.
     */
    void createItem(const MusicResultDataItem &item);
    /*!
     * Current playlist item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

private:
    Module m_module;
    QGridLayout *m_gridLayout;
    QWidgetList m_resizeWidgets;
    MusicAbstractQueryRequest *m_networkRequest;

};


/*! @brief The class of the system tray menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMainRecommendWidget : public QWidget, public TTKAbstractResizeInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicMainRecommendWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMainRecommendWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

private:
    /*!
     * Create current top widget.
     */
    void createTopWidget();
    /*!
     * Create current container widget.
     */
    void createContainerWidget();
    /*!
     * Create current container top widget.
     */
    void createContainerTopWidget();
    /*!
     * Create current container middle top widget.
     */
    void createContainerMiddleTopWidget();
    /*!
     * Create current container middle widget.
     */
    void createContainerMiddleWidget();
    /*!
     * Create current container middle bottom widget.
     */
    void createContainerMiddleBottomWidget();

    QWidget *m_mainWidget, *m_container;
    MusicNewSongRecommendQueryTableWidget *m_newSongsWidget;
    MusicItemRecommendQueryWidget *m_newAlbumsWidget;
    MusicItemRecommendQueryWidget *m_artistsWidget;
    MusicItemRecommendQueryWidget *m_hqPlaylistWidget;
    MusicItemRecommendQueryWidget *m_playlistWidget;

};

#endif // MUSICMAINRECOMMENDWIDGET_H
