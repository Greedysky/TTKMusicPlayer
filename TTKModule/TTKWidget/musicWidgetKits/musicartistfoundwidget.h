#ifndef MUSICARTISTFOUNDTABLEWIDGET_H
#define MUSICARTISTFOUNDTABLEWIDGET_H

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

#include "musicfoundabstractwidget.h"

/*! @brief The class of the artist albums item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistAlbumsItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistAlbumsItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistAlbumsItemWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistAlbumsItemWidget();

    /*!
     * Set music results item.
     */
    void setMusicResultsItem(const MusicResultsItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    MusicResultsItem m_itemData;
    QPushButton *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel, *m_updateLabel;

};

class MusicPagingWidgetObject;

/*! @brief The class of the artist mvs found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistMvsFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistMvsFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistMvsFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistMvsFoundWidget();

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
     * Create the current albums item.
     */
    void createArtistMvsItem(const MusicResultsItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);
    /*!
     * Paging widget button has changed.
     */
    void buttonClicked(int index);

protected:
    QGridLayout *m_gridLayout;
    bool m_firstInit;
    MusicPagingWidgetObject *m_pagingWidgetObject;

};

class MusicDownLoadSimilarThread;

/*! @brief The class of the artist similar found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistSimilarFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistSimilarFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistSimilarFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistSimilarFoundWidget();

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
     * Create the current similar item.
     */
    void createArtistSimilarItem(const MusicResultsItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);

protected:
    QGridLayout *m_gridLayout;
    MusicDownLoadSimilarThread *m_similarThread;

};


/*! @brief The class of the artist albums found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistAlbumsFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistAlbumsFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistAlbumsFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistAlbumsFoundWidget();

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
     * Create the current albums item.
     */
    void createArtistAlbumsItem(const MusicResultsItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const QString &id);

protected:
    QGridLayout *m_gridLayout;

};


/*! @brief The class of the artist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistFoundTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundTableWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistFoundTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query) override;

};


/*! @brief The class of artist music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicArtistFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicArtistFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicArtistFoundWidget();

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
    void queryAllFinished();
    /*!
     * Query artist musics is finished.
     */
    void queryArtistFinished();
    /*!
     * Create the current artist info item.
     */
    void createArtistInfoItem(const MusicResultsItem &item);
    /*!
     * Set current container index.
     */
    void setCurrentIndex(int index);

protected:
    /*!
     * Create init interface lables.
     */
    void createLabels();
    /*!
     * Init the third widget.
     */
    void initThirdWidget();
    /*!
     * Init the fourth widget.
     */
    void initFourthWidget();
    /*!
     * Init the five widget.
     */
    void initFivethWidget();

    MusicArtistAlbumsFoundWidget *m_artistAlbums;
    MusicArtistMvsFoundWidget *m_artistMvs;
    MusicArtistSimilarFoundWidget *m_artistSim;

};

#endif // MUSICARTISTFOUNDTABLEWIDGET_H
