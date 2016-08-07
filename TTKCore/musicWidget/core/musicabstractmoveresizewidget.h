#ifndef MUSICABSTRACTMOVERESIZEWIDGET_H
#define MUSICABSTRACTMOVERESIZEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the moving resize widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAbstractMoveResizeWidget : public QWidget
{
    Q_OBJECT
public:
    enum Direction
    {
        Direction_No = 0,       ///*no direction*/
        Direction_Left = 1,     ///*left direction*/
        Direction_Top = 2,      ///*top direction*/
        Direction_Right = 4,    ///*right direction*/
        Direction_Bottom = 8,   ///*bottom direction*/
        Direction_LeftTop = Direction_Left + Direction_Top,         ///*left top direction*/
        Direction_LeftBottom = Direction_Left + Direction_Bottom,   ///*left bottom direction*/
        Direction_RightTop = Direction_Right + Direction_Top,       ///*right top direction*/
        Direction_RightBottom = Direction_Right + Direction_Bottom  ///*right bottom direction*/
    };
    explicit MusicAbstractMoveResizeWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicAbstractMoveResizeWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */

    void sizeCursor(int direction);
    /*!
     * Set current widget cursor by direction.
     */
    int sizeDirection(int x, int y);
    /*!
     * Size direction detected.
     */
    QObjectList foreachWidget(QObject *object);
    /*!
     * Get all inside widgets.
     */

    QPoint m_pressAt;
    bool m_mouseLeftPress;
    int m_direction;

};

#endif // MUSICABSTRACTMOVERESIZEWIDGET_H
