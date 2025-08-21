#ifndef MUSICPLAYLISTQUERYINFOWIDGET_H
#define MUSICPLAYLISTQUERYINFOWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
#include "musicabstractqueryrequest.h"

class QPushButton;
class QStackedWidget;
class MusicPlaylistQueryTableWidget;
class MusicPlaylistQueryCommentsWidget;

/*! @brief The class of the playlist music query info widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistQueryInfoWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistQueryInfoWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistQueryInfoWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

    /*!
     * Set media playlist item.
     */
    void setResultDataItem(const MusicResultDataItem &item, QObject *obj);
    /*!
     * Set network query input.
     */
    void setQueryInput(MusicAbstractQueryRequest *query);

public Q_SLOTS:
    /*!
     * Set current container index.
     */
    void setCurrentIndex(int index);
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();

private:
    MusicPlaylistQueryCommentsWidget *m_commentsWidget;

};

#endif // MUSICPLAYLISTQUERYINFOWIDGET_H
