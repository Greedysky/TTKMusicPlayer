#ifndef MUSICCUTSLIDERWIDGET_H
#define MUSICCUTSLIDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QPushButton>
#include "musicglobaldefine.h"

/*! @brief The class of the move button.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMoveButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MusicMoveButton(QWidget *parent = 0);

Q_SIGNALS:
    void moveChanged();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */

    QPoint m_pressAt;
    bool m_leftButtonPress;

};


/*! @brief The class of the cut slider widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCutSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicCutSliderWidget(QWidget *parent = 0);
    ~MusicCutSliderWidget();

    void resizeWindow(int width, int height);
    /*!
     * Resize window bound by given width and height.
     */

Q_SIGNALS:
public Q_SLOTS:

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    MusicMoveButton *m_leftControl, *m_rightControl;
    int m_width, m_height;

};

#endif // MUSICCUTSLIDERWIDGET_H
