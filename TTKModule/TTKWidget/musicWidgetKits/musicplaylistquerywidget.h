#ifndef MUSICPLAYLISTQUERYWIDGET_H
#define MUSICPLAYLISTQUERYWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musiccategoryconfigmanager.h"
#include "musicabstractitemquerywidget.h"

class QGridLayout;
class MusicPageQueryWidget;
class MusicPlaylistQueryInfoWidget;
class MusicPlaylistFoundCategoryPopWidget;

/*! @brief The class of the playlist music item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistQueryItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistQueryItemWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistQueryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistQueryItemWidget();

    /*!
     * Set music results item.
     */
    void setResultDataItem(const MusicResultDataItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &bytes);
    /*!
     * Image render finished.
     */
    void renderFinished(const QPixmap &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

private:
    MusicResultDataItem m_itemData;
    QPushButton *m_topListenButton, *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel, *m_creatorLabel;

};



/*! @brief The class of the playlist music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistQueryWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current id to search founds.
     */
    virtual void setCurrentID(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createPlaylistItem(const MusicResultDataItem &item);
    /*!
     * Current play list clicked.
     */
    void currentPlaylistClicked(const MusicResultDataItem &item);
    /*!
     * Set current index to playlist menu page.
     */
    void backToPlaylistMenu();
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);

private:
    bool m_initialized, m_categoryChanged;
    QGridLayout *m_gridLayout;
    MusicPageQueryWidget *m_pageQueryWidget;
    MusicPlaylistQueryInfoWidget *m_infoWidget;
    MusicPlaylistFoundCategoryPopWidget *m_categoryButton;

};

#endif // MUSICPLAYLISTQUERYWIDGET_H
