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
    /*!
     * Object contsructor.
     */
    explicit MusicMoveButton(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Moving button pos updated.
     */
    void moveChanged();
    /*!
     * Moving button released.
     */
    void buttonRelease();

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

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
    /*!
     * Object contsructor.
     */
    explicit MusicCutSliderWidget(QWidget *parent = 0);

    ~MusicCutSliderWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current position.
     */
    void setPosition(qint64 position);
    /*!
     * Set current duration.
     */
    void setDuration(qint64 duration);
    /*!
     * Resize window bound by given width and height.
     */
    void resizeWindow(int width, int height);

Q_SIGNALS:
    /*!
     * Moving button pos changed.
     */
    void posChanged(qint64 start, qint64 end);
    /*!
     * Moving button pos release changed.
     */
    void buttonReleaseChanged(qint64 pos);

public Q_SLOTS:
    /*!
     * Moving button pos updated.
     */
    void buttonMoveUpdate();
    /*!
     * Left moving button released.
     */
    void buttonReleaseLeft();
    /*!
     * Left moving button released.
     */
    void buttonReleaseRight();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}

    MusicMoveButton *m_leftControl, *m_rightControl;
    int m_width, m_height;
    qint64 m_duration, m_position;

};

#endif // MUSICCUTSLIDERWIDGET_H
