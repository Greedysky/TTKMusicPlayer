#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

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

#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"
#include "musicabstractqueryrequest.h"

namespace Ui {
class MusicDownloadWidget;
}

class QLabel;

/*! @brief The class of the download table item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadTableItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadTableItem(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadTableItem();

    /*!
     * Set current item icon.
     */
    void setIcon(const QString &name);
    /*!
     * Set current item other infomation(size and type and time).
     */
    void setInformation(const QString &info);
    /*!
     * Set current item text name.
     */
    void setText(const QString &text);

private:
    QLabel *m_text, *m_information, *m_icon;

};


/*! @brief The class of the download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadTableWidget();

    /*!
     * Create cell item by input data.
     */
    void addCellItem(const TTK::MusicSongProperty &prop, const QString &type, const QString &icon);
    /*!
     * Get current bitrate from item.
     */
    int bitrate() const;

public Q_SLOTS:
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;

private:
    QObjectList m_items;

};


/*! @brief The class of the download widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicDownloadWidget();

    /*!
     * Set current name to search and download data.
     */
    void initialize(const QString &name);
    /*!
     * Set current name to search and download data.
     */
    void initialize(const TTK::MusicSongInformation &info);
    /*!
     * Set current name to search and download data.
     */
    void initialize(MusicAbstractQueryRequest *request, int row);

public:
    /*!
     * Strat to download music.
     */
    static bool startToRequestMusic(const TTK::MusicSongInformation &info, int bitrate, QObject *parent);
    /*!
     * Strat to download movie.
     */
    static bool startToRequestMovie(const TTK::MusicSongInformation &info, int bitrate, QObject *parent);

Q_SIGNALS:
    /*!
     * Data download changed.
     */
    void dataDownloadChanged();

public Q_SLOTS:
    /*!
     * Override close function.
     */
    void close();
    /*!
     * Query selected quality media finished.
     */
    void downLoadNormalFinished();
    /*!
     * Query request data from net finished.
     */
    void downLoadRequestFinished();
    /*!
     * Start to download data.
     */
    void startToRequest();
    /*!
     * Download data from net finished.
     */
    void downloadFinished();

private:
    /*!
     * Enable or disable control state.
     */
    void controlEnabled(bool enabled);
    /*!
     * Set widget fixed height.
     */
    void setFixedHeightWidget(QWidget *w, int height);
    /*!
     * Set widget moved into given pos.
     */
    void setMoveWidget(QWidget *w, int pos);
    /*!
     * Create cell items by input data.
     */
    void addCellItems(const TTK::MusicSongPropertyList &props);

    bool m_exitRequired;
    Ui::MusicDownloadWidget *m_ui;
    MusicAbstractQueryRequest *m_networkRequest;
    TTK::MusicSongInformation m_info;
    MusicAbstractQueryRequest::QueryType m_queryType;

};

#endif // MUSICDOWNLOADWIDGET_H
