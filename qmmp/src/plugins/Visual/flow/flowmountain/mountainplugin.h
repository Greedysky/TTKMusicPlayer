/***************************************************************************
 * This file is part of the TTK qmmp plugin project
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

#ifndef MOUNTAINPLUGIN_H
#define MOUNTAINPLUGIN_H

#include <qmmp/visual.h>

class QMenu;
class QActionGroup;
class MountainWidget;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MountainPlugin : public Visual
{
    Q_OBJECT
public:
    enum Type
    {
        Simple,
        Complex
    };

public:
    explicit MountainPlugin(QWidget *parent = nullptr);

private slots:
    void readSettings();
    void writeSettings();

private:
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void processData(float *left, float *right) override final;

    void createMenu();
    void createMoudle();

    int m_type;
    MountainWidget *m_container = nullptr;

    QMenu *m_menu;
    QActionGroup *m_typeActions;

};

#endif
