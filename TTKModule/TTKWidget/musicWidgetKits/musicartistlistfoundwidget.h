#ifndef MUSICARTISTLISTFOUNDTABLEWIDGET_H
#define MUSICARTISTLISTFOUNDTABLEWIDGET_H

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

#include "musicclickedlabel.h"
#include "musicfoundabstractwidget.h"
#include "musiccategoryconfigmanager.h"

class QGridLayout;
class MusicPagingWidgetObject;
class MusicArtistListFoundCategoryPopWidget;

/*! @brief The class of the artist list music item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistListFoundItemWidget : public MusicClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistListFoundItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistListFoundItemWidget(QWidget *parent = nullptr);

    /*!
     * Set music results item.
     */
    void setMusicResultsItem(const MusicResultsItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultsItem &item);

public Q_SLOTS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    MusicResultsItem m_itemData;

};



/*! @brief The class of the artist list music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistListFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistListFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistListFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistListFoundWidget();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createArtistListItem(const MusicResultsItem &item);
    /*!
     * Current artist list clicked.
     */
    void currentArtistListClicked(const MusicResultsItem &item);
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);
    /*!
     * Number type button has changed.
     */
    void numberButtonClicked(int index);

protected:
    bool m_firstInit, m_categoryChanged;
    QString m_categoryId;
    QGridLayout *m_gridLayout;
    MusicPagingWidgetObject *m_pagingWidgetObject;
    MusicArtistListFoundCategoryPopWidget *m_categoryButton;

};

#endif // MUSICARTISTLISTFOUNDTABLEWIDGET_H
