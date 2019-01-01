#ifndef MUSICTOOLMENUWIDGET_H
#define MUSICTOOLMENUWIDGET_H

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

#include <QMenu>
#include <QToolButton>
#include "musicglobaldefine.h"

/*! @brief The class of the tool menu.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToolMenu : public QMenu
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToolMenu)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToolMenu(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Window state changed.
     */
    void windowStateChanged(bool state);

protected:
    /*!
     * Override the widget event.
     */
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

};


/*! @brief The class of the tool menu base widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToolMenuWidget : public QToolButton
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToolMenuWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToolMenuWidget(QWidget *parent = nullptr);

    ~MusicToolMenuWidget();

    /*!
     * Set background translucent.
     */
    void setTranslucentBackground();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();

protected:
    MusicToolMenu *m_menu;
    QWidget *m_containWidget;

};

#endif // MUSICTOOLMENUWIDGET_H
