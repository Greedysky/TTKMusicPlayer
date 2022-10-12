#ifndef MUSICDOWNLOADBATCHWIDGET_H
#define MUSICDOWNLOADBATCHWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
class MusicDownloadBatchWidget;
}

/*! @brief The class of the download batch table item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadBatchTableItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBatchTableItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBatchTableItem(QWidget *parent = nullptr);
    ~MusicDownloadBatchTableItem();

    /*!
     * Create current item by song info.
     */
    void addItem(const MusicObject::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type);
    /*!
     * Start to download data from net.
     */
    void startRequest();
    /*!
     * Set current quality by index.
     */
    void setCurrentQuality(int index);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    void currentQualityChanged(int index);

private:
    /*!
     * Start to download music data.
     */
    void startRequestMusic();
    /*!
     * Start to download movie data.
     */
    void startRequestMovie();

    QObject *m_supperClass;
    QComboBox *m_qulity;
    MusicObject::MusicSongInformation m_songInfo;
    QLabel *m_songName, *m_singer, *m_information;
    MusicAbstractQueryRequest::QueryType m_queryType;

};


/*! @brief The class of the download batch table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadBatchTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBatchTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBatchTableWidget(QWidget *parent = nullptr);
    ~MusicDownloadBatchTableWidget();

    /*!
     * Set parent class.
     */
    void setParentClass(QWidget *parent);

    /*!
     * Create current item by song info.
     */
    void addItem(const MusicObject::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type);
    /*!
     * Start to download data from net.
     */
    void startRequest();

public Q_SLOTS:
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Data download is finished.
     */
    void dataDownloadFinished();
    /*!
     * Table widget item cell click.
     */
    void currentQualityChanged(int index);

private:
    QObject *m_supperClass;
    int m_downloadOffset;
    QList<MusicDownloadBatchTableItem*> m_items;

};


/*! @brief The class of the download batch widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadBatchWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBatchWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBatchWidget(QWidget *parent = nullptr);
    ~MusicDownloadBatchWidget();

    /*!
     * Set current name to search and download musics.
     */
    void setSongName(const MusicObject::MusicSongInformationList &songInfos, MusicAbstractQueryRequest::QueryType type);

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Start to download data from net.
     */
    void startRequest();

private:
    Ui::MusicDownloadBatchWidget *m_ui;

};

#endif // MUSICDOWNLOADBATCHWIDGET_H
