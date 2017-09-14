#ifndef MUSICANIMATIONSTACKEDWIDGET_H
#define MUSICANIMATIONSTACKEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QPainter>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include "musicglobaldefine.h"

/*! @brief The class of the statckwidget switch animation.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    enum AnimationType
    {
        TopToBottom,    /*!< Animation from top to bottom*/
        BottomToTop,    /*!< Animation from bottom to top*/
        LeftToRight,    /*!< Animation from left to right*/
        RightToLeft     /*!< Animation from right to left*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicAnimationStackedWidget(QWidget *parent = 0);

    virtual ~MusicAnimationStackedWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to anmiation by index.
     */
    void start(int index);
    /*!
     * Set previoud and current index.
     */
    void setIndex(int previous, int current);
    /*!
     * Set animation start and end pos distance and animation type.
     */
    void setLength(int length, AnimationType type);
    /*!
     * Get previoud index.
     */
    int getPreviousIndex() const;
    /*!
     * Get current index.
     */
    int getCurrentIndex() const;

    /*!
     * Set animation duration.
     */
    void setDuration(int duration);
    /*!
     * Get animation duration.
     */
    int getDuration() const;

private Q_SLOTS:
    /*!
     * Animation value changed.
     */
    void valueChanged(const QVariant &value);
    /*!
     * Animation finished.
     */
    void animationFinished();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Render previous widget.
     */
    void renderPreviousWidget(QPainter &painter);
    /*!
     * Render current widget.
     */
    void renderCurrentWidget(QPainter &painter);

    bool m_isAnimating;
    float m_currentValue;
    int m_currentIndex, m_previousIndex;
    AnimationType m_type;
    QPropertyAnimation *m_animation;

};

#endif // MUSICANIMATIONSTACKEDWIDGET_H
