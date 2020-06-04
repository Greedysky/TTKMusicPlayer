/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#ifndef SONIQUEPLUGIN_H
#define SONIQUEPLUGIN_H

#include <qmmp/florid.h>

class SoniqueWidget;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SoniquePlugin : public Florid
{
    Q_OBJECT
public:
    explicit SoniquePlugin(QWidget *parent = nullptr);
    virtual ~SoniquePlugin();

private:
    virtual void contextMenuEvent(QContextMenuEvent *) override;

    virtual void process(float *left, float *right) override;

    SoniqueWidget *m_soniqueWidget;

};

#endif
