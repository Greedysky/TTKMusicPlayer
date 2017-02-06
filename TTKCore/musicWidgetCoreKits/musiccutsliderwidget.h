#ifndef MUSICCUTSLIDERWIDGET_H
#define MUSICCUTSLIDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void moveChanged();
    /*!
     * Moving button pos updated.
     */
    void buttonRelease();
    /*!
     * Moving button released.
     */

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
    /*!
     * Object contsructor.
     */
    ~MusicCutSliderWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setPosition(qint64 position);
    /*!
     * Set current position.
     */
    void setDuration(qint64 duration);
    /*!
     * Set current duration.
     */
    void resizeWindow(int width, int height);
    /*!
     * Resize window bound by given width and height.
     */

Q_SIGNALS:
    void posChanged(qint64 start, qint64 end);
    /*!
     * Moving button pos changed.
     */
    void buttonReleaseChanged(qint64 pos);
    /*!
     * Moving button pos release changed.
     */

public Q_SLOTS:
    void buttonMoveUpdate();
    /*!
     * Moving button pos updated.
     */
    void buttonReleaseLeft();
    /*!
     * Left moving button released.
     */
    void buttonReleaseRight();
    /*!
     * Left moving button released.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    /*!
     * Override the widget event.
     */

    MusicMoveButton *m_leftControl, *m_rightControl;
    int m_width, m_height;
    qint64 m_duration, m_position;

};

#endif // MUSICCUTSLIDERWIDGET_H
