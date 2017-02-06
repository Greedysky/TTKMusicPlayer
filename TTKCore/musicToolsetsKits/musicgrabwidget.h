#ifndef MUSICGRABWIDGET_H
#define MUSICGRABWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the grab area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicGrabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicGrabWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

private Q_SLOTS:
    void musicCreateRightMenu();
    /*!
     * Create right menu.
     */
    void musicCreateRightMenu(const QString &path);
    /*!
     * Create right menu.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_isDrawing;
    QPoint m_ptCursor, m_ptStart, m_ptEnd;

};

#endif // MUSICGRABWIDGET_H
