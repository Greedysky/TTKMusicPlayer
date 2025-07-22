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

#ifndef MADSPINPLUGIN_H
#define MADSPINPLUGIN_H

#include <qmmp/visual.h>

class MadSpinWidget;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MadSpinPlugin : public Visual
{
    Q_OBJECT
public:
    explicit MadSpinPlugin(QWidget *parent = nullptr);

private slots:
    void numChanged(QAction *action);
    void speedChanged(QAction *action);

private:
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void processData(float *left, float *right) override final;

    MadSpinWidget *m_container = nullptr;

};

#endif
