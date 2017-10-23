#ifndef MUSICFOUNDABSTRACTWIDGET_H
#define MUSICFOUNDABSTRACTWIDGET_H

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

#include <QLabel>
#include "musicglobaldefine.h"

/*! @brief The class of the found query widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFoundAbstractWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFoundAbstractWidget(QWidget *parent = 0);

    ~MusicFoundAbstractWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name);
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) = 0;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() = 0;

    /*!
     * Init widget interface.
     */
    void initWidget();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    QList<QLabel*> m_resizeWidget;

};

#endif // MUSICFOUNDABSTRACTWIDGET_H
