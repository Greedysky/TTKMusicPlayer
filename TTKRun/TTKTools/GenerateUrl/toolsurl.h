#ifndef TOOLSURL_H
#define TOOLSURL_H

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

#include <QWidget>

namespace Ui {
class ToolsUrl;
}

class ToolsUrl : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsUrl(QWidget *parent = nullptr);
    ~ToolsUrl();

private Q_SLOTS:
    void encodeClicked();
    void decodeClicked();

private:
    Ui::ToolsUrl *m_ui;

};

#endif // TOOLSURL_H
