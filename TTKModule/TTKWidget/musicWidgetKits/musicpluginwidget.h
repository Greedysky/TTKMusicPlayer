#ifndef MUSICPLUGINWIDGET_H
#define MUSICPLUGINWIDGET_H

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

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicPluginWidget;
}
class QTreeWidgetItem;

/*! @brief The class of the plugin manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPluginWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPluginWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPluginWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Plugin item clicked changed.
     */
    void pluginItemChanged(QTreeWidgetItem *item, int column);
    /*!
     * Plugin item setting button click.
     */
    void pluginButtonClicked();

private:
    /*!
     * Load plugins info.
     */
    void loadPluginsInfo();

    Ui::MusicPluginWidget *m_ui;

};

#endif // MUSICPLUGINWIDGET_H
