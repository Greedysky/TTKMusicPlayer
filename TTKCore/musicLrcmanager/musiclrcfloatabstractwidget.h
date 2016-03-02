#ifndef MUSICLRCFLOATABSTRACTWIDGET_H
#define MUSICLRCFLOATABSTRACTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPropertyAnimation>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the lrc float widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatAbstractWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatAbstractWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcFloatAbstractWidget();

    inline void setBlockAnimation(bool b) { m_blockAnimation = b;}
    /*!
     * Set block animation state or not.
     */
    virtual void resizeWidth(int width) = 0;
    /*!
     * Resize width bound by given width.
     * Subclass should implement this function.
     */

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    /*!
     * Override the widget event.
     */
    void animationIn();
    /*!
     * Show animation in case.
     */
    void animationOut();
    /*!
     * Show animation out case.
     */

    QPropertyAnimation *m_animation;
    QRect m_rectIn, m_rectOut;
    bool m_blockAnimation;

};

#endif // MUSICLRCFLOATABSTRACTWIDGET_H
