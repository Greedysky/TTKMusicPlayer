#ifndef MUSICFLOATABSTRACTWIDGET_H
#define MUSICFLOATABSTRACTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPropertyAnimation>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the float widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFloatAbstractWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFloatAbstractWidget(QWidget *parent = 0);

    virtual ~MusicFloatAbstractWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set block animation state or not.
     */
    inline void setBlockAnimation(bool b) { m_blockAnimation = b;}
    /*!
     * Resize window bound by given width and height.
     * Subclass should implement this function.
     */
    virtual void resizeWindow(int width, int height) = 0;

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    /*!
     * Show animation in case.
     */
    void animationIn();
    /*!
     * Show animation out case.
     */
    void animationOut();

    QPropertyAnimation *m_animation;
    QRect m_rectIn, m_rectOut;
    bool m_blockAnimation;

};

#endif // MUSICFLOATABSTRACTWIDGET_H
