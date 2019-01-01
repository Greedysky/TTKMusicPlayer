#ifndef MUSICWEBMVRADIOFOUNDCATEGORYPOPWIDGET_H
#define MUSICWEBMVRADIOFOUNDCATEGORYPOPWIDGET_H

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

#include "musictoolmenuwidget.h"
#include "musiccategoryconfigmanager.h"

/*! @brief The class of the music mv category item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMVRadioFoundCategoryItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioFoundCategoryItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMVRadioFoundCategoryItem(QWidget *parent = nullptr);

    /*!
     * Set current category.
     */
    void setCategory(const MusicResultsCategory &category);

Q_SIGNALS:
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);

public Q_SLOTS:
    /*!
     * Current category item clicked.
     */
    void buttonClicked(int index);

protected:
    MusicResultsCategory m_category;

};



/*! @brief The class of the music mv category pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMVRadioFoundCategoryPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioFoundCategoryPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMVRadioFoundCategoryPopWidget(QWidget *parent = nullptr);

    /*!
     * Set current category by input server.
     */
    void setCategory(const QString &server, QObject *obj);
    /*!
     * Close the menu dialog.
     */
    void closeMenu();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

};

#endif // MUSICWEBMVRADIOFOUNDCATEGORYPOPWIDGET_H
