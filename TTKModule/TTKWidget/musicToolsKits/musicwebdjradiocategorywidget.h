#ifndef MUSICWEBDJRADIOCATEGORYWIDGET_H
#define MUSICWEBDJRADIOCATEGORYWIDGET_H

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

class QGridLayout;
class MusicDJRadioCategoryThread;

/*! @brief The class of music dj radio category item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioCategoryItemWidget : public MusicClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioCategoryItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioCategoryItemWidget(QWidget *parent = nullptr);

    ~MusicWebDJRadioCategoryItemWidget();

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
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    QLabel *m_iconLabel, *m_nameLabel;
    MusicResultsItem m_itemData;

};


/*! @brief The class of music dj radio category widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebDJRadioCategoryWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioCategoryWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebDJRadioCategoryWidget(QWidget *parent = nullptr);

    virtual ~MusicWebDJRadioCategoryWidget();

    /*!
     * Init widget.
     */
    void init();
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

Q_SIGNALS:
    /*!
     * Current category clicked.
     */
    void currentCategoryClicked(const MusicResultsItem &item);

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createCategoryItems();

protected:
    QGridLayout *m_gridLayout;
    QList<QLabel*> m_resizeWidgets;
    MusicDJRadioCategoryThread *m_categoryThread;

};

#endif // MUSICWEBDJRADIOCATEGORYWIDGET_H
