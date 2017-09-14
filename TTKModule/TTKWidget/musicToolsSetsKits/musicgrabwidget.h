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
