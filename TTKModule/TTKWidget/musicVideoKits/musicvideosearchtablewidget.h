#ifndef MUSICVIDEOSEARCHTABLEWIDGET_H
#define MUSICVIDEOSEARCHTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "musicitemsearchtablewidget.h"
#include "musicabstractqueryrequest.h"

Q_DECLARE_METATYPE(TTK::MusicSongInformation)

/*! @brief The class of the video item play data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicVideoItem
{
    QString m_name;
    QString m_url;
    QString m_id;
    QString m_cover;
    QString m_server;
};

/*! @brief The class of the video search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoSearchTableWidget : public MusicItemSearchTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoSearchTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoSearchTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoSearchTableWidget();

    /*!
     * Start to search query by text.
     */
    virtual void startToSearchByText(const QString &text) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override final;

    /*!
     * Start to search query by given id.
     */
    void startToSearchByID(const QString &text);
    /*!
     * Start to search query by given data.
     */
    void startToSearchByID(const QVariant &data);
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection(int delta);

Q_SIGNALS:
    /*!
     * Set current media name and url to play.
     */
    void mediaUrlPathChanged(const MusicVideoItem &item);

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Table widget item cell double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Create search result items.
     */
    virtual void createResultItem(const MusicResultInfoItem &songItem) override final;
    /*!
     * Get music media information data.
     */
    void queryMediaProps(TTK::MusicSongPropertyList &props);
    /*!
     * Download current mv by controller.
     */
    void downloadLocalFromControl();

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    /*!
     * Download movie to local file by index.
     */
    void downloadLocalMovie(int row);

    bool m_singleRadioMode;
    QString m_currentSongName;

};

#endif // MUSICVIDEOSEARCHTABLEWIDGET_H
