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

#ifndef INFINITYWIDGET_H
#define INFINITYWIDGET_H

#include <qmmp/visual.h>

struct VCoord
{
    int x;
    int y;
};

struct VComplex
{
    float x;
    float y;
};

struct VColor
{
    short r;
    short g;
    short b;
};

struct VInterpol
{
    unsigned int coord;   /* coordinates of the top left pixel.*/
    unsigned int weight;  /* 32 bits = 4*8= weights of the four corners */
};

struct VEffect
{
    int num_effect;
    int curve;
    int curve_color;
    int curve_amplitude;
    int spectral_amplitude;
    int spectral_color;
    int mode_spectre;
    int spectral_shift;
};

#define NB_FCT 7
#define NB_PALETTES 5
#define NB_EFFECT_COUNT 29

class QMenu;
class QActionGroup;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class InfinityWidget : public Visual
{
    Q_OBJECT
public:
    explicit InfinityWidget(QWidget *parent = nullptr);
    virtual ~InfinityWidget();

private slots:
    void readSettings();
    void writeSettings();

private:
    virtual void resizeEvent(QResizeEvent *e) override final;
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void processData(float *left, float *right) override final;

    void deinit();
    void initialize();
    void createMenu();
    void createModule();
    void loadRandomEffect();
    void displayBlur();
    void displaySpectral(short *left, short *right);
    void displayCurve();
    void displayLine(int x1, int y1, int x2, int y2, int c);
    VComplex fct(VComplex c, int n, int p1, int p2);

    VEffect m_currentEffect;

    unsigned char *m_renders[2] = {0};
    VInterpol *m_field = nullptr;

    int m_oldColor = 0, m_newColor = 0;
    int m_colorCounter = 0, m_effectCounter = 0;
    int m_effectTime = 100;
    int m_colorTime = 100;

    short m_currentColors[256];
    VColor m_colorTable[NB_PALETTES][256];

    QMenu *m_menu;
    QActionGroup *m_effectActions, *m_colorActions;

};

#endif
