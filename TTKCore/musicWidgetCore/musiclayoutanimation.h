#ifndef MUSICLAYOUTANIMATION_H
#define MUSICLAYOUTANIMATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class QVBoxLayout;
class QPropertyAnimation;

/*! @brief The class of the layout float animation.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLayoutAnimation : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLayoutAnimation(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLayoutAnimation();

    void start();
    /*!
     * Start to anmiation.
     */
    void stop();
    /*!
     * Stop to anmiation.
     */

    void addWidget(QWidget *widget);
    /*!
     * Add widget to container.
     */

private slots:
    void valueChanged(const QVariant &value);
    /*!
     * Animation value changed.
     */
    void animationFinished();
    /*!
     * Animation finished.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_isAnimating;
    float m_currentValue;
    QPropertyAnimation *m_animation;
    QList<QWidget*> m_widgetItems;
    QWidget *m_mainWidget;
    QVBoxLayout *m_widgetLayout;

};

#endif // MUSICLAYOUTANIMATION_H
