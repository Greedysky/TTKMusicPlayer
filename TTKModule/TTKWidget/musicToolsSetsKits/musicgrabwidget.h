#ifndef MUSICGRABWIDGET_H
#define MUSICGRABWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QWidget>
#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the grab area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicGrabWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicGrabWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

private Q_SLOTS:
    /*!
     * Create right menu.
     */
    void musicCreateRightMenu();
    /*!
     * Create right menu.
     */
    void musicCreateRightMenu(const QString &path);

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Grab pixmap.
     */
    void grabPixmap();

    bool m_isDrawing;
    QPixmap m_originPixmap, m_grabPixmap;
    QPoint m_ptCursor, m_ptStart, m_ptEnd;

};

#endif // MUSICGRABWIDGET_H
