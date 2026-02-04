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
#include <QStackedWidget>
#include "musicquerytablewidget.h"

class QGridLayout;
class TTKClickedGroup;
class MusicPageQueryWidget;

enum RecommendModule
{
    Album,
    Artist,
    Playlist,
    PlaylistHQ
};

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
class TTK_MODULE_EXPORT MusicItemRecommendQueryWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicItemRecommendQueryWidget(RecommendModule module, QWidget *parent = nullptr);
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
    void createResultItem(const MusicResultDataItem &item);
    /*!
     * Current playlist item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

private:
    RecommendModule m_module;
    QGridLayout *m_gridLayout;
    QWidgetList m_resizeWidgets;
    MusicAbstractQueryRequest *m_networkRequest;

};


/*! @brief The class of the item more recommend query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemMoreRecommendQueryWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicItemMoreRecommendQueryWidget(RecommendModule module, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicItemMoreRecommendQueryWidget();

    /*!
     * Resize widget bound by resize called.
     */
    void resizeGeometry();

public Q_SLOTS:
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);
    /*!
     * Current category changed.
     */
    void categoryChanged(int index);
    /*!
     * Create the current item info.
     */
    void createResultItem(const MusicResultDataItem &item);
    /*!
     * Current playlist item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();
    /*!
     * Remove all items.
     */
    void removeItems(QLayout *layout);

    bool m_categoryChanged;
    RecommendModule m_module;
    QWidget *m_mainWidget;
    QGridLayout *m_gridLayout;
    QWidgetList m_resizeWidgets;
    TTKClickedGroup *m_areasGroup;
    MusicPageQueryWidget *m_pageQueryWidget;
    MusicAbstractQueryRequest *m_networkRequest;

};


/*! @brief The class of the main recommend widget.
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

private Q_SLOTS:
    /*!
     * Change area item to more.
     */
    void areaItemChangedToMore(int id);
    /*!
     * Change more item to area.
     */
    void moreItemChangedToArea();

private:
    /*!
     * Create home widget.
     */
    void createHomeWidget();
    /*!
     * Create top widget.
     */
    void createTopWidget();
    /*!
     * Create container widget.
     */
    void createContainerWidget();
    /*!
     * Create container top widget.
     */
    void createContainerTopWidget();
    /*!
     * Create container middle top widget.
     */
    void createContainerMiddleTopWidget();
    /*!
     * Create container middle widget.
     */
    void createContainerMiddleWidget();
    /*!
     * Create container middle bottom widget.
     */
    void createContainerMiddleBottomWidget();

    QStackedWidget *m_mainWidget;
    TTKClickedGroup *m_areasGroup;
    QWidget *m_homeWidget, *m_homeContainer;
    MusicNewSongRecommendQueryTableWidget *m_newSongsWidget;
    MusicItemRecommendQueryWidget *m_newAlbumsWidget;
    MusicItemRecommendQueryWidget *m_artistsWidget;
    MusicItemRecommendQueryWidget *m_hqPlaylistWidget;
    MusicItemRecommendQueryWidget *m_playlistWidget;
    MusicItemMoreRecommendQueryWidget *m_itemMoreWidget;

};

#endif // MUSICMAINRECOMMENDWIDGET_H
