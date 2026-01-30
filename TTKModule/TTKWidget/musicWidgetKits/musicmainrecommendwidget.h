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
class MusicNewAlbumsRecommendRequest;

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
    virtual void downloadQueryResult(int row) override;

    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

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
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    /*!
     * Add search media to play list by index.
     */
    void addSearchMusicToPlaylist(int row, bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(TTK::MusicSongInformation *info, bool play);

};


/*! @brief The class of the new album recommend query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNewAlbumsRecommendQueryWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicNewAlbumsRecommendQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicNewAlbumsRecommendQueryWidget();

public Q_SLOTS:
    /*!
     * Create the current album info item.
     */
    void createAlbumItem(const MusicResultDataItem &item);

private:
    QGridLayout *m_gridLayout;
    QWidgetList m_resizeWidgets;
    MusicNewAlbumsRecommendRequest *m_networkRequest;

};


/*! @brief The class of the system tray menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMainRecommendWidget : public QWidget
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
    MusicNewAlbumsRecommendQueryWidget *m_newAlbumsWidget;

};

#endif // MUSICMAINRECOMMENDWIDGET_H
