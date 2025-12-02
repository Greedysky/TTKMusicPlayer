#ifndef TOOLSKIN_H
#define TOOLSKIN_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include <QWidget>

namespace Ui {
class ToolSkin;
}

/*! @brief The class of the skin tool.
 * @author Greedysky <greedysky@163.com>
 */
class ToolSkin : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit ToolSkin(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~ToolSkin();

private Q_SLOTS:
    void imageClicked();
    void paletteClicked();
    void readClicked();
    void writeClicked();

private:
    Ui::ToolSkin *m_ui;
    QPixmap m_pixmap;

};

#endif // TOOLSKIN_H
