#ifndef MUSICPLAYLISTCATEGORYWIDGET_H
#define MUSICPLAYLISTCATEGORYWIDGET_H

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

#include <QLabel>
#include "ttkabstractresizeinterface.h"

class QGridLayout;
struct CategoryProperty;

/*! @brief The class of the playlist music category item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistCategoryItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistCategoryItem(CategoryProperty *category, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistCategoryItem();

    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Current category item clicked.
     */
    void buttonClicked(int index);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

    QWidgetList m_items;
    QGridLayout *m_gridLayout;

};



/*! @brief The class of the playlist music category widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistCategoryWidget : public QFrame, public TTKAbstractResizeInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistCategoryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlaylistCategoryWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();

    QList<MusicPlaylistCategoryItem*> m_items;

};

#endif // MUSICPLAYLISTCATEGORYWIDGET_H
