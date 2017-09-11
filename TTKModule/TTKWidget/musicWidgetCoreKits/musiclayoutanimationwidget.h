#ifndef MUSICLAYOUTANIMATIONWIDGET_H
#define MUSICLAYOUTANIMATIONWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class QVBoxLayout;
class QPropertyAnimation;

/*! @brief The class of the layout float animation widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicLayoutAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLayoutAnimationWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLayoutAnimationWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void start();
    /*!
     * Start to anmiation.
     */
    void stop();
    /*!
     * Stop to anmiation.
     */

    void connectTo(QObject *parent);
    /*!
     * Connect to parent.
     */
    void addStretch(int stretch = 0);
    /*!
     * Add widget layout stretch.
     */
    void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = 0);
    /*!
     * Add widget to container.
     */
    void removeWidget(QWidget *widget);
    /*!
     * Remove widget to container.
     */

private Q_SLOTS:
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
    QWidget *m_mainWidget;
    QVBoxLayout *m_widgetLayout;
    QPropertyAnimation *m_animation;

};

#endif // MUSICLAYOUTANIMATIONWIDGET_H
