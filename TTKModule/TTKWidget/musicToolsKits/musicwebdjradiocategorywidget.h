#ifndef MUSICWEBDJRADIOCATEGORYWIDGET_H
#define MUSICWEBDJRADIOCATEGORYWIDGET_H

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

#include "ttkclickedlabel.h"
#include "musicabstractitemquerywidget.h"

class QGridLayout;
class MusicDJRadioCategoryRequest;

/*! @brief The class of the music dj radio category item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioCategoryItemWidget : public TTKClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioCategoryItemWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioCategoryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioCategoryItemWidget();

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
     * Current item clicked.
     */
    void currentItemClicked();

private:
    QLabel *m_iconLabel, *m_nameLabel;
    MusicResultDataItem m_itemData;

};


/*! @brief The class of the music dj radio category widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioCategoryWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebDJRadioCategoryWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioCategoryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioCategoryWidget();

    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

Q_SIGNALS:
    /*!
     * Current category clicked.
     */
    void currentCategoryClicked(const MusicResultDataItem &item);

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createCategoryItems();

private:
    QGridLayout *m_gridLayout;
    QWidgetList m_resizeWidgets;
    MusicDJRadioCategoryRequest *m_networkRequest;

};

#endif // MUSICWEBDJRADIOCATEGORYWIDGET_H
