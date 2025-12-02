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

#ifndef JAKDAWWIDGET_H
#define JAKDAWWIDGET_H

#include <qmmp/visual.h>

class QMenu;
class QActionGroup;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class JakdawWidget : public Visual
{
    Q_OBJECT
public:
    enum FeedbackType
    {
        FEEDBACK_ZOOMRIPPLE,
        FEEDBACK_BLURONLY,
        FEEDBACK_ZOOMROTATE,
        FEEDBACK_SCROLL,
        FEEDBACK_INTOSCREEN,
        FEEDBACK_NEWRIPPLE
    };

    enum PlotterColor
    {
        PLOTTER_COLOUR_SOLID,
        PLOTTER_COLOUR_RANDOM,
        PLOTTER_COLOUR_MUSICTRIG
    };

    enum PlotterOptions
    {
        PLOTTER_SCOPE_LINES,
        PLOTTER_SCOPE_DOTS,
        PLOTTER_SCOPE_SOLID,
        PLOTTER_SCOPE_NOTHING
    };

public:
    explicit JakdawWidget(QWidget *parent = nullptr);
    virtual ~JakdawWidget();

private slots:
    void readSettings();
    void writeSettings();

private:
    virtual void resizeEvent(QResizeEvent *e) override final;
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void processData(float *left, float *right) override final;

    void deinit();
    void createMenu();
    void feedbackReset();
    void feedbackRender();
    void plotterDraw(float *pcm, float *freq);
    void vertLine(int x, int a, int b, uint32_t col);

    FeedbackType m_zoomMode = FEEDBACK_ZOOMRIPPLE;
    PlotterColor m_plotterColor = PLOTTER_COLOUR_SOLID;
    PlotterOptions m_plotterType = PLOTTER_SCOPE_LINES;

    int m_tableIndex = 0;
    int m_width = 0, m_height = 0;

    uint32_t *m_table = nullptr;
    uint32_t *m_image = nullptr;
    uint32_t *m_texture = nullptr;

    QMenu *m_menu;
    QActionGroup *m_modeActions, *m_colorActions, *m_typeActions;

};

#endif
