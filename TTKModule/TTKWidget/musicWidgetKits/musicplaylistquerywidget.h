#ifndef MUSICPLAYLISTQUERYWIDGET_H
#define MUSICPLAYLISTQUERYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include "musiccategoryconfigmanager.h"
#include "musicabstractitemquerywidget.h"

class QGridLayout;
class MusicPagingWidgetObject;
class MusicPlaylistQueryInfoWidget;
class MusicPlaylistFoundCategoryPopWidget;

/*! @brief The class of the playlist music item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistQueryItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistQueryItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistQueryItemWidget(QWidget *parent = nullptr);

    virtual ~MusicPlaylistQueryItemWidget();

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
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Image render finished.
     */
    void renderFinished(const QImage &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    MusicResultsItem m_itemData;
    QPushButton *m_topListenButton, *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel, *m_creatorLabel;

};



/*! @brief The class of the playlist music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistQueryWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistQueryWidget(QWidget *parent = nullptr);

    virtual ~MusicPlaylistQueryWidget();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Resize window bound by widget resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createPlaylistItem(const MusicResultsItem &item);
    /*!
     * Current play list clicked.
     */
    void currentPlaylistClicked(const MusicResultsItem &item);
    /*!
     * Set current index to playlist menu page.
     */
    void backToPlaylistMenu();
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);

protected:
    bool m_initialized, m_categoryChanged;
    QGridLayout *m_gridLayout;
    MusicPagingWidgetObject *m_pagingWidgetObject;
    MusicPlaylistQueryInfoWidget *m_infoWidget;
    MusicPlaylistFoundCategoryPopWidget *m_categoryButton;

};

#endif // MUSICPLAYLISTQUERYWIDGET_H
