#ifndef MUSICTOOLMENUWIDGET_H
#define MUSICTOOLMENUWIDGET_H

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

#include <QMenu>
#include <QToolButton>

#include "musicuiobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the tool menu.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicToolMenu : public QMenu
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToolMenu)
public:
    /*!
     * Object constructor.
     */
    explicit MusicToolMenu(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Window state changed.
     */
    void windowStateChanged(bool state);

private:
    /*!
     * Override the widget event.
     */
    virtual void showEvent(QShowEvent *event) override final;
    virtual void hideEvent(QHideEvent *event) override final;

};


/*! @brief The class of the tool menu base widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicToolMenuWidget : public QToolButton
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToolMenuWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicToolMenuWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicToolMenuWidget();

    /*!
     * Set background translucent.
     */
    void setTranslucentBackground();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    virtual void popupMenu();

protected:
    MusicToolMenu *m_menu;
    QWidget *m_containWidget;

};

#endif // MUSICTOOLMENUWIDGET_H
