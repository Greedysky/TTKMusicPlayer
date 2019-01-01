#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

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

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class QListWidgetItem;
namespace Ui {
class MusicToolSetsWidget;
}

/*! @brief The class of the tool sets widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicToolSetsWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToolSetsWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToolSetsWidget(QWidget *parent = nullptr);

    virtual ~MusicToolSetsWidget();

public Q_SLOTS:
    /*!
     * Tool sets list item has clicked.
     */
    void itemHasClicked(QListWidgetItem *item);
    /*!
     * Add tool sets list into list widget.
     */
    void addListWidgetItem();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    Ui::MusicToolSetsWidget *m_ui;

};

#endif // MUSICTOOLSETSWIDGET_H
