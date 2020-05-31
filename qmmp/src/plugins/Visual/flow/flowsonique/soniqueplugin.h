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

#include <QTimer>
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

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();

private:
    virtual void showEvent(QShowEvent *) override;
    virtual void hideEvent(QHideEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override;

    QTimer *m_timer;
    SoniqueWidget *m_soniqueWidget;
    bool m_running;

    float m_left[QMMP_VISUAL_NODE_SIZE];
    float m_right[QMMP_VISUAL_NODE_SIZE];

};

#endif
