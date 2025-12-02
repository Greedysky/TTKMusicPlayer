/***************************************************************************
 * This file is part of the TTK qmmp plugin project
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

#ifndef WAVEVOICE_H
#define WAVEVOICE_H

#include <qmmp/visual.h>
#include <qmmp/visualpalette.h>

class QMenu;
class QActionGroup;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class WaveVoice : public Visual
{
    Q_OBJECT
public:
    explicit WaveVoice(QWidget *parent = nullptr);
    virtual ~WaveVoice();

private slots:
    void readSettings();
    void writeSettings();

private:
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *e) override final;

    virtual void processData(float *left, float *right) override final;

    void createMenu();
    void createPalette(int row);
    void initialize();

    VisualPalette::Palette m_palette= VisualPalette::PALETTE_DEFAULT;
    QImage m_backgroundImage;
    int m_offset = 0;
    int *m_xscale = nullptr;
    const double m_analyzerSize = 2.2;
    int m_rangeValue = 30;

    QMenu *m_menu;
    QAction *m_channelsAction;
    QActionGroup *m_typeActions, *m_rangeActions;

};

#endif
