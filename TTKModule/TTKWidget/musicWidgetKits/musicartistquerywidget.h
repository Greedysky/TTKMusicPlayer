#ifndef MUSICARTISTQUERYWIDGET_H
#define MUSICARTISTQUERYWIDGET_H

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

#include "musicabstractitemquerywidget.h"

class MusicPageQueryWidget;

/*! @brief The class of the artist mvs query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicArtistMvsQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicArtistMvsQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicArtistMvsQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current key to search founds.
     */
    virtual void setCurrentKey(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Create the current albums item.
     */
    void createArtistMvsItem(const MusicResultDataItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);

private:
    bool m_initialized;
    QGridLayout *m_gridLayout;
    MusicPageQueryWidget *m_pageQueryWidget;

};


/*! @brief The class of the artist albums query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicArtistAlbumsQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicArtistAlbumsQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicArtistAlbumsQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current key to search founds.
     */
    virtual void setCurrentKey(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Create the current albums item.
     */
    void createArtistAlbumsItem(const MusicResultDataItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);

private:
    bool m_initialized;
    QGridLayout *m_gridLayout;
    MusicPageQueryWidget *m_pageQueryWidget;

};


/*! @brief The class of the artist music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicArtistQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicArtistQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicArtistQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current key to search founds.
     */
    virtual void setCurrentKey(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Set current container index.
     */
    void setCurrentIndex(int index);
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
    void createArtistItem(const MusicResultDataItem &item);

private:
    /*!
     * Create init interface lables.
     */
    void createLabels();

    MusicArtistAlbumsQueryWidget *m_artistAlbums;
    MusicArtistMvsQueryWidget *m_artistMvs;

};

#endif // MUSICARTISTQUERYWIDGET_H
